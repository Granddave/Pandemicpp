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

    std::string getName() const { return m_name; }

    void setHasResearchStation(const bool b) { m_hasResearchStation = b; }
    bool getHasResearchStation() const { return m_hasResearchStation; }

    void addNeighbour(std::shared_ptr<City> other);
    std::vector<std::shared_ptr<City>> getNeighbours() const { return m_neighbours; }

    std::map<DiseaseCube, int> getDiseaseCubes() const { return m_diseaseCubes; }
    bool addDisease(const DiseaseType type);
    void cureDisease(const DiseaseType type);

    DiseaseType getDiseaseType() const { return m_diseaseType; }

private:
    std::string m_name;
    bool m_hasResearchStation = false;
    //point2i m_coordinates;
    std::vector<std::shared_ptr<City>> m_neighbours;
    DiseaseType m_diseaseType;
    std::map<DiseaseCube, int> m_diseaseCubes;
};

class CityReader
{
public:
    CityReader() {}
    CityReader(const std::string& filepath);
    void parseString(const std::string& string);

    std::vector<std::shared_ptr<City>> getCities() const { return m_cities; }
    std::shared_ptr<City> getMainCity() const { return m_startCity; }

private:
    struct parsedCity
    {
        std::string name;
        std::vector<std::string> neighbours;
        int diseaseType = -1;
        bool startCity = false;
    };

    void readFile(const std::string& filepath);
    void parseContent(std::istream& stream);
    void createCities();

    std::vector<parsedCity> m_parsedCities;
    std::vector<std::shared_ptr<City>> m_cities;
    std::shared_ptr<City> m_startCity;
};


}

