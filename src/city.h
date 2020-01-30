#pragma once

#include <string>
#include <istream>
#include <memory>
#include <vector>
#include <map>

#include "disease.h"

namespace Pandemic {

class City : public std::enable_shared_from_this<City>
{
public:
    City(const std::string& name, const DiseaseType diseaseType);

    void addNeighbour(std::shared_ptr<City> other);
    bool addDisease(const DiseaseType type);
    void setHasResearchStation(const bool b) { m_hasResearchStation = b; }
    void cureDisease(const DiseaseType type);

    std::string getName() const { return m_name; }
    bool getHasResearchStation() const { return m_hasResearchStation; }
    std::vector<std::shared_ptr<City>> getNeighbours() const { return m_neighbours; }
    DiseaseType getDiseaseType() const { return m_diseaseType; }
    std::map<DiseaseCube, int> getDiseaseCubes() const { return m_diseaseCubes; }
    int getNumDiseaseCubes(DiseaseType type) const { return m_diseaseCubes.at(type); }

private:
    std::string m_name;
    bool m_hasResearchStation = false;
    //point2i m_coordinates;
    std::vector<std::shared_ptr<City>> m_neighbours;
    DiseaseType m_diseaseType;
    std::map<DiseaseCube, int> m_diseaseCubes;
};

class CityParser
{
public:
    CityParser(const std::string& str);

    std::vector<std::shared_ptr<City>> getCities() const { return m_cities; }
    std::shared_ptr<City> getStartCity() const { return m_startCity; }

private:
    struct parsedCity
    {
        std::string name;
        std::vector<std::string> neighbours;
        int diseaseType = -1;
        bool startCity = false;
    };

    void parseContent(const std::string& str);
    void createCities();

    std::vector<parsedCity> m_parsedCities;
    std::vector<std::shared_ptr<City>> m_cities;
    std::shared_ptr<City> m_startCity;
};


}

