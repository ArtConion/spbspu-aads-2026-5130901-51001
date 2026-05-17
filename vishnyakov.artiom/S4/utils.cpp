#include "utils.hpp"
#include <sstream>
#include <string>

namespace vishnyakov
{

void parse_dict_file(std::istream& file, BSTree< std::string, Dictionary, std::less< std::string > >& dicts)
{
  std::string line;

  while (std::getline(file, line))
  {
    if (line.empty())
    {
      continue;
    }

    std::istringstream iss(line);
    std::string dict_name;
    iss >> dict_name;

    if (dicts.has(dict_name))
    {
      throw std::runtime_error("Duplicate dictionary name");
    }

    Dictionary dict;
    int key;
    std::string value;

    while (iss >> key >> value)
    {
      dict.push(key, value);
    }

    dicts.push(dict_name, dict);
  }
}

void process_commands(std::istream& in, BSTree< std::string, Dictionary, std::less< std::string > >& dicts, std::ostream& out)
{
  std::string line;

  while (std::getline(in, line))
  {
    if (line.empty())
    {
      continue;
    }

    std::istringstream iss(line);
    std::string cmd;
    iss >> cmd;

    if (cmd == "print")
    {
      std::string name;
      iss >> name;

      if (!dicts.has(name))
      {
        out << "<INVALID COMMAND>\n";
        continue;
      }

      const Dictionary& dict = dicts.at(name);

      if (dict.empty())
      {
        out << "<EMPTY>\n";
        continue;
      }

      auto it = dict.begin();
      out << it->first << " " << it->second;
      ++it;

      for (; it != dict.end(); ++it)
      {
        out << " " << it->first << " " << it->second;
      }
      out << "\n";
    }
    else
    {
      out << "<INVALID COMMAND>\n";
    }
  }
}

}

