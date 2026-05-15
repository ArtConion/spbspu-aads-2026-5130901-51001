#include "graph.hpp"
#include <algorithm>

namespace vishnyakov
{
  void Graph::add_edge(const Vertex& from, const Vertex& to, Weight weight)
  {
    if (!has_vertex(from))
    {
      vertices_.push_back(from);
    }

    if (!has_vertex(to))
    {
      vertices_.push_back(to);
    }

    EdgeKey key(from, to);

    if (!edges_.has(key))
    {
      edges_.add(key, List< Weight >());
    }

    List< Weight >& weights = edges_.at(key);
    weights.push_back(weight);
  }

  void Graph::remove_edge(const Vertex& from, const Vertex& to, Weight weight)
  {
    EdgeKey key(from, to);

    if (!edges_.has(key))
    {
      throw std::runtime_error("Edge not found");
    }

    List< Weight >& weights = edges_.at(key);
    typename List< Weight >::Iter prev = weights.begin();
    typename List< Weight >::Iter it = weights.begin();

    for (; it != weights.end(); ++it)
    {
      if (*it == weight)
      {
        weights.erase_after(prev);
        return;
      }

      prev = it;
    }

    throw std::runtime_error("Weight not found");
  }

  bool Graph::has_vertex(const Vertex& v) const
  {
    for (const Vertex& vertex : vertices_)
    {
      if (vertex == v)
      {
        return true;
      }
    }

    return false;
  }

  bool Graph::has_edge(const Vertex& from, const Vertex& to, Weight weight) const
  {
    EdgeKey key(from, to);

    if (!edges_.has(key))
    {
      return false;
    }

    const List< Weight >& weights = edges_.at(key);

    for (Weight w : weights)
    {
      if (w == weight)
      {
        return true;
      }
    }

    return false;
  }

  List< std::pair< Graph::Vertex, List< Graph::Weight > > > Graph::get_outbound(const Vertex& v) const
  {
    List< std::pair< Vertex, List< Weight > > > result;

    for (const auto& key : edges_)
    {
      if (key.first == v)
      {
        result.push_back(std::make_pair(key.second, edges_.at(key)));
      }
    }

    return result;
  }

  List< std::pair< Graph::Vertex, List< Graph::Weight > > > Graph::get_inbound(const Vertex& v) const
  {
    List< std::pair< Vertex, List< Weight > > > result;

    for (const auto& key : edges_)
    {
      if (key.second == v)
      {
        result.push_back(std::make_pair(key.first, edges_.at(key)));
      }
    }

    return result;
  }
}

