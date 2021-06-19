#pragma once

#include <istream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "disease.h"

namespace pandemic
{
class City : public std::enable_shared_from_this<City>
{
public:
    City(const std::string& name, const DiseaseType diseaseType);

    void addNeighbour(std::shared_ptr<City> other);
    bool addDisease(const DiseaseType type);
    void setResearchStation(const bool b)
    {
        m_hasResearchStation = b;
    }
    void treatDisease(const DiseaseType type);

    std::string name() const
    {
        return m_name;
    }
    bool hasResearchStation() const
    {
        return m_hasResearchStation;
    }
    std::vector<std::shared_ptr<City>> neighbours() const
    {
        return m_neighbours;
    }
    DiseaseType diseaseType() const
    {
        return m_diseaseType;
    }
    std::vector<DiseaseType> diseaseCubes() const
    {
        return m_diseaseCubes;
    }
    int numDiseaseCubes() const;
    int numDiseaseCubes(const DiseaseType type) const;
    std::vector<DiseaseType> diseaseCubeTypes() const;

private:
    std::string m_name;
    bool m_hasResearchStation = false;
    // point2i m_coordinates;
    // int population;
    std::vector<std::shared_ptr<City>> m_neighbours;
    DiseaseType m_diseaseType;
    std::vector<DiseaseType> m_diseaseCubes;
};

class CityParser
{
public:
    CityParser(const std::string& str);

    std::vector<std::shared_ptr<City>> cities() const
    {
        return m_cities;
    }
    std::shared_ptr<City> startCity() const
    {
        return m_startCity;
    }

private:
    struct ParsedCity
    {
        std::string name;
        std::vector<std::string> neighbours;
        int diseaseType = -1;
        bool startCity = false;
    };

    void parseContent(const std::string& str);
    void createCities();

    std::vector<ParsedCity> m_parsedCities;
    std::vector<std::shared_ptr<City>> m_cities;
    std::shared_ptr<City> m_startCity;
};

} // namespace pandemic
