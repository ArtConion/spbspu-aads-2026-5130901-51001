#include <boost/test/unit_test.hpp>
#include "command.hpp"
#include "utils.hpp"
#include <sstream>
#include <string>

BOOST_AUTO_TEST_SUITE(FileParsingTests)

BOOST_AUTO_TEST_CASE(ParseValidFile)
{
  std::string content = "gr1 3\na b 10\nb c 20\nc a 30\n\ngr2 2\na b 40\nb c 50\n";
  std::istringstream file(content);
  vishnyakov::CommandHandler handler;

  vishnyakov::parse_graph_file(file, handler);

  BOOST_CHECK(handler.has_graph("gr1"));
  BOOST_CHECK(handler.has_graph("gr2"));

  const auto& gr1 = handler.get_graph("gr1");
  BOOST_CHECK(gr1.has_edge("a", "b", 10));
  BOOST_CHECK(gr1.has_edge("b", "c", 20));
  BOOST_CHECK(gr1.has_edge("c", "a", 30));

  const auto& gr2 = handler.get_graph("gr2");
  BOOST_CHECK(gr2.has_edge("a", "b", 40));
  BOOST_CHECK(gr2.has_edge("b", "c", 50));
}

BOOST_AUTO_TEST_CASE(ParseFileWithEmptyLines)
{
  std::string content = "\n\ngr1 2\n\na b 10\n\nb c 20\n\n\n";
  std::istringstream file(content);
  vishnyakov::CommandHandler handler;

  vishnyakov::parse_graph_file(file, handler);

  BOOST_CHECK(handler.has_graph("gr1"));
  const auto& gr1 = handler.get_graph("gr1");
  BOOST_CHECK(gr1.has_edge("a", "b", 10));
  BOOST_CHECK(gr1.has_edge("b", "c", 20));
}

BOOST_AUTO_TEST_CASE(ParseFileMultipleSpaces)
{
  std::string content = "gr1   2\na   b   10\nb    c    20\n";
  std::istringstream file(content);
  vishnyakov::CommandHandler handler;

  vishnyakov::parse_graph_file(file, handler);

  BOOST_CHECK(handler.has_graph("gr1"));
  const auto& gr1 = handler.get_graph("gr1");
  BOOST_CHECK(gr1.has_edge("a", "b", 10));
  BOOST_CHECK(gr1.has_edge("b", "c", 20));
}

BOOST_AUTO_TEST_CASE(ParseFileWithZeroEdges)
{
  std::string content = "empty 0\n";
  std::istringstream file(content);
  vishnyakov::CommandHandler handler;

  vishnyakov::parse_graph_file(file, handler);

  BOOST_CHECK(handler.has_graph("empty"));
  const auto& graph = handler.get_graph("empty");
  BOOST_CHECK(graph.empty());
}

BOOST_AUTO_TEST_CASE(ParseFileLargeEdgeCount)
{
  std::string content = "large 3\na b 10\nb c 20\nc d 30\n";
  std::istringstream file(content);
  vishnyakov::CommandHandler handler;

  vishnyakov::parse_graph_file(file, handler);

  BOOST_CHECK(handler.has_graph("large"));
  const auto& graph = handler.get_graph("large");
  BOOST_CHECK(graph.has_edge("a", "b", 10));
  BOOST_CHECK(graph.has_edge("b", "c", 20));
  BOOST_CHECK(graph.has_edge("c", "d", 30));
  BOOST_CHECK_EQUAL(graph.get_vertices().size(), 4);
}

BOOST_AUTO_TEST_CASE(ParseFileDuplicateGraphNames)
{
  std::string content = "dup 1\na b 10\ndup 1\nc d 20\n";
  std::istringstream file(content);
  vishnyakov::CommandHandler handler;

  BOOST_CHECK_THROW(vishnyakov::parse_graph_file(file, handler), std::runtime_error);
}

BOOST_AUTO_TEST_SUITE_END()

