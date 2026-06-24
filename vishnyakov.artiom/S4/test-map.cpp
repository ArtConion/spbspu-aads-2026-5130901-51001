#include <boost/test/unit_test.hpp>
#include "map.hpp"
#include <string>

BOOST_AUTO_TEST_SUITE(MapTests)

BOOST_AUTO_TEST_CASE(MapInsertAndAccess)
{
  vishnyakov::Map< int, std::string > map;

  map.insert(1, "one");
  map.insert(2, "two");
  map.insert(3, "three");

  BOOST_CHECK_EQUAL(map.size(), 3);
  BOOST_CHECK(map.has(1));
  BOOST_CHECK(map.has(2));
  BOOST_CHECK(map.has(3));
  BOOST_CHECK_EQUAL(map.at(1), "one");
  BOOST_CHECK_EQUAL(map.at(2), "two");
  BOOST_CHECK_EQUAL(map.at(3), "three");
}

BOOST_AUTO_TEST_CASE(MapSubscript)
{
  vishnyakov::Map< int, std::string > map;

  map[1] = "one";
  map[2] = "two";

  BOOST_CHECK_EQUAL(map[1], "one");
  BOOST_CHECK_EQUAL(map[2], "two");
  BOOST_CHECK_EQUAL(map.size(), 2);
}

BOOST_AUTO_TEST_CASE(MapDrop)
{
  vishnyakov::Map< int, std::string > map;

  map.insert(1, "one");
  map.insert(2, "two");
  map.insert(3, "three");

  std::string removed = map.drop(2);

  BOOST_CHECK_EQUAL(removed, "two");
  BOOST_CHECK_EQUAL(map.size(), 2);
  BOOST_CHECK(!map.has(2));
  BOOST_CHECK(map.has(1));
  BOOST_CHECK(map.has(3));
}

BOOST_AUTO_TEST_CASE(MapIterator)
{
  vishnyakov::Map< int, std::string > map;

  map.insert(3, "three");
  map.insert(1, "one");
  map.insert(2, "two");

  std::vector< int > keys;
  for (auto it = map.begin(); it != map.end(); ++it)
  {
    keys.push_back(it->first);
  }

  std::vector< int > expected = {1, 2, 3};
  BOOST_CHECK_EQUAL_COLLECTIONS(keys.begin(), keys.end(),
                                expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(MapClear)
{
  vishnyakov::Map< int, std::string > map;

  map.insert(1, "one");
  map.insert(2, "two");

  map.clear();

  BOOST_CHECK(map.empty());
  BOOST_CHECK_EQUAL(map.size(), 0);
  BOOST_CHECK(!map.has(1));
  BOOST_CHECK(!map.has(2));
}

BOOST_AUTO_TEST_SUITE_END()

