#include "city.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <assert.h>

namespace Pandemic {

City::City(const std::string& name, const DiseaseType diseaseType)
    : m_name(name),
      m_diseaseType(diseaseType)
{
    for (size_t t = 0; t < c_numDiseases; t++)
    {
        m_diseaseCubes.insert({ static_cast<DiseaseType>(t), 0 });
    }
}

void City::addNeighbour(std::shared_ptr<City> other)
{
    auto it = std::find_if(m_neighbours.begin(), m_neighbours.end(),
                           [&](std::shared_ptr<City> const& p) {
        return p == other;
    });

    if (it == m_neighbours.end())
    {
        m_neighbours.push_back(other);
        other->addNeighbour(shared_from_this());
    }
}

bool City::addDisease(const DiseaseType type)
{
    const int num = m_diseaseCubes.at(type);
    if (num == c_maxCubesInCity)
    {
        // Trigger outbreak
        return true;
    }

    m_diseaseCubes.at(type)++;
    return false;
}

void City::cureDisease(const DiseaseType type)
{
    if (m_diseaseCubes.count(type) > 0)
    {
        m_diseaseCubes.at(type)--;
    }
}

std::vector<std::string> split(std::string strToSplit, char delimeter)
{
    std::stringstream ss(strToSplit);
    std::string item;
    std::vector<std::string> splittedStrings;
    while (std::getline(ss, item, delimeter))
    {
        splittedStrings.push_back(item);
    }
    return splittedStrings;
}

std::string titleCase(std::string str)
{
    std::replace(str.begin(), str.end(), '_', ' ');

    char last = ' ';
    std::for_each(str.begin(), str.end(), [&](char& c) {
        if(last == ' ' && c != ' ' && ::isalpha(c))
            c = static_cast<char>(::toupper(c));
        last = c;
    });

    return str;
}

CityReader::CityReader(const std::string& filepath)
{
    readFile(filepath);
    createCities();
}

void CityReader::parseString(const std::string &str)
{
    assert(str.length() != 0);
    std::istringstream iss(str);
    parseContent(iss);
    createCities();
}

void CityReader::readFile(const std::string& filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open() || !file.good())
    {
        std::cerr << "Failed to open file: " << filepath << std::endl;
        assert(0);
        return;
    }
    parseContent(file);
    file.close();
}

void CityReader::parseContent(std::istream& stream)
{
    bool startCityFound = false;
    std::string line;
    while(std::getline(stream, line))
    {
        if (line.size() == 0)
        {
            break;
        }
        parsedCity city;

        if (line.at(0) == '*')
        {
            assert(!startCityFound); // Only one city may be the starting city
            startCityFound = true;

            city.startCity = true;
            line.erase(0, 1); // Remove first character
        }

        std::vector<std::string> splittedLine = split(line, ' ');
        city.diseaseType = std::stoi(splittedLine.at(0));
        city.name = titleCase(splittedLine.at(1));

        for (size_t i = 2; i < splittedLine.size(); i++)
        {
            city.neighbours.push_back(titleCase(splittedLine.at(i)));
        }

        m_parsedCities.push_back(city);
    }
    if (!startCityFound)
    {
        std::cerr << "No start city found!" << std::endl;
        assert(startCityFound);
    }
}

void CityReader::createCities()
{
    for (const parsedCity& parsedCity : m_parsedCities)
    {
        auto diseaseType = static_cast<DiseaseType>(parsedCity.diseaseType);
        auto city = std::make_shared<City>(parsedCity.name, diseaseType);
        if (parsedCity.startCity)
        {
            m_startCity = city;
            city->setHasResearchStation(true);
        }
        m_cities.push_back(city);
    }

    for (size_t i = 0; i < m_parsedCities.size(); i++)
    {
        for (size_t j = 0; j < m_parsedCities[i].neighbours.size(); j++)
        {
            auto city = std::find_if(m_cities.begin(), m_cities.end(),
                                     [&](const std::shared_ptr<City>& c)
            {
                return c->getName() == m_parsedCities[i].neighbours[j];
            });
            if (*city != nullptr)
            {
                m_cities[i]->addNeighbour(*city);
            }
        }
    }
}



}
