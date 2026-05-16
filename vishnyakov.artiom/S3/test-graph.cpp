#include <boost/test/unit_test.hpp>
#include "graph.hpp"
#include <string>

BOOST_AUTO_TEST_SUITE(GraphEdgeTests)

BOOST_AUTO_TEST_CASE(AddSingleEdge)
{
  vishnyakov::Graph g;
  g.add_edge("a", "b", 10);

  BOOST_CHECK(g.has_vertex("a"));
  BOOST_CHECK(g.has_vertex("b"));
  BOOST_CHECK(g.has_edge("a", "b", 10));
  BOOST_CHECK(!g.has_edge("a", "b", 20));
}

BOOST_AUTO_TEST_CASE(AddMultipleEdgesSamePair)
{
  vishnyakov::Graph g;
  g.add_edge("a", "b", 10);
  g.add_edge("a", "b", 20);
  g.add_edge("a", "b", 30);

  BOOST_CHECK(g.has_edge("a", "b", 10));
  BOOST_CHECK(g.has_edge("a", "b", 20));
  BOOST_CHECK(g.has_edge("a", "b", 30));
  BOOST_CHECK(g.has_any_edge("a", "b"));
}

BOOST_AUTO_TEST_CASE(AddEdgesDifferentPairs)
{
  vishnyakov::Graph g;
  g.add_edge("a", "b", 10);
  g.add_edge("b", "c", 20);
  g.add_edge("c", "a", 30);

  BOOST_CHECK(g.has_edge("a", "b", 10));
  BOOST_CHECK(g.has_edge("b", "c", 20));
  BOOST_CHECK(g.has_edge("c", "a", 30));
  BOOST_CHECK(!g.has_edge("a", "c", 10));
}

BOOST_AUTO_TEST_CASE(RemoveEdge)
{
  vishnyakov::Graph g;
  g.add_edge("a", "b", 10);
  g.add_edge("a", "b", 20);

  g.remove_edge("a", "b", 10);

  BOOST_CHECK(!g.has_edge("a", "b", 10));
  BOOST_CHECK(g.has_edge("a", "b", 20));
}

BOOST_AUTO_TEST_CASE(RemoveLastEdge)
{
  vishnyakov::Graph g;
  g.add_edge("a", "b", 10);

  g.remove_edge("a", "b", 10);

  BOOST_CHECK(!g.has_edge("a", "b", 10));
  BOOST_CHECK(!g.has_any_edge("a", "b"));
}

BOOST_AUTO_TEST_CASE(RemoveNonExistingEdge)
{
  vishnyakov::Graph g;
  g.add_edge("a", "b", 10);

  BOOST_CHECK_THROW(g.remove_edge("a", "b", 20), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(RemoveFromNonExistingVertex)
{
  vishnyakov::Graph g;

  BOOST_CHECK_THROW(g.remove_edge("a", "b", 10), std::runtime_error);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(GraphVertexTests)

BOOST_AUTO_TEST_CASE(GetVertices)
{
  vishnyakov::Graph g;
  g.add_edge("b", "c", 10);
  g.add_edge("a", "b", 20);

  const auto& vertices = g.get_vertices();
  bool has_a = false;
  bool has_b = false;
  bool has_c = false;

  for (const auto& v : vertices)
  {
    if (v == "a") has_a = true;
    if (v == "b") has_b = true;
    if (v == "c") has_c = true;
  }

  BOOST_CHECK(has_a);
  BOOST_CHECK(has_b);
  BOOST_CHECK(has_c);
}

BOOST_AUTO_TEST_CASE(GetSortedVertices)
{
  vishnyakov::Graph g;
  g.add_edge("c", "d", 10);
  g.add_edge("a", "b", 20);
  g.add_edge("b", "c", 30);

  vishnyakov::List< std::string > sorted = g.get_sorted_vertices();
  vishnyakov::List< std::string > expected;
  expected.push_back("a");
  expected.push_back("b");
  expected.push_back("c");
  expected.push_back("d");

  auto it1 = sorted.begin();
  auto it2 = expected.begin();

  for (; it1 != sorted.end() && it2 != expected.end(); ++it1, ++it2)
  {
    BOOST_CHECK_EQUAL(*it1, *it2);
  }
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(GraphOutboundTests)

BOOST_AUTO_TEST_CASE(GetOutbound)
{
  vishnyakov::Graph g;
  g.add_edge("a", "b", 10);
  g.add_edge("a", "c", 20);
  g.add_edge("a", "b", 30);
  g.add_edge("b", "c", 40);

  auto outbound = g.get_outbound("a");

  bool found_b = false;
  bool found_c = false;

  for (const auto& pair : outbound)
  {
    if (pair.first == "b")
    {
      found_b = true;

      bool has_10 = false;
      bool has_30 = false;

      for (auto w : pair.second)
      {
        if (w == 10) has_10 = true;
        if (w == 30) has_30 = true;
      }

      BOOST_CHECK(has_10);
      BOOST_CHECK(has_30);
    }

    if (pair.first == "c")
    {
      found_c = true;

      bool has_20 = false;

      for (auto w : pair.second)
      {
        if (w == 20) has_20 = true;
      }

      BOOST_CHECK(has_20);
    }
  }

  BOOST_CHECK(found_b);
  BOOST_CHECK(found_c);
}

BOOST_AUTO_TEST_CASE(GetOutboundEmpty)
{
  vishnyakov::Graph g;
  g.add_edge("b", "c", 10);

  auto outbound = g.get_outbound("a");
  BOOST_CHECK(outbound.empty());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(GraphInboundTests)

BOOST_AUTO_TEST_CASE(GetInbound)
{
  vishnyakov::Graph g;
  g.add_edge("b", "a", 10);
  g.add_edge("c", "a", 20);
  g.add_edge("b", "a", 30);
  g.add_edge("b", "c", 40);

  auto inbound = g.get_inbound("a");

  bool found_b = false;
  bool found_c = false;

  for (const auto& pair : inbound)
  {
    if (pair.first == "b")
    {
      found_b = true;

      bool has_10 = false;
      bool has_30 = false;

      for (auto w : pair.second)
      {
        if (w == 10) has_10 = true;
        if (w == 30) has_30 = true;
      }

      BOOST_CHECK(has_10);
      BOOST_CHECK(has_30);
    }

    if (pair.first == "c")
    {
      found_c = true;

      bool has_20 = false;

      for (auto w : pair.second)
      {
        if (w == 20) has_20 = true;
      }

      BOOST_CHECK(has_20);
    }
  }

  BOOST_CHECK(found_b);
  BOOST_CHECK(found_c);
}

BOOST_AUTO_TEST_CASE(GetInboundEmpty)
{
  vishnyakov::Graph g;
  g.add_edge("a", "b", 10);

  auto inbound = g.get_inbound("c");
  BOOST_CHECK(inbound.empty());
}

BOOST_AUTO_TEST_SUITE_END()

