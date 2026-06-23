#include <boost/test/unit_test.hpp>
#include "command.hpp"
#include <sstream>
#include <string>

BOOST_AUTO_TEST_SUITE(CommandHandlerTests)

BOOST_AUTO_TEST_CASE(CmdGraphsEmpty)
{
  vishnyakov::CommandHandler handler;
  std::ostringstream out;
  handler.execute("graphs", out);
  BOOST_CHECK_EQUAL(out.str(), "\n");
}

BOOST_AUTO_TEST_CASE(CmdGraphsWithGraphs)
{
  vishnyakov::CommandHandler handler;
  handler.add_graph("gr2", vishnyakov::Graph());
  handler.add_graph("gr1", vishnyakov::Graph());
  handler.add_graph("gr3", vishnyakov::Graph());

  std::ostringstream out;
  handler.execute("graphs", out);

  std::string expected = "gr1\ngr2\ngr3\n";
  BOOST_CHECK_EQUAL(out.str(), expected);
}

BOOST_AUTO_TEST_CASE(CmdVertexes)
{
  vishnyakov::CommandHandler handler;
  vishnyakov::Graph g;
  g.add_edge("b", "c", 10);
  g.add_edge("a", "b", 20);
  handler.add_graph("test", g);

  std::ostringstream out;
  handler.execute("vertexes test", out);

  std::string expected = "a\nb\nc\n";
  BOOST_CHECK_EQUAL(out.str(), expected);
}

BOOST_AUTO_TEST_CASE(CmdVertexesNonExistingGraph)
{
  vishnyakov::CommandHandler handler;
  std::ostringstream out;
  handler.execute("vertexes nonexist", out);
  BOOST_CHECK_EQUAL(out.str(), "<INVALID COMMAND>\n");
}

BOOST_AUTO_TEST_CASE(CmdCreateNewGraph)
{
  vishnyakov::CommandHandler handler;
  std::ostringstream out;
  handler.execute("create newgraph", out);

  BOOST_CHECK(handler.has_graph("newgraph"));
  BOOST_CHECK(handler.get_graph("newgraph").empty());
}

BOOST_AUTO_TEST_CASE(CmdCreateExistingGraph)
{
  vishnyakov::CommandHandler handler;
  handler.add_graph("existing", vishnyakov::Graph());
  std::ostringstream out;
  handler.execute("create existing", out);
  BOOST_CHECK_EQUAL(out.str(), "<INVALID COMMAND>\n");
}

BOOST_AUTO_TEST_CASE(CmdBind)
{
  vishnyakov::CommandHandler handler;
  handler.add_graph("test", vishnyakov::Graph());
  std::ostringstream out;
  handler.execute("bind test a b 100", out);

  const auto& graph = handler.get_graph("test");
  BOOST_CHECK(graph.has_vertex("a"));
  BOOST_CHECK(graph.has_vertex("b"));
  BOOST_CHECK(graph.has_edge("a", "b", 100));
}

BOOST_AUTO_TEST_CASE(CmdBindNonExistingGraph)
{
  vishnyakov::CommandHandler handler;
  std::ostringstream out;
  handler.execute("bind nonexist a b 100", out);
  BOOST_CHECK_EQUAL(out.str(), "<INVALID COMMAND>\n");
}

BOOST_AUTO_TEST_CASE(CmdCut)
{
  vishnyakov::CommandHandler handler;
  handler.add_graph("test", vishnyakov::Graph());
  std::ostringstream dummy;
  handler.execute("bind test a b 100", dummy);
  handler.execute("bind test a b 200", dummy);

  std::ostringstream out;
  handler.execute("cut test a b 100", out);

  const auto& graph = handler.get_graph("test");
  BOOST_CHECK(!graph.has_edge("a", "b", 100));
  BOOST_CHECK(graph.has_edge("a", "b", 200));
}

BOOST_AUTO_TEST_CASE(CmdCutNonExistingEdge)
{
  vishnyakov::CommandHandler handler;
  handler.add_graph("test", vishnyakov::Graph());
  std::ostringstream dummy;
  handler.execute("bind test a b 100", dummy);

  std::ostringstream out;
  handler.execute("cut test a b 200", out);
  BOOST_CHECK_EQUAL(out.str(), "<INVALID COMMAND>\n");
}

BOOST_AUTO_TEST_CASE(CmdOutbound)
{
  vishnyakov::CommandHandler handler;
  vishnyakov::Graph g;
  g.add_edge("a", "b", 10);
  g.add_edge("a", "c", 20);
  g.add_edge("a", "b", 30);
  handler.add_graph("test", g);

  std::ostringstream out;
  handler.execute("outbound test a", out);

  std::string expected = "b 10 30\nc 20\n";
  BOOST_CHECK_EQUAL(out.str(), expected);
}

BOOST_AUTO_TEST_CASE(CmdInbound)
{
  vishnyakov::CommandHandler handler;
  vishnyakov::Graph g;
  g.add_edge("b", "a", 10);
  g.add_edge("c", "a", 20);
  g.add_edge("b", "a", 30);
  handler.add_graph("test", g);

  std::ostringstream out;
  handler.execute("inbound test a", out);

  std::string expected = "b 10 30\nc 20\n";
  BOOST_CHECK_EQUAL(out.str(), expected);
}

BOOST_AUTO_TEST_CASE(CmdMerge)
{
  vishnyakov::CommandHandler handler;
  vishnyakov::Graph g1;
  g1.add_edge("a", "b", 10);
  handler.add_graph("gr1", g1);

  vishnyakov::Graph g2;
  g2.add_edge("b", "c", 20);
  handler.add_graph("gr2", g2);

  std::ostringstream out;
  handler.execute("merge merged gr1 gr2", out);

  BOOST_CHECK(handler.has_graph("merged"));
  const auto& merged = handler.get_graph("merged");
  BOOST_CHECK(merged.has_edge("a", "b", 10));
  BOOST_CHECK(merged.has_edge("b", "c", 20));
}

BOOST_AUTO_TEST_CASE(CmdExtract)
{
  vishnyakov::CommandHandler handler;
  vishnyakov::Graph g;
  g.add_edge("a", "b", 10);
  g.add_edge("a", "c", 20);
  g.add_edge("b", "c", 30);
  g.add_edge("c", "a", 40);
  handler.add_graph("original", g);

  std::ostringstream out;
  handler.execute("extract extracted original 2 a c", out);

  BOOST_CHECK(handler.has_graph("extracted"));
  const auto& extracted = handler.get_graph("extracted");
  BOOST_CHECK(extracted.has_edge("a", "c", 20));
  BOOST_CHECK(extracted.has_edge("c", "a", 40));
  BOOST_CHECK(!extracted.has_edge("a", "b", 10));
  BOOST_CHECK(!extracted.has_edge("b", "c", 30));
}

BOOST_AUTO_TEST_CASE(InvalidCommand)
{
  vishnyakov::CommandHandler handler;
  std::ostringstream out;
  handler.execute("unknowncommand", out);
  BOOST_CHECK_EQUAL(out.str(), "<INVALID COMMAND>\n");
}

BOOST_AUTO_TEST_SUITE_END()

