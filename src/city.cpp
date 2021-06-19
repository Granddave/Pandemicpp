#include "city.h"

#include <algorithm>
#include <iostream>
#include <sstream>

#include "logging.h"
#include "utils.h"

namespace pandemic
{
// -----------------------------------------------------------------------------
// City

City::City(const std::string& name, const DiseaseType diseaseType)
    : m_name(titleCase(name)), m_diseaseType(diseaseType)
{
}

void City::addNeighbour(std::shared_ptr<City> other)
{
    const auto it = std::find_if(m_neighbours.begin(),
                                 m_neighbours.end(),
                                 [&](std::shared_ptr<City> const& p) { return p == other; });

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

void City::treatDisease(const DiseaseType type)
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

std::vector<DiseaseType> City::diseaseCubeTypes() const
{
    std::vector<DiseaseType> types;
    for (int i = 0; i < c_numDiseases; ++i)
    {
        const auto type = static_cast<DiseaseType>(i);
        if (std::count(m_diseaseCubes.begin(), m_diseaseCubes.end(), type) > 0)
        {
            types.emplace_back(type);
        }
    }
    return types;
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
    // City parser format:
    //
    // ['*' if starting city]['0-3' disease type] ['city_name'] [Neighbouring cities...]
    //
    // Short example:
    // ...
    // 1 tokyo san_francisco osaka shanghai seoul
    // *2 atlanta washington miami chicago
    // 2 chicago montreal atlanta mexico_city los_angeles san_francisco
    // ...
    //
    bool startCityFound = false;
    std::string line;
    std::stringstream ss(str);
    while (std::getline(ss, line))
    {
        line = trim(line);
        if (line.empty())
        {
            break;
        }

        ParsedCity city;
        if (line.at(0) == '*')
        {
            VERIFY_LOG(!startCityFound, "Only one city may be the starting city");
            startCityFound = true;

            city.startCity = true;
            line.erase(0, 1); // Remove first character
        }

        const std::vector<std::string> splittedLine = split(line, ' ');
        VERIFY(splittedLine.size() > 2);
        city.diseaseType = std::stoi(splittedLine.at(0));
        const bool validDiseaseType = city.diseaseType >= 0 && city.diseaseType < c_numDiseases;
        VERIFY_LOG(validDiseaseType,
                   "Disease type out of range ({}) for city: {}",
                   city.name,
                   city.diseaseType);

        city.name = titleCase(splittedLine.at(1));

        for (size_t i = 2; i < splittedLine.size(); i++)
        {
            city.neighbours.push_back(titleCase(splittedLine.at(i)));
        }

        m_parsedCities.push_back(city);
    }
    VERIFY_LOG(startCityFound, "No start city found!");
}

void CityParser::createCities()
{
    for (const ParsedCity& parsedCity : m_parsedCities)
    {
        const auto diseaseType = static_cast<DiseaseType>(parsedCity.diseaseType);
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
            auto city =
                std::find_if(m_cities.begin(), m_cities.end(), [&](const std::shared_ptr<City>& c) {
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

} // namespace pandemic
