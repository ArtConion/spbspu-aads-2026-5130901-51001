#include <boost/test/unit_test.hpp>
#include "utils.hpp"
#include <sstream>
#include <string>

BOOST_AUTO_TEST_SUITE(DictionaryTests)

BOOST_AUTO_TEST_CASE(ParseValidFile)
{
  std::string content = "first 1 name 2 surname\nsecond 4 mouse 1 name 2 keyboard\n";
  std::istringstream file(content);
  vishnyakov::BSTree< std::string, vishnyakov::Dictionary, std::less< std::string > > dicts;

  vishnyakov::parse_dict_file(file, dicts);

  BOOST_CHECK(dicts.has("first"));
  BOOST_CHECK(dicts.has("second"));
  BOOST_CHECK_EQUAL(dicts.size(), 2);

  const auto& first = dicts.at("first");
  BOOST_CHECK(first.has(1));
  BOOST_CHECK(first.has(2));
  BOOST_CHECK_EQUAL(first.at(1), "name");
  BOOST_CHECK_EQUAL(first.at(2), "surname");

  const auto& second = dicts.at("second");
  BOOST_CHECK(second.has(4));
  BOOST_CHECK(second.has(1));
  BOOST_CHECK(second.has(2));
  BOOST_CHECK_EQUAL(second.at(4), "mouse");
  BOOST_CHECK_EQUAL(second.at(1), "name");
  BOOST_CHECK_EQUAL(second.at(2), "keyboard");
}

BOOST_AUTO_TEST_CASE(ParseFileWithEmptyLines)
{
  std::string content = "\n\nfirst 1 name\n\nsecond 2 value\n\n";
  std::istringstream file(content);
  vishnyakov::BSTree< std::string, vishnyakov::Dictionary, std::less< std::string > > dicts;

  vishnyakov::parse_dict_file(file, dicts);

  BOOST_CHECK(dicts.has("first"));
  BOOST_CHECK(dicts.has("second"));
  BOOST_CHECK_EQUAL(dicts.size(), 2);
}

BOOST_AUTO_TEST_CASE(ParseFileDuplicateDictNames)
{
  std::string content = "dup 1 a\ndup 2 b\n";
  std::istringstream file(content);
  vishnyakov::BSTree< std::string, vishnyakov::Dictionary, std::less< std::string > > dicts;

  BOOST_CHECK_THROW(vishnyakov::parse_dict_file(file, dicts), std::runtime_error);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(PrintCommandTests)

BOOST_AUTO_TEST_CASE(PrintNonExistingDict)
{
  vishnyakov::BSTree< std::string, vishnyakov::Dictionary, std::less< std::string > > dicts;
  std::istringstream in("print nonexist\n");
  std::ostringstream out;

  vishnyakov::process_commands(in, dicts, out);

  std::string expected = "<INVALID COMMAND>\n";
  BOOST_CHECK_EQUAL(out.str(), expected);
}

BOOST_AUTO_TEST_CASE(PrintEmptyDict)
{
  vishnyakov::BSTree< std::string, vishnyakov::Dictionary, std::less< std::string > > dicts;
  vishnyakov::Dictionary dict;
  dicts.push("empty", dict);

  std::istringstream in("print empty\n");
  std::ostringstream out;

  vishnyakov::process_commands(in, dicts, out);

  std::string expected = "<EMPTY>\n";
  BOOST_CHECK_EQUAL(out.str(), expected);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(CommandTests)

BOOST_AUTO_TEST_CASE(ComplementCommand)
{
  vishnyakov::BSTree< std::string, vishnyakov::Dictionary, std::less< std::string > > dicts;
  vishnyakov::Dictionary d1, d2;

  d1.push(1, "one");
  d1.push(2, "two");
  d1.push(3, "three");

  d2.push(2, "two");
  d2.push(4, "four");

  dicts.push("first", d1);
  dicts.push("second", d2);

  std::istringstream in("complement third first second\n");
  std::ostringstream out;

  vishnyakov::process_commands(in, dicts, out);

  BOOST_CHECK(dicts.has("third"));
  const auto& third = dicts.at("third");
  BOOST_CHECK(third.has(1));
  BOOST_CHECK(third.has(3));
  BOOST_CHECK(!third.has(2));
  BOOST_CHECK(!third.has(4));
  BOOST_CHECK_EQUAL(third.size(), 2);
}

BOOST_AUTO_TEST_CASE(IntersectCommand)
{
  vishnyakov::BSTree< std::string, vishnyakov::Dictionary, std::less< std::string > > dicts;
  vishnyakov::Dictionary d1, d2;

  d1.push(1, "one");
  d1.push(2, "two");
  d1.push(3, "three");

  d2.push(2, "two");
  d2.push(3, "three");
  d2.push(4, "four");

  dicts.push("first", d1);
  dicts.push("second", d2);

  std::istringstream in("intersect third first second\n");
  std::ostringstream out;

  vishnyakov::process_commands(in, dicts, out);

  BOOST_CHECK(dicts.has("third"));
  const auto& third = dicts.at("third");
  BOOST_CHECK(third.has(2));
  BOOST_CHECK(third.has(3));
  BOOST_CHECK(!third.has(1));
  BOOST_CHECK(!third.has(4));
  BOOST_CHECK_EQUAL(third.size(), 2);
}

BOOST_AUTO_TEST_CASE(UnionCommand)
{
  vishnyakov::BSTree< std::string, vishnyakov::Dictionary, std::less< std::string > > dicts;
  vishnyakov::Dictionary d1, d2;

  d1.push(1, "one");
  d1.push(2, "two");

  d2.push(2, "two");
  d2.push(3, "three");
  d2.push(4, "four");

  dicts.push("first", d1);
  dicts.push("second", d2);

  std::istringstream in("union third first second\n");
  std::ostringstream out;

  vishnyakov::process_commands(in, dicts, out);

  BOOST_CHECK(dicts.has("third"));
  const auto& third = dicts.at("third");
  BOOST_CHECK(third.has(1));
  BOOST_CHECK(third.has(2));
  BOOST_CHECK(third.has(3));
  BOOST_CHECK(third.has(4));
  BOOST_CHECK_EQUAL(third.size(), 4);
  BOOST_CHECK_EQUAL(third.at(2), "two");
}

BOOST_AUTO_TEST_CASE(ComplementExistingNewName)
{
  vishnyakov::BSTree< std::string, vishnyakov::Dictionary, std::less< std::string > > dicts;
  vishnyakov::Dictionary d1, d2;

  d1.push(1, "one");
  d2.push(2, "two");

  dicts.push("first", d1);
  dicts.push("second", d2);
  dicts.push("existing", d1);

  std::istringstream in("complement existing first second\n");
  std::ostringstream out;

  vishnyakov::process_commands(in, dicts, out);

  BOOST_CHECK(out.str().empty());

  const auto& existing = dicts.at("existing");
  BOOST_CHECK(existing.has(1));
  BOOST_CHECK(!existing.has(2));
  BOOST_CHECK_EQUAL(existing.size(), 1);
  BOOST_CHECK_EQUAL(existing.at(1), "one");
}

BOOST_AUTO_TEST_CASE(ComplementMissingDict)
{
  vishnyakov::BSTree< std::string, vishnyakov::Dictionary, std::less< std::string > > dicts;
  vishnyakov::Dictionary d1;
  d1.push(1, "one");
  dicts.push("first", d1);

  std::istringstream in("complement third first missing\n");
  std::ostringstream out;

  vishnyakov::process_commands(in, dicts, out);

  std::string expected = "<INVALID COMMAND>\n";
  BOOST_CHECK_EQUAL(out.str(), expected);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(DuplicateKeyTests)

BOOST_AUTO_TEST_CASE(IntersectLeftPriority)
{
  vishnyakov::BSTree< std::string, vishnyakov::Dictionary, std::less< std::string > > dicts;
  vishnyakov::Dictionary d1, d2;

  d1.push(1, "left_one");
  d1.push(2, "left_two");

  d2.push(1, "right_one");
  d2.push(3, "right_three");

  dicts.push("left", d1);
  dicts.push("right", d2);

  std::istringstream in("intersect result left right\n");
  std::ostringstream out;

  vishnyakov::process_commands(in, dicts, out);

  const auto& result = dicts.at("result");
  BOOST_CHECK(result.has(1));
  BOOST_CHECK_EQUAL(result.at(1), "left_one");
  BOOST_CHECK(!result.has(2));
  BOOST_CHECK(!result.has(3));
}

BOOST_AUTO_TEST_CASE(UnionLeftPriority)
{
  vishnyakov::BSTree< std::string, vishnyakov::Dictionary, std::less< std::string > > dicts;
  vishnyakov::Dictionary d1, d2;

  d1.push(1, "left_one");
  d1.push(2, "left_two");

  d2.push(1, "right_one");
  d2.push(3, "right_three");

  dicts.push("left", d1);
  dicts.push("right", d2);

  std::istringstream in("union result left right\n");
  std::ostringstream out;

  vishnyakov::process_commands(in, dicts, out);

  const auto& result = dicts.at("result");
  BOOST_CHECK(result.has(1));
  BOOST_CHECK_EQUAL(result.at(1), "left_one");
  BOOST_CHECK(result.has(2));
  BOOST_CHECK_EQUAL(result.at(2), "left_two");
  BOOST_CHECK(result.has(3));
  BOOST_CHECK_EQUAL(result.at(3), "right_three");
  BOOST_CHECK_EQUAL(result.size(), 3);
}

BOOST_AUTO_TEST_SUITE_END()

