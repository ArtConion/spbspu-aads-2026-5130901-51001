#include <boost/test/unit_test.hpp>
#include <memory>
#include <string>
#include <list.hpp>
#include "stack.hpp"
#include "queue.hpp"

using namespace vishnyakov;

BOOST_AUTO_TEST_SUITE(emplace_tests)

struct TestStruct
{
  int a;
  double b;
  std::string c;

  TestStruct(int a_, double b_, const std::string& c_):
    a(a_),
    b(b_),
    c(c_)
  {}

  bool operator==(const TestStruct& other) const
  {
    return a == other.a && b == other.b && c == other.c;
  }
};

BOOST_AUTO_TEST_CASE(test_emplace_front)
{
  List< TestStruct > lst;
  lst.emplace_front(42, 3.14, "hello");

  BOOST_TEST(lst.size() == 1);
  BOOST_TEST(lst.front().a == 42);
  BOOST_TEST(lst.front().b == 3.14);
  BOOST_TEST(lst.front().c == "hello");
}

BOOST_AUTO_TEST_CASE(test_emplace_back)
{
  List< TestStruct > lst;
  lst.emplace_back(42, 3.14, "hello");
  lst.emplace_back(100, 2.71, "world");

  BOOST_TEST(lst.size() == 2);

  LIter< TestStruct > it = lst.begin();
  BOOST_REQUIRE(it != lst.end());
  BOOST_TEST(it->a == 42);
  BOOST_TEST(it->b == 3.14);
  BOOST_TEST(it->c == "hello");
  ++it;
  BOOST_REQUIRE(it != lst.end());
  BOOST_TEST(it->a == 100);
  BOOST_TEST(it->b == 2.71);
  BOOST_TEST(it->c == "world");
}

BOOST_AUTO_TEST_CASE(test_emplace_after)
{
  List< TestStruct > lst;
  lst.emplace_back(1, 1.1, "first");
  lst.emplace_back(3, 3.3, "third");

  LIter< TestStruct > pos = lst.begin();
  lst.emplace_after(pos, 2, 2.2, "second");

  BOOST_TEST(lst.size() == 3);

  LIter< TestStruct > it = lst.begin();
  BOOST_REQUIRE(it != lst.end());
  BOOST_TEST(it->a == 1);
  ++it;
  BOOST_REQUIRE(it != lst.end());
  BOOST_TEST(it->a == 2);
  ++it;
  BOOST_REQUIRE(it != lst.end());
  BOOST_TEST(it->a == 3);
}

BOOST_AUTO_TEST_CASE(test_emplace_move_only)
{
  List< std::unique_ptr< int > > lst;
  lst.emplace_front(std::make_unique< int >(42));

  BOOST_TEST(lst.size() == 1);
  BOOST_TEST(*lst.front() == 42);
}

BOOST_AUTO_TEST_SUITE_END()

