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
    : m_name(titleCase(name)),
      m_diseaseType(diseaseType)
{
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
    if (numDiseaseCubes(type) == c_maxCubesInCity)
    {
        return true; // Trigger outbreak
    }

    m_diseaseCubes.push_back(type);
    return false;
}

void City::cureDisease(const DiseaseType type)
{
    const auto rmIt = std::find(m_diseaseCubes.begin(), m_diseaseCubes.end(), type);
    if (rmIt != m_diseaseCubes.cend())
    {
        m_diseaseCubes.erase(rmIt);
    }
}

int City::numDiseaseCubes() const
{
    return static_cast<int>(m_diseaseCubes.size());
}

int City::numDiseaseCubes(const DiseaseType type) const
{
    return static_cast<int>(std::count(m_diseaseCubes.begin(), m_diseaseCubes.end(), type));
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
            city->setResearchStation(true);
        }
        m_cities.push_back(city);
    }

    for (size_t cityIx = 0; cityIx < m_parsedCities.size(); cityIx++)
    {
        for (size_t neighIx = 0; neighIx < m_parsedCities[cityIx].neighbours.size(); neighIx++)
        {
            auto city = std::find_if(m_cities.begin(), m_cities.end(),
                                     [&](const std::shared_ptr<City>& c)
            {
                return c->name() == m_parsedCities[cityIx].neighbours[neighIx];
            });
            if (*city == nullptr)
            {
                LOG_ERROR("Failed to find neighbour {} for {}",
                          m_parsedCities[cityIx].neighbours[neighIx],
                          m_parsedCities[cityIx].name);
            }
            else
            {
                m_cities[cityIx]->addNeighbour(*city);
            }
        }
    }
    LOG_DEBUG("Found {} cities", m_cities.size());
}

}
