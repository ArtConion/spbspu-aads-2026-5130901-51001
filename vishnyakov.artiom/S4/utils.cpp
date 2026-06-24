#include "utils.hpp"
#include <sstream>
#include <string>

namespace vishnyakov
{
  Dictionary complement(const Dictionary& a, const Dictionary& b)
  {
    Dictionary result;

    for (auto it = a.begin(); it != a.end(); ++it)
    {
      if (!b.has(it->first))
      {
        result.push(it->first, it->second);
      }
    }

    return result;
  }

  Dictionary intersect(const Dictionary& a, const Dictionary& b)
  {
    Dictionary result;

    for (auto it = a.begin(); it != a.end(); ++it)
    {
      if (b.has(it->first))
      {
        result.push(it->first, it->second);
      }
    }

    return result;
  }

  Dictionary unite(const Dictionary& a, const Dictionary& b)
  {
    Dictionary result = a;

    for (auto it = b.begin(); it != b.end(); ++it)
    {
      if (!result.has(it->first))
      {
        result.push(it->first, it->second);
      }
    }

    return result;
  }

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

        out << name << " ";
        bool first = true;
        for (auto it = dict.begin(); it != dict.end(); ++it)
        {
          if (!first)
          {
            out << " ";
          }
          out << it->first << " " << it->second;
          first = false;
        }
        out << "\n";
      }
      else if (cmd == "complement")
      {
        std::string new_name, name1, name2;
        iss >> new_name >> name1 >> name2;

        if (!dicts.has(name1) || !dicts.has(name2))
        {
          out << "<INVALID COMMAND>\n";
          continue;
        }

        const Dictionary& dict1 = dicts.at(name1);
        const Dictionary& dict2 = dicts.at(name2);

        Dictionary result = complement(dict1, dict2);

        if (dicts.has(new_name))
        {
          dicts.drop(new_name);
        }

        dicts.push(new_name, result);
      }
      else if (cmd == "intersect")
      {
        std::string new_name, name1, name2;
        iss >> new_name >> name1 >> name2;

        if (!dicts.has(name1) || !dicts.has(name2))
        {
          out << "<INVALID COMMAND>\n";
          continue;
        }

        const Dictionary& dict1 = dicts.at(name1);
        const Dictionary& dict2 = dicts.at(name2);

        Dictionary result = intersect(dict1, dict2);

        if (dicts.has(new_name))
        {
          dicts.drop(new_name);
        }

        dicts.push(new_name, result);
      }
      else if (cmd == "union")
      {
        std::string new_name, name1, name2;
        iss >> new_name >> name1 >> name2;

        if (!dicts.has(name1) || !dicts.has(name2))
        {
          out << "<INVALID COMMAND>\n";
          continue;
        }

        const Dictionary& dict1 = dicts.at(name1);
        const Dictionary& dict2 = dicts.at(name2);

        Dictionary result = unite(dict1, dict2);

        if (dicts.has(new_name))
        {
          dicts.drop(new_name);
        }

        dicts.push(new_name, result);
      }
      else
      {
        out << "<INVALID COMMAND>\n";
      }
    }
  }
}

