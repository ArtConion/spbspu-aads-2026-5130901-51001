#include "commands.hpp"
#include <iostream>

int main()
{
  using namespace vishnyakov;

  World world;

  processCommands(std::cin, world, std::cout);

  return 0;
}
