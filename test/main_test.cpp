#define CATCH_CONFIG_RUNNER
#include "catch2/catch.hpp"

#include "logging.h"

int main(int argc, char* argv[])
{
    pandemic::setupLog("off");
    return Catch::Session().run(argc, argv);
}
