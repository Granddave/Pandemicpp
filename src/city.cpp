#include "city.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <assert.h>

#include "utils.h"
#include "logging.h"

namespace Pandemic {

// -----------------------------------------------------------------------------
// City

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
    if (m_diseaseCubes.count(type) > 0 && m_diseaseCubes.at(type) > 0)
    {
        m_diseaseCubes.at(type)--;
    }
}

// -----------------------------------------------------------------------------
// City parser

CityParser::CityParser(const std::string& str)
{
    parseContent(str);
    createCities();
}

void CityParser::parseContent(const std::string& str)
{
    bool startCityFound = false;
    std::string line;
    std::stringstream ss(str);
    while(std::getline(ss, line))
    {
        line = trim(line);
        if (line.size() == 0)
        {
            break;
        }

        parsedCity city;
        if (line.at(0) == '*')
        {
            if (startCityFound)
            {
                LOG_CRIT("Only one city may be the starting city");
                assert(!startCityFound);
            }
            startCityFound = true;

            city.startCity = true;
            line.erase(0, 1); // Remove first character
        }

        std::vector<std::string> splittedLine = split(line, ' ');
        assert(splittedLine.size() > 2);
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
        LOG_CRIT("No start city found!");
        assert(startCityFound);
    }
}

void CityParser::createCities()
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
            if (*city == nullptr)
            {
                LOG_ERROR("Failed to find neighbour {} for {}",
                          m_parsedCities[i].neighbours[j],
                          m_parsedCities[i].name);
            }
            else
            {
                m_cities[i]->addNeighbour(*city);
            }
        }
    }
    LOG_DEBUG("Found {} cities", m_cities.size());
}

}
