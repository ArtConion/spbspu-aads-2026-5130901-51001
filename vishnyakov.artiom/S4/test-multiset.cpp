#include <boost/test/unit_test.hpp>
#include "multiset.hpp"

BOOST_AUTO_TEST_SUITE(MultisetTests)

BOOST_AUTO_TEST_CASE(MultisetInsertAndCount)
{
  vishnyakov::Multiset< int > multiset;

  multiset.insert(1);
  multiset.insert(1);
  multiset.insert(2);
  multiset.insert(2);
  multiset.insert(2);
  multiset.insert(3);

  BOOST_CHECK_EQUAL(multiset.size(), 3);
  BOOST_CHECK_EQUAL(multiset.count(1), 2);
  BOOST_CHECK_EQUAL(multiset.count(2), 3);
  BOOST_CHECK_EQUAL(multiset.count(3), 1);
  BOOST_CHECK_EQUAL(multiset.count(4), 0);
}

BOOST_AUTO_TEST_CASE(MultisetEraseOne)
{
  vishnyakov::Multiset< int > multiset;

  multiset.insert(1);
  multiset.insert(1);
  multiset.insert(1);
  multiset.insert(2);

  multiset.erase_one(1);

  BOOST_CHECK_EQUAL(multiset.count(1), 2);
  BOOST_CHECK_EQUAL(multiset.count(2), 1);
  BOOST_CHECK_EQUAL(multiset.size(), 2);

  multiset.erase_one(1);
  multiset.erase_one(1);

  BOOST_CHECK_EQUAL(multiset.count(1), 0);
  BOOST_CHECK(!multiset.has(1));
  BOOST_CHECK_EQUAL(multiset.size(), 1);
}

BOOST_AUTO_TEST_CASE(MultisetEraseAll)
{
  vishnyakov::Multiset< int > multiset;

  multiset.insert(1);
  multiset.insert(1);
  multiset.insert(1);
  multiset.insert(2);

  multiset.erase_all(1);

  BOOST_CHECK_EQUAL(multiset.count(1), 0);
  BOOST_CHECK(!multiset.has(1));
  BOOST_CHECK_EQUAL(multiset.count(2), 1);
  BOOST_CHECK_EQUAL(multiset.size(), 1);
}

BOOST_AUTO_TEST_CASE(MultisetIterator)
{
  vishnyakov::Multiset< int > multiset;

  multiset.insert(3);
  multiset.insert(1);
  multiset.insert(1);
  multiset.insert(2);

  std::vector< int > values;
  for (auto it = multiset.begin(); it != multiset.end(); ++it)
  {
    values.push_back(it->first);
  }

  std::vector< int > expected = {1, 2, 3};
  BOOST_CHECK_EQUAL_COLLECTIONS(values.begin(), values.end(),
                                expected.begin(), expected.end());
}

BOOST_AUTO_TEST_SUITE_END()

