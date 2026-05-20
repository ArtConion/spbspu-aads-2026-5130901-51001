#include <boost/test/unit_test.hpp>
#include "cuckoo_ht.hpp"
#include <string>

namespace vishnyakov
{
  BOOST_AUTO_TEST_SUITE(CuckooHashTableTests)

  BOOST_AUTO_TEST_CASE(ConstructorDefault)
  {
    CuckooHashTable< int, std::string > table;
    BOOST_TEST(table.empty());
    BOOST_TEST(table.size() == 0);
    BOOST_TEST(table.capacity() == 16);
  }

  BOOST_AUTO_TEST_CASE(ConstructorWithCapacity)
  {
    CuckooHashTable< int, std::string > table(32);
    BOOST_TEST(table.empty());
    BOOST_TEST(table.size() == 0);
    BOOST_TEST(table.capacity() == 32);
  }

  BOOST_AUTO_TEST_CASE(AddAndHas)
  {
    CuckooHashTable< int, std::string > table;
    table.add(1, "one");
    table.add(2, "two");
    table.add(3, "three");

    BOOST_TEST(table.size() == 3);
    BOOST_TEST(!table.empty());
    BOOST_TEST(table.has(1));
    BOOST_TEST(table.has(2));
    BOOST_TEST(table.has(3));
    BOOST_TEST(!table.has(4));
  }

  BOOST_AUTO_TEST_CASE(AddDuplicate)
  {
    CuckooHashTable< int, std::string > table;
    table.add(1, "one");
    table.add(1, "one_again");

    BOOST_TEST(table.size() == 1);
    BOOST_TEST(table.has(1));
  }

  BOOST_AUTO_TEST_CASE(At)
  {
    CuckooHashTable< int, std::string > table;
    table.add(1, "one");
    table.add(2, "two");

    BOOST_TEST(table.at(1) == "one");
    BOOST_TEST(table.at(2) == "two");
    BOOST_CHECK_THROW(table.at(3), std::out_of_range);
  }

  BOOST_AUTO_TEST_CASE(OperatorBrackets)
  {
    CuckooHashTable< int, std::string > table;
    table[1] = "one";
    table[2] = "two";

    BOOST_TEST(table[1] == "one");
    BOOST_TEST(table[2] == "two");
    BOOST_TEST(table.size() == 2);
    BOOST_TEST(table.has(3) == false);
    std::string& val = table[3];
    val = "three";
    BOOST_TEST(table[3] == "three");
    BOOST_TEST(table.size() == 3);
  }

  BOOST_AUTO_TEST_CASE(Drop)
  {
    CuckooHashTable< int, std::string > table;
    table.add(1, "one");
    table.add(2, "two");
    table.add(3, "three");

    std::string val = table.drop(2);
    BOOST_TEST(val == "two");
    BOOST_TEST(table.size() == 2);
    BOOST_TEST(!table.has(2));
    BOOST_TEST(table.has(1));
    BOOST_TEST(table.has(3));
  }

  BOOST_AUTO_TEST_CASE(DropNonExistent)
  {
    CuckooHashTable< int, std::string > table;
    table.add(1, "one");
    BOOST_CHECK_THROW(table.drop(2), std::out_of_range);
  }

  BOOST_AUTO_TEST_CASE(Clear)
  {
    CuckooHashTable< int, std::string > table;
    table.add(1, "one");
    table.add(2, "two");
    table.add(3, "three");

    BOOST_TEST(table.size() == 3);
    table.clear();
    BOOST_TEST(table.empty());
    BOOST_TEST(table.size() == 0);
    BOOST_TEST(!table.has(1));
  }

  BOOST_AUTO_TEST_CASE(Rehash)
  {
    CuckooHashTable< int, std::string > table(4);
    for (int i = 0; i < 10; ++i)
    {
      table.add(i, std::to_string(i));
    }
    BOOST_TEST(table.size() == 10);
    for (int i = 0; i < 10; ++i)
    {
      BOOST_TEST(table.has(i));
      BOOST_TEST(table.at(i) == std::to_string(i));
    }
  }

  BOOST_AUTO_TEST_CASE(CopyConstructor)
  {
    CuckooHashTable< int, std::string > table1;
    table1.add(1, "one");
    table1.add(2, "two");

    CuckooHashTable< int, std::string > table2(table1);
    BOOST_TEST(table2.size() == 2);
    BOOST_TEST(table2.has(1));
    BOOST_TEST(table2.has(2));
  }

  BOOST_AUTO_TEST_CASE(MoveConstructor)
  {
    CuckooHashTable< int, std::string > table1;
    table1.add(1, "one");
    table1.add(2, "two");

    CuckooHashTable< int, std::string > table2(std::move(table1));
    BOOST_TEST(table2.size() == 2);
    BOOST_TEST(table2.has(1));
    BOOST_TEST(table2.has(2));
    BOOST_TEST(table1.empty());
  }

  BOOST_AUTO_TEST_CASE(CopyAssignment)
  {
    CuckooHashTable< int, std::string > table1;
    table1.add(1, "one");
    table1.add(2, "two");

    CuckooHashTable< int, std::string > table2;
    table2 = table1;
    BOOST_TEST(table2.size() == 2);
    BOOST_TEST(table2.has(1));
    BOOST_TEST(table2.has(2));
  }

  BOOST_AUTO_TEST_CASE(MoveAssignment)
  {
    CuckooHashTable< int, std::string > table1;
    table1.add(1, "one");
    table1.add(2, "two");

    CuckooHashTable< int, std::string > table2;
    table2 = std::move(table1);
    BOOST_TEST(table2.size() == 2);
    BOOST_TEST(table2.has(1));
    BOOST_TEST(table2.has(2));
    BOOST_TEST(table1.empty());
  }

  BOOST_AUTO_TEST_CASE(StringKeys)
  {
    CuckooHashTable< std::string, int > table;
    table.add("one", 1);
    table.add("two", 2);
    table.add("three", 3);

    BOOST_TEST(table.size() == 3);
    BOOST_TEST(table.has("one"));
    BOOST_TEST(table.has("two"));
    BOOST_TEST(table.has("three"));
    BOOST_TEST(table.at("one") == 1);
    BOOST_TEST(table.at("two") == 2);
    BOOST_TEST(table.at("three") == 3);
  }

  BOOST_AUTO_TEST_SUITE_END()
}

