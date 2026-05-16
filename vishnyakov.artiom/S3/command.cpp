#include "command.hpp"
#include <sstream>
#include <string>
#include <cstdlib>

namespace vishnyakov
{
  CommandHandler::CommandHandler():
    graphs_(),
    commands_()
  {
    commands_.add("graphs", [this](std::istream& in, std::ostream& out)
    {
      this->cmd_graphs(in, out);
    });

    commands_.add("vertexes", [this](std::istream& in, std::ostream& out)
    {
      this->cmd_vertexes(in, out);
    });

    commands_.add("outbound", [this](std::istream& in, std::ostream& out)
    {
      this->cmd_outbound(in, out);
    });

    commands_.add("inbound", [this](std::istream& in, std::ostream& out)
    {
      this->cmd_inbound(in, out);
    });

    commands_.add("bind", [this](std::istream& in, std::ostream& out)
    {
      this->cmd_bind(in, out);
    });

    commands_.add("cut", [this](std::istream& in, std::ostream& out)
    {
      this->cmd_cut(in, out);
    });

    commands_.add("create", [this](std::istream& in, std::ostream& out)
    {
      this->cmd_create(in, out);
    });

    commands_.add("merge", [this](std::istream& in, std::ostream& out)
    {
      this->cmd_merge(in, out);
    });

    commands_.add("extract", [this](std::istream& in, std::ostream& out)
    {
      this->cmd_extract(in, out);
    });
  }

  void CommandHandler::add_graph(const std::string& name, const Graph& graph)
  {
    graphs_.add(name, graph);
  }

  bool CommandHandler::has_graph(const std::string& name) const
  {
    return graphs_.has(name);
  }

  Graph& CommandHandler::get_graph(const std::string& name)
  {
    return graphs_.at(name);
  }

  const Graph& CommandHandler::get_graph(const std::string& name) const
  {
    return graphs_.at(name);
  }

  void CommandHandler::execute(const std::string& command_line, std::ostream& out)
  {
    std::istringstream iss(command_line);
    std::string cmd_name;
    iss >> cmd_name;

    if (!commands_.has(cmd_name))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    try
    {
      commands_.at(cmd_name)(iss, out);
    }
    catch (const std::exception&)
    {
      out << "<INVALID COMMAND>\n";
    }
  }

  void CommandHandler::cmd_graphs(std::istream&, std::ostream& out)
  {
    List< std::string > names = graphs_.get_keys();

    if (names.empty())
    {
      out << '\n';
      return;
    }

    for (LIter< std::string > it = names.begin(); it != names.end(); ++it)
    {
      LIter< std::string > min_it = it;

      for (LIter< std::string > jt = it; jt != names.end(); ++jt)
      {
        if (*jt < *min_it)
        {
          min_it = jt;
        }
      }

      if (min_it != it)
      {
        std::string tmp = *it;
        *it = *min_it;
        *min_it = tmp;
      }
    }

    for (LIter< std::string > it = names.begin(); it != names.end(); ++it)
    {
      out << *it << '\n';
    }
  }

  void CommandHandler::cmd_vertexes(std::istream& in, std::ostream& out)
  {
    std::string graph_name;
    in >> graph_name;

    if (!graphs_.has(graph_name))
    {
      throw std::runtime_error("Graph not found");
    }

    const Graph& graph = graphs_.at(graph_name);
    List< std::string > vertices = graph.get_sorted_vertices();

    if (vertices.empty())
    {
      out << '\n';
      return;
    }

    for (LIter< std::string > it = vertices.begin(); it != vertices.end(); ++it)
    {
      out << *it << '\n';
    }
  }

  void CommandHandler::cmd_outbound(std::istream& in, std::ostream& out)
  {
    std::string graph_name;
    std::string vertex;
    in >> graph_name >> vertex;

    if (!graphs_.has(graph_name))
    {
      throw std::runtime_error("Graph not found");
    }

    const Graph& graph = graphs_.at(graph_name);

    if (!graph.has_vertex(vertex))
    {
      throw std::runtime_error("Vertex not found");
    }

    List< std::pair< std::string, List< unsigned long long > > > outbound = graph.get_outbound(vertex);

    if (outbound.empty())
    {
      out << '\n';
      return;
    }

    List< std::string > dest_vertices;
    for (LIter< std::pair< std::string, List< unsigned long long > > > it = outbound.begin();
         it != outbound.end(); ++it)
    {
      dest_vertices.push_back(it->first);
    }

    for (LIter< std::string > it = dest_vertices.begin(); it != dest_vertices.end(); ++it)
    {
      LIter< std::string > min_it = it;
      for (LIter< std::string > jt = it; jt != dest_vertices.end(); ++jt)
      {
        if (*jt < *min_it)
        {
          min_it = jt;
        }
      }
      if (min_it != it)
      {
        std::string tmp = *it;
        *it = *min_it;
        *min_it = tmp;
      }
    }

    for (LIter< std::string > dit = dest_vertices.begin(); dit != dest_vertices.end(); ++dit)
    {
      const std::string& dest = *dit;
      out << dest;

      for (LIter< std::pair< std::string, List< unsigned long long > > > oit = outbound.begin();
           oit != outbound.end(); ++oit)
      {
        if (oit->first == dest)
        {
          List< unsigned long long > sorted_weights = graph.get_sorted_weights(oit->second);
          for (LIter< unsigned long long > wit = sorted_weights.begin();
               wit != sorted_weights.end(); ++wit)
          {
            out << " " << *wit;
          }
          break;
        }
      }
      out << '\n';
    }
  }

  void CommandHandler::cmd_inbound(std::istream& in, std::ostream& out)
  {
    std::string graph_name;
    std::string vertex;
    in >> graph_name >> vertex;

    if (!graphs_.has(graph_name))
    {
      throw std::runtime_error("Graph not found");
    }

    const Graph& graph = graphs_.at(graph_name);

    if (!graph.has_vertex(vertex))
    {
      throw std::runtime_error("Vertex not found");
    }

    List< std::pair< std::string, List< unsigned long long > > > inbound = graph.get_inbound(vertex);

    if (inbound.empty())
    {
      out << '\n';
      return;
    }

    List< std::string > src_vertices;
    for (LIter< std::pair< std::string, List< unsigned long long > > > it = inbound.begin();
         it != inbound.end(); ++it)
    {
      src_vertices.push_back(it->first);
    }

    for (LIter< std::string > it = src_vertices.begin(); it != src_vertices.end(); ++it)
    {
      LIter< std::string > min_it = it;
      for (LIter< std::string > jt = it; jt != src_vertices.end(); ++jt)
      {
        if (*jt < *min_it)
        {
          min_it = jt;
        }
      }
      if (min_it != it)
      {
        std::string tmp = *it;
        *it = *min_it;
        *min_it = tmp;
      }
    }

    for (LIter< std::string > sit = src_vertices.begin(); sit != src_vertices.end(); ++sit)
    {
      const std::string& src = *sit;
      out << src;

      for (LIter< std::pair< std::string, List< unsigned long long > > > iit = inbound.begin();
           iit != inbound.end(); ++iit)
      {
        if (iit->first == src)
        {
          List< unsigned long long > sorted_weights = graph.get_sorted_weights(iit->second);
          for (LIter< unsigned long long > wit = sorted_weights.begin();
              wit != sorted_weights.end(); ++wit)
          {
            out << " " << *wit;
          }
          break;
        }
      }
      out << '\n';
    }
  }

  void CommandHandler::cmd_bind(std::istream& in, std::ostream&)
  {
    std::string graph_name;
    std::string vertex_a;
    std::string vertex_b;
    unsigned long long weight;

    in >> graph_name >> vertex_a >> vertex_b >> weight;

    if (!graphs_.has(graph_name))
    {
      throw std::runtime_error("Graph not found");
    }

    Graph& graph = graphs_.at(graph_name);
    graph.add_edge(vertex_a, vertex_b, weight);
  }

  void CommandHandler::cmd_cut(std::istream& in, std::ostream&)
  {
    std::string graph_name;
    std::string vertex_a;
    std::string vertex_b;
    unsigned long long weight;

    in >> graph_name >> vertex_a >> vertex_b >> weight;

    if (!graphs_.has(graph_name))
    {
      throw std::runtime_error("Graph not found");
    }

    Graph& graph = graphs_.at(graph_name);

    if (!graph.has_vertex(vertex_a) || !graph.has_vertex(vertex_b))
    {
      throw std::runtime_error("Vertex not found");
    }

    if (!graph.has_edge(vertex_a, vertex_b, weight))
    {
      throw std::runtime_error("Edge not found");
    }

    graph.remove_edge(vertex_a, vertex_b, weight);
  }

  void CommandHandler::cmd_create(std::istream& in, std::ostream&)
  {
    std::string graph_name;
    in >> graph_name;

    if (graphs_.has(graph_name))
    {
      throw std::runtime_error("Graph already exists");
    }

    Graph new_graph;

    if (in >> std::ws && !in.eof())
    {
      std::string count_str;
      in >> count_str;
      std::size_t vertex_count = std::stoul(count_str);

      for (std::size_t i = 0; i < vertex_count; ++i)
      {
        std::string vertex;
        in >> vertex;
        new_graph.get_vertices().push_back(vertex);
      }
    }

    graphs_.add(graph_name, new_graph);
  }

  void CommandHandler::cmd_merge(std::istream& in, std::ostream&)
  {
    std::string new_name;
    std::string old_name1;
    std::string old_name2;

    in >> new_name >> old_name1 >> old_name2;

    if (graphs_.has(new_name))
    {
      throw std::runtime_error("Graph already exists");
    }

    if (!graphs_.has(old_name1) || !graphs_.has(old_name2))
    {
      throw std::runtime_error("Graph not found");
    }

    const Graph& g1 = graphs_.at(old_name1);
    const Graph& g2 = graphs_.at(old_name2);

    Graph merged;

    LCIter< std::string > it1 = g1.get_vertices().begin();
    LCIter< std::string > end1 = g1.get_vertices().end();

    for (; it1 != end1; ++it1)
    {
      if (!merged.has_vertex(*it1))
      {
        merged.get_vertices().push_back(*it1);
      }
    }

    LCIter< std::string > it2 = g2.get_vertices().begin();
    LCIter< std::string > end2 = g2.get_vertices().end();

    for (; it2 != end2; ++it2)
    {
      if (!merged.has_vertex(*it2))
      {
        merged.get_vertices().push_back(*it2);
      }
    }

    List< Graph::EdgeKey > keys1 = g1.get_edges().get_keys();
    LIter< Graph::EdgeKey > kit1 = keys1.begin();
    LIter< Graph::EdgeKey > kend1 = keys1.end();

    for (; kit1 != kend1; ++kit1)
    {
      const List< Graph::Weight >& weights = g1.get_edges().at(*kit1);
      LCIter< Graph::Weight > wit = weights.begin();
      LCIter< Graph::Weight > wend = weights.end();

      for (; wit != wend; ++wit)
      {
        merged.add_edge(kit1->first, kit1->second, *wit);
      }
    }

    List< Graph::EdgeKey > keys2 = g2.get_edges().get_keys();
    LIter< Graph::EdgeKey > kit2 = keys2.begin();
    LIter< Graph::EdgeKey > kend2 = keys2.end();

    for (; kit2 != kend2; ++kit2)
    {
      const List< Graph::Weight >& weights = g2.get_edges().at(*kit2);
      LCIter< Graph::Weight > wit = weights.begin();
      LCIter< Graph::Weight > wend = weights.end();

      for (; wit != wend; ++wit)
      {
        merged.add_edge(kit2->first, kit2->second, *wit);
      }
    }

    graphs_.add(new_name, merged);
  }

  void CommandHandler::cmd_extract(std::istream& in, std::ostream&)
  {
    std::string new_name;
    std::string old_name;
    std::size_t vertex_count;

    in >> new_name >> old_name >> vertex_count;

    List< std::string > vertices;

    for (std::size_t i = 0; i < vertex_count; ++i)
    {
      std::string v;
      in >> v;
      vertices.push_back(v);
    }

    if (graphs_.has(new_name))
    {
      throw std::runtime_error("Graph already exists");
    }

    if (!graphs_.has(old_name))
    {
      throw std::runtime_error("Graph not found");
    }

    const Graph& old_graph = graphs_.at(old_name);

    for (LIter< std::string > it = vertices.begin(); it != vertices.end(); ++it)
    {
      if (!old_graph.has_vertex(*it))
      {
        throw std::runtime_error("Vertex not found");
      }
    }

    Graph extracted;

    for (LIter< std::string > it = vertices.begin(); it != vertices.end(); ++it)
    {
      extracted.get_vertices().push_back(*it);
    }

    for (LIter< std::string > fit = vertices.begin(); fit != vertices.end(); ++fit)
    {
      const std::string& from = *fit;
      List< std::pair< std::string, List< unsigned long long > > > outbound = old_graph.get_outbound(from);

      for (LIter< std::pair< std::string, List< unsigned long long > > > oit = outbound.begin();
           oit != outbound.end(); ++oit)
      {
        bool to_included = false;

        for (LIter< std::string > vit = vertices.begin(); vit != vertices.end(); ++vit)
        {
          if (*vit == oit->first)
          {
            to_included = true;
            break;
          }
        }

        if (to_included)
        {
          for (LIter< unsigned long long > wit = oit->second.begin();
               wit != oit->second.end(); ++wit)
          {
            extracted.add_edge(from, oit->first, *wit);
          }
        }
      }
    }

    graphs_.add(new_name, extracted);
  }
}

