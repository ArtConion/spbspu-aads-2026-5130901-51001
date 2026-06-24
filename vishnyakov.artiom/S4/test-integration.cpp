// lab4/test-integration.cpp
#include <boost/test/unit_test.hpp>
#include "utils.hpp"
#include <sstream>
#include <string>

BOOST_AUTO_TEST_SUITE(IntegrationTests)

BOOST_AUTO_TEST_CASE(FullWorkflow)
{
  std::string file_content = "first 1 name 2 surname\nsecond 4 mouse 1 name 2 keyboard\n";
  std::istringstream file(file_content);
  vishnyakov::BSTree< std::string, vishnyakov::Dictionary, std::less< std::string > > dicts;

  vishnyakov::parse_dict_file(file, dicts);

  std::istringstream commands(
    "print first\n"
    "print second\n"
    "complement third second first\n"
    "print third\n"
    "intersect fourth second first\n"
    "print fourth\n"
    "union fifth second first\n"
    "print fifth\n"
  );
  std::ostringstream out;

  vishnyakov::process_commands(commands, dicts, out);

  std::string expected =
    "first 1 name 2 surname\n"
    "second 1 name 2 keyboard 4 mouse\n"
    "third 4 mouse\n"
    "fourth 1 name 2 keyboard\n"
    "fifth 1 name 2 keyboard 4 mouse\n";

  BOOST_CHECK_EQUAL(out.str(), expected);
}

BOOST_AUTO_TEST_CASE(EmptyDictWorkflow)
{
  std::string file_content = "empty \n";
  std::istringstream file(file_content);
  vishnyakov::BSTree< std::string, vishnyakov::Dictionary, std::less< std::string > > dicts;

  vishnyakov::parse_dict_file(file, dicts);

  std::istringstream commands(
    "print empty\n"
    "complement new empty empty\n"
    "print new\n"
  );
  std::ostringstream out;

  vishnyakov::process_commands(commands, dicts, out);

  std::string expected = "<EMPTY>\n<EMPTY>\n";
  BOOST_CHECK_EQUAL(out.str(), expected);
}

BOOST_AUTO_TEST_CASE(InvalidCommandsWorkflow)
{
  std::string file_content = "test 1 value\n";
  std::istringstream file(file_content);
  vishnyakov::BSTree< std::string, vishnyakov::Dictionary, std::less< std::string > > dicts;

  vishnyakov::parse_dict_file(file, dicts);

  std::istringstream commands(
    "print unknown\n"
    "complement new unknown test\n"
    "intersect new test unknown\n"
    "union new test unknown\n"
    "unknowncmd\n"
  );
  std::ostringstream out;

  vishnyakov::process_commands(commands, dicts, out);

  std::string expected =
    "<INVALID COMMAND>\n"
    "<INVALID COMMAND>\n"
    "<INVALID COMMAND>\n"
    "<INVALID COMMAND>\n"
    "<INVALID COMMAND>\n";

  BOOST_CHECK_EQUAL(out.str(), expected);
}

BOOST_AUTO_TEST_SUITE_END()

