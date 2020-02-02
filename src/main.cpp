#include <iostream>

#include "game.h"
#include "utils.h"

int main(int argc, char *argv[])
{
    UNUSED(argc); UNUSED(argv);

    Pandemic::Config config;
    Pandemic::Game game(config);
    return 0;
}
