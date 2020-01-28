#include <iostream>

#include "game.h"

#define UNUSED(x) (void)(x)

int main(int argc, char *argv[])
{
    UNUSED(argc); UNUSED(argv);

    Pandemic::Config config;
    Pandemic::Game game(config);
    return 0;
}
