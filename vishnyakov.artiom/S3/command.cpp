#include "command.hpp"
#include <sstream>
#include <string>

namespace vishnyakov
{
  CommandHandler::CommandHandler()
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
      out << "<INVALID COMMAND>" << '\n';
      return;
    }

    try
    {
      commands_.at(cmd_name)(iss, out);
    }
    catch (const std::exception&)
    {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void CommandHandler::cmd_graphs(std::istream&, std::ostream& out)
  {
    List< std::string > names;

    for (const auto& pair : graphs_)
    {
      names.push_back(pair.first);
    }

    for (typename List< std::string >::Iter it = names.begin(); it != names.end(); ++it)
    {
      typename List< std::string >::Iter min_it = it;

      for (typename List< std::string >::Iter jt = it; jt != names.end(); ++jt)
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

    for (const std::string& name : names)
    {
      out << name << '\n';
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

    for (const std::string& v : vertices)
    {
      out << v << '\n';
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
      return;
    }

    List< std::string > dest_vertices;

    for (const auto& pair : outbound)
    {
      dest_vertices.push_back(pair.first);
    }

    for (typename List< std::string >::Iter it = dest_vertices.begin(); it != dest_vertices.end(); ++it)
    {
      typename List< std::string >::Iter min_it = it;

      for (typename List< std::string >::Iter jt = it; jt != dest_vertices.end(); ++jt)
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

    for (const std::string& dest : dest_vertices)
    {
      out << dest;

      for (const auto& pair : outbound)
      {
        if (pair.first == dest)
        {
          List< unsigned long long > sorted_weights = graph.get_sorted_weights(pair.second);

          for (unsigned long long w : sorted_weights)
          {
            out << " " << w;
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
      return;
    }

    List< std::string > src_vertices;

    for (const auto& pair : inbound)
    {
      src_vertices.push_back(pair.first);
    }

    for (typename List< std::string >::Iter it = src_vertices.begin(); it != src_vertices.end(); ++it)
    {
      typename List< std::string >::Iter min_it = it;

      for (typename List< std::string >::Iter jt = it; jt != src_vertices.end(); ++jt)
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

    for (const std::string& src : src_vertices)
    {
      out << src;

      for (const auto& pair : inbound)
      {
        if (pair.first == src)
        {
          List< unsigned long long > sorted_weights = graph.get_sorted_weights(pair.second);

          for (unsigned long long w : sorted_weights)
          {
            out << " " << w;
          }

          break;
        }
      }

      out << '\n';
    }
  }

  void CommandHandler::cmd_bind(std::istream& in, std::ostream& out)
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

  void CommandHandler::cmd_cut(std::istream& in, std::ostream& out)
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

  void CommandHandler::cmd_create(std::istream& in, std::ostream& out)
  {
    std::string graph_name;
    in >> graph_name;

    if (graphs_.has(graph_name))
    {
      throw std::runtime_error("Graph already exists");
    }

    graphs_.add(graph_name, Graph());
  }

  void CommandHandler::cmd_merge(std::istream& in, std::ostream& out)
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

    for (const std::string& v : g1.get_vertices())
    {
      if (!merged.has_vertex(v))
      {
        merged.get_vertices().push_back(v);
      }
    }

    for (const std::string& v : g2.get_vertices())
    {
      if (!merged.has_vertex(v))
      {
        merged.get_vertices().push_back(v);
      }
    }

    for (const auto& edge_pair : g1.get_edges())
    {
      const auto& key = edge_pair.first;
      const auto& weights = edge_pair.second;

      for (unsigned long long w : weights)
      {
        merged.add_edge(key.first, key.second, w);
      }
    }

    for (const auto& edge_pair : g2.get_edges())
    {
      const auto& key = edge_pair.first;
      const auto& weights = edge_pair.second;

      for (unsigned long long w : weights)
      {
        merged.add_edge(key.first, key.second, w);
      }
    }

    graphs_.add(new_name, merged);
  }

  void CommandHandler::cmd_extract(std::istream& in, std::ostream& out)
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

    for (const std::string& v : vertices)
    {
      if (!old_graph.has_vertex(v))
      {
        throw std::runtime_error("Vertex not found");
      }
    }

    Graph extracted;

    for (const std::string& v : vertices)
    {
      extracted.get_vertices().push_back(v);
    }

    for (const std::string& from : vertices)
    {
      List< std::pair< std::string, List< unsigned long long > > > outbound = old_graph.get_outbound(from);

      for (const auto& pair : outbound)
      {
        bool to_included = false;

        for (const std::string& v : vertices)
        {
          if (v == pair.first)
          {
            to_included = true;
            break;
          }
        }

        if (to_included)
        {
          for (unsigned long long w : pair.second)
          {
            extracted.add_edge(from, pair.first, w);
          }
        }
      }
    }

    graphs_.add(new_name, extracted);
  }
}

