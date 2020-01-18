#include "city.h"

#include <algorithm>

namespace Pandemic {

City::City(const std::string& name, const DiseaseType diseaseType)
    : m_name(name),
      m_diseaseType(diseaseType)
{

}

void City::addNeighbour(std::shared_ptr<City> other)
{
    auto it = std::find_if(m_neighbours.begin(),
                           m_neighbours.end(),
                           [&](std::shared_ptr<City> const& p) {
        return p->getName() == other->getName();
    });

    if (it == m_neighbours.end())
    {
        m_neighbours.push_back(other);
        other->addNeighbour(shared_from_this());
    }
}

void City::addDisease(const DiseaseType type)
{
    m_diseaseCubes.push_back(type);
}

void City::addDisease()
{
    addDisease(m_diseaseType);
}


}
