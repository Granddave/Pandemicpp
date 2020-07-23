#include <iostream>

#include "game.h"
#include "utils.h"

int main(int argc, char *argv[])
{
    UNUSED(argc); UNUSED(argv);

    pandemic::Config config;
    pandemic::Game game(config);
    return 0;
}
