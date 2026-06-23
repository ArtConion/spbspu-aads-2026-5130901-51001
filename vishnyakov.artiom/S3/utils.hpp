#ifndef UTILS_HPP
#define UTILS_HPP

#include "command.hpp"
#include <istream>

namespace vishnyakov
{
  void parse_graph_file(std::istream& file, CommandHandler& handler);
  void process_commands(std::istream& in, CommandHandler& handler, std::ostream& out);
}

#endif

