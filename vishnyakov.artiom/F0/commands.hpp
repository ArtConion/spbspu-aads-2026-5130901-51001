#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "world.hpp"
#include <iostream>
#include <string>

namespace vishnyakov
{
  void processCommands(std::istream& in, World& world, std::ostream& out);
}

#endif

