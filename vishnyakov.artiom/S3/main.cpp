#include "command.hpp"
#include "graph.hpp"
#include "siphash.hpp"
#include "../common/list.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

namespace vishnyakov
{
  void parse_graph_file(std::istream& file, CommandHandler& handler)
  {
    std::string line;

    while (std::getline(file, line))
    {
      if (line.empty())
      {
        continue;
      }

      std::istringstream iss(line);
      std::string graph_name;
      std::size_t edge_count;

      iss >> graph_name >> edge_count;

      if (handler.has_graph(graph_name))
      {
        throw std::runtime_error("Duplicate graph name");
      }

      Graph graph;

      for (std::size_t i = 0; i < edge_count; ++i)
      {
        if (!std::getline(file, line))
        {
          throw std::runtime_error("Unexpected end of file");
        }

        if (line.empty())
        {
          --i;
          continue;
        }

        std::istringstream edge_iss(line);
        std::string from;
        std::string to;
        unsigned long long weight;

        edge_iss >> from >> to >> weight;

        graph.add_edge(from, to, weight);
      }

      handler.add_graph(graph_name, graph);
    }
  }

  void process_commands(std::istream& in, CommandHandler& handler, std::ostream& out)
  {
    std::string line;

    while (std::getline(in, line))
    {
      if (line.empty())
      {
        continue;
      }

      handler.execute(line, out);
    }
  }
}

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

  CommandHandler handler;

  try
  {
    parse_graph_file(file, handler);
    process_commands(std::cin, handler, std::cout);
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}

