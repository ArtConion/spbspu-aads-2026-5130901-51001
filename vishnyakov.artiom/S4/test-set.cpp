#include <boost/test/unit_test.hpp>
#include "set.hpp"

BOOST_AUTO_TEST_SUITE(SetTests)

BOOST_AUTO_TEST_CASE(SetInsertAndHas)
{
  vishnyakov::Set< int > set;

  set.insert(1);
  set.insert(2);
  set.insert(3);

  BOOST_CHECK_EQUAL(set.size(), 3);
  BOOST_CHECK(set.has(1));
  BOOST_CHECK(set.has(2));
  BOOST_CHECK(set.has(3));
  BOOST_CHECK(!set.has(4));
}

BOOST_AUTO_TEST_CASE(SetDuplicateInsert)
{
  vishnyakov::Set< int > set;

  set.insert(1);
  set.insert(1);
  set.insert(2);

  BOOST_CHECK_EQUAL(set.size(), 2);
  BOOST_CHECK(set.has(1));
  BOOST_CHECK(set.has(2));
}

BOOST_AUTO_TEST_CASE(SetErase)
{
  vishnyakov::Set< int > set;

  set.insert(1);
  set.insert(2);
  set.insert(3);

  set.erase(2);

  BOOST_CHECK_EQUAL(set.size(), 2);
  BOOST_CHECK(!set.has(2));
  BOOST_CHECK(set.has(1));
  BOOST_CHECK(set.has(3));
}

BOOST_AUTO_TEST_CASE(SetIterator)
{
  vishnyakov::Set< int > set;

  set.insert(3);
  set.insert(1);
  set.insert(2);

  std::vector< int > values;
  for (auto it = set.begin(); it != set.end(); ++it)
  {
    values.push_back(it->first);
  }

  std::vector< int > expected = {1, 2, 3};
  BOOST_CHECK_EQUAL_COLLECTIONS(values.begin(), values.end(),
                                expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(SetClear)
{
  vishnyakov::Set< int > set;

  set.insert(1);
  set.insert(2);

  set.clear();

  BOOST_CHECK(set.empty());
  BOOST_CHECK_EQUAL(set.size(), 0);
  BOOST_CHECK(!set.has(1));
  BOOST_CHECK(!set.has(2));
}

BOOST_AUTO_TEST_SUITE_END()

