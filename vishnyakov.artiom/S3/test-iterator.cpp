#define BOOST_TEST_MODULE S3
#include <boost/test/unit_test.hpp>
#include "hash_table.hpp"
#include "siphash.hpp"
#include <string>

BOOST_AUTO_TEST_SUITE(IteratorTests)

BOOST_AUTO_TEST_CASE(IteratorBeginEndEmpty)
{
  vishnyakov::HashTable< int, std::string, vishnyakov::SipHash, std::equal_to< int > > table;
  BOOST_CHECK(table.begin() == table.end());
}

BOOST_AUTO_TEST_CASE(IteratorIterateOverElements)
{
  vishnyakov::HashTable< int, std::string, vishnyakov::SipHash, std::equal_to< int > > table;
  table.add(1, "one");
  table.add(2, "two");
  table.add(3, "three");

  int count = 0;
  for (auto it = table.begin(); it != table.end(); ++it)
  {
    ++count;
  }

  BOOST_CHECK_EQUAL(count, 3);
}

BOOST_AUTO_TEST_CASE(IteratorDereference)
{
  vishnyakov::HashTable< int, std::string, vishnyakov::SipHash, std::equal_to< int > > table;
  table.add(42, "answer");

  auto it = table.begin();
  BOOST_CHECK_EQUAL(it->first, 42);
  BOOST_CHECK_EQUAL(it->second, "answer");
  BOOST_CHECK_EQUAL((*it).second, "answer");
}

BOOST_AUTO_TEST_CASE(IteratorPrefixIncrement)
{
  vishnyakov::HashTable< int, std::string, vishnyakov::SipHash, std::equal_to< int > > table;
  table.add(1, "one");
  table.add(2, "two");

  auto it = table.begin();
  auto& ref = ++it;
  BOOST_CHECK(it == ref);
}

BOOST_AUTO_TEST_CASE(IteratorPostfixIncrement)
{
  vishnyakov::HashTable< int, std::string, vishnyakov::SipHash, std::equal_to< int > > table;
  table.add(1, "one");

  auto it = table.begin();
  auto old = it++;
  BOOST_CHECK(old != it);
  BOOST_CHECK(it == table.end());
}

BOOST_AUTO_TEST_SUITE_END()

