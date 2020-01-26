#include "catch.hpp"

#include "city.h"

TEST_CASE("Test")
{
    Pandemic::City city("atlanta", Pandemic::DiseaseType::Red);

    REQUIRE(city.getName() == "atlanta");
    REQUIRE(city.getDiseaseType() == Pandemic::DiseaseType::Red);
}
