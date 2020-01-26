#include <iostream>

#include "board.h"

#define UNUSED(x) (void)(x)

int main(int argc, char *argv[])
{
    UNUSED(argc); UNUSED(argv);

    Pandemic::Config config;
    Pandemic::Board game(config);
    return 0;
}
