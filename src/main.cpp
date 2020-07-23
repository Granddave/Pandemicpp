#include <iostream>

#include "game.h"
#include "instrumentor.h"
#include "utils.h"

int main(int argc, char *argv[])
{
    UNUSED(argc); UNUSED(argv);

    PROFILE_BEGIN_SESSION("Pandemic", "trace.json");
    Pandemic::Config config;
    Pandemic::Game game(config);
    PROFILE_END_SESSION();
    return 0;
}
