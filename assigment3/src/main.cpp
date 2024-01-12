#include <SFML/Graphics.hpp>
#include "gameEngine.h"
int main() {
  GameEngine g("include/assets.txt");
  g.run();
}
