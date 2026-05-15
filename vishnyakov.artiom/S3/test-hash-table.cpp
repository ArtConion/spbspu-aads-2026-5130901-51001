#define BOOST_TEST_MODULE S3
#include <boost/test/unit_test.hpp>
#include "hash_table.hpp"
#include "siphash.hpp"
#include <string>

BOOST_AUTO_TEST_SUITE(HashTableConstructorTests)

BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
  vishnyakov::HashTable< int, std::string, vishnyakov::SipHash, std::equal_to< int > > table;
  BOOST_CHECK(table.empty());
  BOOST_CHECK_EQUAL(table.size(), 0);
  BOOST_CHECK_EQUAL(table.capacity(), 16);
}

BOOST_AUTO_TEST_CASE(CustomCapacityConstructor)
{
  vishnyakov::HashTable< int, std::string, vishnyakov::SipHash, std::equal_to< int > > table(32);
  BOOST_CHECK(table.empty());
  BOOST_CHECK_EQUAL(table.size(), 0);
  BOOST_CHECK_EQUAL(table.capacity(), 32);
}

BOOST_AUTO_TEST_CASE(CopyConstructor)
{
  vishnyakov::HashTable< int, std::string, vishnyakov::SipHash, std::equal_to< int > > table1;
  table1.add(1, "one");
  table1.add(2, "two");

  vishnyakov::HashTable< int, std::string, vishnyakov::SipHash, std::equal_to< int > > table2(table1);
  BOOST_CHECK_EQUAL(table2.size(), 2);
  BOOST_CHECK(table2.has(1));
  BOOST_CHECK(table2.has(2));
}

BOOST_AUTO_TEST_CASE(MoveConstructor)
{
  vishnyakov::HashTable< int, std::string, vishnyakov::SipHash, std::equal_to< int > > table1;
  table1.add(1, "one");

  vishnyakov::HashTable< int, std::string, vishnyakov::SipHash, std::equal_to< int > > table2(std::move(table1));
  BOOST_CHECK_EQUAL(table2.size(), 1);
  BOOST_CHECK(table2.has(1));
  BOOST_CHECK(table1.empty());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(HashTableAddTests)

BOOST_AUTO_TEST_CASE(AddSingleElement)
{
  vishnyakov::HashTable< int, std::string, vishnyakov::SipHash, std::equal_to< int > > table;
  table.add(10, "ten");

  BOOST_CHECK_EQUAL(table.size(), 1);
  BOOST_CHECK(table.has(10));
}

BOOST_AUTO_TEST_CASE(AddMultipleElements)
{
  vishnyakov::HashTable< int, std::string, vishnyakov::SipHash, std::equal_to< int > > table;
  table.add(1, "one");
  table.add(2, "two");
  table.add(3, "three");

  BOOST_CHECK_EQUAL(table.size(), 3);
  BOOST_CHECK(table.has(1));
  BOOST_CHECK(table.has(2));
  BOOST_CHECK(table.has(3));
}

BOOST_AUTO_TEST_CASE(AddDuplicateKey)
{
  vishnyakov::HashTable< int, std::string, vishnyakov::SipHash, std::equal_to< int > > table;
  table.add(1, "one");
  table.add(1, "uno");

  BOOST_CHECK_EQUAL(table.size(), 1);
  BOOST_CHECK(table.has(1));
}

BOOST_AUTO_TEST_CASE(AddMoveSemantics)
{
  vishnyakov::HashTable< int, std::string, vishnyakov::SipHash, std::equal_to< int > > table;
  std::string value = "move";
  table.add(5, std::move(value));

  BOOST_CHECK(table.has(5));
  BOOST_CHECK(value.empty());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(HashTableHasTests)

BOOST_AUTO_TEST_CASE(HasExistingKey)
{
  vishnyakov::HashTable< int, std::string, vishnyakov::SipHash, std::equal_to< int > > table;
  table.add(42, "answer");
  BOOST_CHECK(table.has(42));
}

BOOST_AUTO_TEST_CASE(HasNonExistingKey)
{
  vishnyakov::HashTable< int, std::string, vishnyakov::SipHash, std::equal_to< int > > table;
  table.add(42, "answer");
  BOOST_CHECK(!table.has(100));
}

BOOST_AUTO_TEST_CASE(HasOnEmptyTable)
{
  vishnyakov::HashTable< int, std::string, vishnyakov::SipHash, std::equal_to< int > > table;
  BOOST_CHECK(!table.has(0));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(HashTableDropTests)

BOOST_AUTO_TEST_CASE(DropExistingKey)
{
  vishnyakov::HashTable< int, std::string, vishnyakov::SipHash, std::equal_to< int > > table;
  table.add(1, "one");
  table.add(2, "two");

  std::string value = table.drop(1);

  BOOST_CHECK_EQUAL(value, "one");
  BOOST_CHECK_EQUAL(table.size(), 1);
  BOOST_CHECK(!table.has(1));
  BOOST_CHECK(table.has(2));
}

BOOST_AUTO_TEST_CASE(DropNonExistingKey)
{
  vishnyakov::HashTable< int, std::string, vishnyakov::SipHash, std::equal_to< int > > table;
  table.add(1, "one");

  BOOST_CHECK_THROW(table.drop(999), std::out_of_range);
  BOOST_CHECK_EQUAL(table.size(), 1);
}

BOOST_AUTO_TEST_CASE(DropFromEmptyTable)
{
  vishnyakov::HashTable< int, std::string, vishnyakov::SipHash, std::equal_to< int > > table;
  BOOST_CHECK_THROW(table.drop(0), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(DropSingleElement)
{
  vishnyakov::HashTable< int, std::string, vishnyakov::SipHash, std::equal_to< int > > table;
  table.add(42, "answer");

  std::string value = table.drop(42);

  BOOST_CHECK_EQUAL(value, "answer");
  BOOST_CHECK(table.empty());
  BOOST_CHECK_EQUAL(table.size(), 0);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(HashTableClearTests)

BOOST_AUTO_TEST_CASE(ClearEmptyTable)
{
  vishnyakov::HashTable< int, std::string, vishnyakov::SipHash, std::equal_to< int > > table;
  table.clear();
  BOOST_CHECK(table.empty());
  BOOST_CHECK_EQUAL(table.size(), 0);
}

BOOST_AUTO_TEST_CASE(ClearNonEmptyTable)
{
  vishnyakov::HashTable< int, std::string, vishnyakov::SipHash, std::equal_to< int > > table;
  table.add(1, "one");
  table.add(2, "two");
  table.add(3, "three");

  table.clear();

  BOOST_CHECK(table.empty());
  BOOST_CHECK_EQUAL(table.size(), 0);
  BOOST_CHECK(!table.has(1));
  BOOST_CHECK(!table.has(2));
  BOOST_CHECK(!table.has(3));
}

BOOST_AUTO_TEST_CASE(ClearThenAdd)
{
  vishnyakov::HashTable< int, std::string, vishnyakov::SipHash, std::equal_to< int > > table;
  table.add(1, "one");
  table.clear();
  table.add(2, "two");

  BOOST_CHECK_EQUAL(table.size(), 1);
  BOOST_CHECK(!table.has(1));
  BOOST_CHECK(table.has(2));
}

BOOST_AUTO_TEST_SUITE_END()

