#include "utils.hpp"
#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[])
{
  using namespace vishnyakov;

  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
    return 1;
  }

  std::ifstream file(argv[1]);

  if (!file.is_open())
  {
    std::cerr << "Error: Cannot open file " << argv[1] << std::endl;
    return 1;
  }

  BSTree< std::string, Dictionary, std::less< std::string > > dicts;

  try
  {
    parse_dict_file(file, dicts);
    process_commands(std::cin, dicts, std::cout);
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}

