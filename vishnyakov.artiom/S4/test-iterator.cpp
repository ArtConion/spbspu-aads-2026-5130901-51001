#include <boost/test/unit_test.hpp>
#include "bstree.hpp"
#include <string>

BOOST_AUTO_TEST_SUITE(BSTreeIteratorTests)

BOOST_AUTO_TEST_CASE(BeginEndEmptyTree)
{
  vishnyakov::BSTree< int, std::string, std::less< int > > tree;
  BOOST_CHECK(tree.begin() == tree.end());
}

BOOST_AUTO_TEST_CASE(IteratorInOrderTraversal)
{
  vishnyakov::BSTree< int, std::string, std::less< int > > tree;
  tree.push(5, "five");
  tree.push(3, "three");
  tree.push(7, "seven");
  tree.push(2, "two");
  tree.push(4, "four");
  tree.push(6, "six");
  tree.push(8, "eight");

  std::vector< int > expected = {2, 3, 4, 5, 6, 7, 8};
  std::vector< int > actual;

  for (auto it = tree.begin(); it != tree.end(); ++it)
  {
    actual.push_back(it->first);
  }

  BOOST_CHECK_EQUAL_COLLECTIONS(actual.begin(), actual.end(),
                                expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(IteratorDereference)
{
  vishnyakov::BSTree< int, std::string, std::less< int > > tree;
  tree.push(42, "answer");

  auto it = tree.begin();
  BOOST_CHECK_EQUAL(it->first, 42);
  BOOST_CHECK_EQUAL(it->second, "answer");
  BOOST_CHECK_EQUAL((*it).second, "answer");
}

BOOST_AUTO_TEST_CASE(IteratorPrefixIncrement)
{
  vishnyakov::BSTree< int, std::string, std::less< int > > tree;
  tree.push(1, "one");
  tree.push(2, "two");

  auto it = tree.begin();
  auto& ref = ++it;
  BOOST_CHECK(it == ref);
}

BOOST_AUTO_TEST_CASE(IteratorPostfixIncrement)
{
  vishnyakov::BSTree< int, std::string, std::less< int > > tree;
  tree.push(1, "one");

  auto it = tree.begin();
  auto old = it++;
  BOOST_CHECK(old != it);
  BOOST_CHECK(it == tree.end());
}

BOOST_AUTO_TEST_CASE(ConstIterator)
{
  vishnyakov::BSTree< int, std::string, std::less< int > > tree;
  tree.push(1, "one");
  tree.push(2, "two");

  const auto& const_tree = tree;
  std::vector< int > actual;

  for (auto it = const_tree.begin(); it != const_tree.end(); ++it)
  {
    actual.push_back(it->first);
  }

  std::vector< int > expected = {1, 2};
  BOOST_CHECK_EQUAL_COLLECTIONS(actual.begin(), actual.end(),
                                expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(RangeBasedForLoop)
{
  vishnyakov::BSTree< int, std::string, std::less< int > > tree;
  tree.push(3, "three");
  tree.push(1, "one");
  tree.push(2, "two");

  std::vector< int > actual;
  for (const auto& pair : tree)
  {
    actual.push_back(pair.first);
  }

  std::vector< int > expected = {1, 2, 3};
  BOOST_CHECK_EQUAL_COLLECTIONS(actual.begin(), actual.end(),
                                expected.begin(), expected.end());
}

BOOST_AUTO_TEST_SUITE_END()

