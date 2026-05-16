#include <boost/test/unit_test.hpp>
#include "command.hpp"
#include "utils.hpp"
#include <sstream>
#include <string>

BOOST_AUTO_TEST_SUITE(IntegrationTests)

BOOST_AUTO_TEST_CASE(FullWorkflow)
{
  std::string file_content = "school 4\na b 5\nb c 3\na c 7\nc b 2\n";
  std::istringstream file(file_content);

  vishnyakov::CommandHandler handler;
  vishnyakov::parse_graph_file(file, handler);

  std::ostringstream out;

  handler.execute("graphs", out);
  handler.execute("vertexes school", out);
  handler.execute("outbound school a", out);
  handler.execute("inbound school b", out);

  std::string expected = "school\na\nb\nc\nb 5\nc 7\na 5\nc 2\n";
  BOOST_CHECK_EQUAL(out.str(), expected);
}

BOOST_AUTO_TEST_CASE(CreateAndBindThenQuery)
{
  vishnyakov::CommandHandler handler;
  std::ostringstream out;

  handler.execute("create test", out);
  handler.execute("bind test x y 100", out);
  handler.execute("bind test x z 200", out);
  handler.execute("outbound test x", out);

  std::string expected = "y 100\nz 200\n";
  BOOST_CHECK_EQUAL(out.str(), expected);
}

BOOST_AUTO_TEST_CASE(MergeThenQuery)
{
  vishnyakov::CommandHandler handler;
  std::ostringstream out;

  handler.execute("create g1", out);
  handler.execute("bind g1 a b 10", out);
  handler.execute("bind g1 a c 20", out);

  handler.execute("create g2", out);
  handler.execute("bind g2 b c 30", out);
  handler.execute("bind g2 c a 40", out);

  handler.execute("merge merged g1 g2", out);

  out.str("");
  out.clear();

  handler.execute("vertexes merged", out);
  handler.execute("outbound merged a", out);
  handler.execute("inbound merged c", out);

  std::string result = out.str();

  BOOST_CHECK(result.find("a\nb\nc\n") != std::string::npos);

  BOOST_CHECK(result.find("b 10") != std::string::npos);
  BOOST_CHECK(result.find("c 20") != std::string::npos);

  BOOST_CHECK(result.find("a 20") != std::string::npos);
  BOOST_CHECK(result.find("b 30") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(ExtractThenQuery)
{
  vishnyakov::CommandHandler handler;
  std::ostringstream out;

  handler.execute("create original", out);
  handler.execute("bind original a b 10", out);
  handler.execute("bind original b c 20", out);
  handler.execute("bind original c d 30", out);
  handler.execute("bind original a d 40", out);

  handler.execute("extract extracted original 2 a d", out);
  handler.execute("vertexes extracted", out);
  handler.execute("outbound extracted a", out);

  std::string result = out.str();
  BOOST_CHECK(result.find("a\nd\n") != std::string::npos);
  BOOST_CHECK(result.find("d 40") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(InvalidCommandsThroughout)
{
  vishnyakov::CommandHandler handler;
  std::ostringstream out;

  handler.execute("unknown", out);
  handler.execute("vertexes nonexist", out);
  handler.execute("outbound missing v", out);
  handler.execute("bind missing a b 10", out);
  handler.execute("cut missing a b 10", out);
  handler.execute("create existing", out);
  handler.execute("create existing", out);
  handler.execute("merge new missing1 missing2", out);
  handler.execute("extract new missing 1 a", out);
  handler.execute("invalid command with spaces", out);

  int line_count = 0;
  for (char c : out.str())
  {
    if (c == '\n') line_count++;
  }

  BOOST_CHECK_EQUAL(line_count, 9);
}

BOOST_AUTO_TEST_CASE(CutAfterBind)
{
  vishnyakov::CommandHandler handler;
  std::ostringstream out;

  handler.execute("create graph", out);
  handler.execute("bind graph a b 100", out);
  handler.execute("bind graph a b 200", out);
  handler.execute("outbound graph a", out);

  out.str("");
  handler.execute("cut graph a b 100", out);
  handler.execute("outbound graph a", out);

  std::string result = out.str();
  BOOST_CHECK(result.find("b 200") != std::string::npos);
  BOOST_CHECK(result.find("b 100") == std::string::npos);
}

BOOST_AUTO_TEST_SUITE_END()

