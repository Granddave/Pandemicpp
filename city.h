#pragma once

#include <string>
#include <memory>
#include <vector>

#include "disease.h"

namespace Pandemic {

class City : public std::enable_shared_from_this<City>
{
public:
    City(const std::string& name, const DiseaseType diseaseType);

    std::string getName() const { return m_name; }

    bool setHasResearchStation(const bool b) { m_hasResearchStation = b; }
    bool getHasResearchStation() const { return m_hasResearchStation; }

    void addNeighbour(std::shared_ptr<City> other);
    //std::vector<std::shared_ptr<City>> getNeighbours() const;

    void addDisease(const DiseaseType type);
    void addDisease();
    void cureDisease();

    DiseaseType getDiseaseType() const { return m_diseaseType; }

private:
    std::string m_name;
    bool m_hasResearchStation = false;
    //point2i m_coordinates;
    std::vector<std::shared_ptr<City>> m_neighbours;
    DiseaseType m_diseaseType;
    std::vector<DiseaseCube> m_diseaseCubes;
};

}
