#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "hash_table.hpp"
#include "siphash.hpp"
#include "list.hpp"
#include <string>
#include <utility>

namespace vishnyakov
{
  class Graph
  {
  public:
    using Vertex = std::string;
    using Weight = unsigned long long;
    using EdgeKey = std::pair< Vertex, Vertex >;

    Graph() = default;
    Graph(const Graph&) = default;
    Graph(Graph&&) noexcept = default;
    ~Graph() = default;

    Graph& operator=(const Graph&) = default;
    Graph& operator=(Graph&&) noexcept = default;

    void add_edge(const Vertex& from, const Vertex& to, Weight weight);
    void remove_edge(const Vertex& from, const Vertex& to, Weight weight);
    bool has_vertex(const Vertex& v) const;
    bool has_edge(const Vertex& from, const Vertex& to, Weight weight) const;
    bool has_any_edge(const Vertex& from, const Vertex& to) const;
    bool empty() const;

    List< Vertex >& get_vertices()
    {
      return vertices_;
    }

    const List< Vertex >& get_vertices() const
    {
      return vertices_;
    }

    HashTable< EdgeKey, List< Weight >, SipHash, std::equal_to< EdgeKey > >& get_edges()
    {
      return edges_;
    }

    const HashTable< EdgeKey, List< Weight >, SipHash, std::equal_to< EdgeKey > >& get_edges() const
    {
      return edges_;
    }

    List< std::pair< Vertex, List< Weight > > > get_outbound(const Vertex& v) const;
    List< std::pair< Vertex, List< Weight > > > get_inbound(const Vertex& v) const;

    List< Vertex > get_sorted_vertices() const;
    List< Weight > get_sorted_weights(const List< Weight >& weights) const;

  private:
    List< Vertex > vertices_;
    HashTable< EdgeKey, List< Weight >, SipHash, std::equal_to< EdgeKey > > edges_;
  };
}

#endif

