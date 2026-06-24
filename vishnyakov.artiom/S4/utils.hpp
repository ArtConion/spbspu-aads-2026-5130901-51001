#ifndef UTILS_HPP
#define UTILS_HPP

#include "bstree.hpp"
#include <istream>
#include <string>

namespace vishnyakov
{
  using Dictionary = BSTree< int, std::string, std::less< int > >;

  void parse_dict_file(std::istream& file, BSTree< std::string, Dictionary, std::less< std::string > >& dicts);
  void process_commands(std::istream& in, BSTree< std::string, Dictionary, std::less< std::string > >& dicts, std::ostream& out);
}

#endif

