#include <boost/test/unit_test.hpp>
#include "multimap.hpp"
#include <string>

BOOST_AUTO_TEST_SUITE(MultimapTests)

BOOST_AUTO_TEST_CASE(MultimapInsertAndCount)
{
  vishnyakov::Multimap< int, std::string > multimap;

  multimap.insert(1, "one_a");
  multimap.insert(1, "one_b");
  multimap.insert(2, "two_a");
  multimap.insert(2, "two_b");
  multimap.insert(2, "two_c");
  multimap.insert(3, "three");

  BOOST_CHECK_EQUAL(multimap.size(), 3);
  BOOST_CHECK_EQUAL(multimap.count(1), 2);
  BOOST_CHECK_EQUAL(multimap.count(2), 3);
  BOOST_CHECK_EQUAL(multimap.count(3), 1);
  BOOST_CHECK_EQUAL(multimap.count(4), 0);
}

BOOST_AUTO_TEST_CASE(MultimapEraseOne)
{
  vishnyakov::Multimap< int, std::string > multimap;

  multimap.insert(1, "one_a");
  multimap.insert(1, "one_b");
  multimap.insert(1, "one_c");
  multimap.insert(2, "two");

  multimap.erase_one(1);

  BOOST_CHECK_EQUAL(multimap.count(1), 2);
  BOOST_CHECK_EQUAL(multimap.count(2), 1);
  BOOST_CHECK_EQUAL(multimap.size(), 2);
}

BOOST_AUTO_TEST_CASE(MultimapEraseAll)
{
  vishnyakov::Multimap< int, std::string > multimap;

  multimap.insert(1, "one_a");
  multimap.insert(1, "one_b");
  multimap.insert(1, "one_c");
  multimap.insert(2, "two");

  multimap.erase_all(1);

  BOOST_CHECK_EQUAL(multimap.count(1), 0);
  BOOST_CHECK(!multimap.has(1));
  BOOST_CHECK_EQUAL(multimap.count(2), 1);
  BOOST_CHECK_EQUAL(multimap.size(), 1);
}

BOOST_AUTO_TEST_CASE(MultimapIterator)
{
  vishnyakov::Multimap< int, std::string > multimap;

  multimap.insert(3, "three");
  multimap.insert(1, "one");
  multimap.insert(1, "one_again");
  multimap.insert(2, "two");

  std::vector< int > keys;
  for (auto it = multimap.begin(); it != multimap.end(); ++it)
  {
    keys.push_back(it->first);
  }

  std::vector< int > expected = {1, 2, 3};
  BOOST_CHECK_EQUAL_COLLECTIONS(keys.begin(), keys.end(),
                                expected.begin(), expected.end());
}

BOOST_AUTO_TEST_SUITE_END()

