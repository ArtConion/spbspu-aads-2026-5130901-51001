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

    if (weights.empty())
    {
      throw std::runtime_error("No edges found");
    }

    if (weights.front() == weight)
    {
      weights.pop_front();

      if (weights.empty())
      {
        edges_.drop(key);
      }

      return;
    }

    LIter< Weight > prev = weights.begin();
    LIter< Weight > it = weights.begin();
    ++it;

    for (; it != weights.end(); ++it)
    {
      if (*it == weight)
      {
        weights.erase_after(prev);

        if (weights.empty())
        {
          edges_.drop(key);
        }

        return;
      }

      prev = it;
    }

    throw std::runtime_error("Weight not found");
  }

  bool Graph::has_vertex(const Vertex& v) const
  {
    for (LCIter< Vertex > it = vertices_.begin(); it != vertices_.end(); ++it)
    {
      if (*it == v)
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

    for (LCIter< Weight > it = weights.begin(); it != weights.end(); ++it)
    {
      if (*it == weight)
      {
        return true;
      }
    }

    return false;
  }

  bool Graph::has_any_edge(const Vertex& from, const Vertex& to) const
  {
    EdgeKey key(from, to);
    return edges_.has(key);
  }

  bool Graph::empty() const
  {
    return vertices_.empty();
  }

  List< std::pair< Graph::Vertex, List< Graph::Weight > > > Graph::get_outbound(const Vertex& v) const
  {
    List< std::pair< Vertex, List< Weight > > > result;
    List< EdgeKey > keys = edges_.get_keys();

    for (LCIter< EdgeKey > kit = keys.begin(); kit != keys.end(); ++kit)
    {
      if (kit->first == v)
      {
        result.push_back(std::make_pair(kit->second, edges_.at(*kit)));
      }
    }

    return result;
  }

  List< std::pair< Graph::Vertex, List< Graph::Weight > > > Graph::get_inbound(const Vertex& v) const
  {
    List< std::pair< Vertex, List< Weight > > > result;
    List< EdgeKey > keys = edges_.get_keys();

    for (LCIter< EdgeKey > kit = keys.begin(); kit != keys.end(); ++kit)
    {
      if (kit->second == v)
      {
        result.push_back(std::make_pair(kit->first, edges_.at(*kit)));
      }
    }

    return result;
  }

  List< Graph::Vertex > Graph::get_sorted_vertices() const
  {
    List< Vertex > sorted;

    for (LCIter< Vertex > it = vertices_.begin(); it != vertices_.end(); ++it)
    {
      sorted.push_back(*it);
    }

    for (LIter< Vertex > it = sorted.begin(); it != sorted.end(); ++it)
    {
      LIter< Vertex > min_it = it;

      for (LIter< Vertex > jt = it; jt != sorted.end(); ++jt)
      {
        if (*jt < *min_it)
        {
          min_it = jt;
        }
      }

      if (min_it != it)
      {
        Vertex tmp = *it;
        *it = *min_it;
        *min_it = tmp;
      }
    }

    return sorted;
  }

  List< Graph::Weight > Graph::get_sorted_weights(const List< Weight >& weights) const
  {
    List< Weight > sorted;

    for (LCIter< Weight > it = weights.begin(); it != weights.end(); ++it)
    {
      sorted.push_back(*it);
    }

    for (LIter< Weight > it = sorted.begin(); it != sorted.end(); ++it)
    {
      LIter< Weight > min_it = it;

      for (LIter< Weight > jt = it; jt != sorted.end(); ++jt)
      {
        if (*jt < *min_it)
        {
          min_it = jt;
        }
      }

      if (min_it != it)
      {
        Weight tmp = *it;
        *it = *min_it;
        *min_it = tmp;
      }
    }

    return sorted;
  }
}

