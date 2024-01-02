#include <SFML/Graphics.hpp>
#include "game.h"


int main(int argc, char *argv[])
{
    Game g("config.txt");
    g.run();
    return 0;
}


