#include <boost/test/unit_test.hpp>
#include "bstree.hpp"
#include <string>
#include <vector>

BOOST_AUTO_TEST_SUITE(BSTreeRotationTests)

BOOST_AUTO_TEST_CASE(RotateLeftSimple)
{
  vishnyakov::BSTree< int, std::string, std::less< int > > tree;
  tree.push(1, "one");
  tree.push(2, "two");
  tree.push(3, "three");

  auto it = tree.find(2);
  tree.rotate_left(it);

  BOOST_CHECK(tree.has(1));
  BOOST_CHECK(tree.has(2));
  BOOST_CHECK(tree.has(3));
  BOOST_CHECK_EQUAL(tree.size(), 3);
}

BOOST_AUTO_TEST_CASE(RotateRightSimple)
{
  vishnyakov::BSTree< int, std::string, std::less< int > > tree;
  tree.push(3, "three");
  tree.push(2, "two");
  tree.push(1, "one");

  auto it = tree.find(2);
  tree.rotate_right(it);

  BOOST_CHECK(tree.has(1));
  BOOST_CHECK(tree.has(2));
  BOOST_CHECK(tree.has(3));
  BOOST_CHECK_EQUAL(tree.size(), 3);
}

BOOST_AUTO_TEST_CASE(RotateLeftLarge)
{
  vishnyakov::BSTree< int, std::string, std::less< int > > tree;
  tree.push(10, "ten");
  tree.push(5, "five");
  tree.push(15, "fifteen");
  tree.push(12, "twelve");
  tree.push(20, "twenty");
  tree.push(13, "thirteen");

  auto it = tree.find(15);
  tree.rotate_left_large(it);

  BOOST_CHECK_EQUAL(tree.size(), 6);
}

BOOST_AUTO_TEST_CASE(RotateRightLarge)
{
  vishnyakov::BSTree< int, std::string, std::less< int > > tree;
  tree.push(10, "ten");
  tree.push(20, "twenty");
  tree.push(5, "five");
  tree.push(3, "three");
  tree.push(8, "eight");
  tree.push(4, "four");

  auto it = tree.find(5);
  tree.rotate_right_large(it);

  BOOST_CHECK_EQUAL(tree.size(), 6);
}

BOOST_AUTO_TEST_CASE(RotateLeftInvalid)
{
  vishnyakov::BSTree< int, std::string, std::less< int > > tree;
  tree.push(1, "one");

  auto it = tree.find(1);
  auto new_it = tree.rotate_left(it);

  BOOST_CHECK(tree.has(1));
  BOOST_CHECK_EQUAL(tree.size(), 1);
}

BOOST_AUTO_TEST_CASE(RotateRightInvalid)
{
  vishnyakov::BSTree< int, std::string, std::less< int > > tree;
  tree.push(1, "one");

  auto it = tree.find(1);
  auto new_it = tree.rotate_right(it);

  BOOST_CHECK(tree.has(1));
  BOOST_CHECK_EQUAL(tree.size(), 1);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(BSTreeHeightTests)

BOOST_AUTO_TEST_CASE(HeightEmptyTree)
{
  vishnyakov::BSTree< int, std::string, std::less< int > > tree;
  BOOST_CHECK_EQUAL(tree.height(), 0);
}

BOOST_AUTO_TEST_CASE(HeightSingleNode)
{
  vishnyakov::BSTree< int, std::string, std::less< int > > tree;
  tree.push(1, "one");
  BOOST_CHECK_EQUAL(tree.height(), 1);
}

BOOST_AUTO_TEST_CASE(HeightMultipleNodes)
{
  vishnyakov::BSTree< int, std::string, std::less< int > > tree;
  tree.push(5, "five");
  tree.push(3, "three");
  tree.push(7, "seven");
  tree.push(2, "two");
  tree.push(4, "four");

  BOOST_CHECK_EQUAL(tree.height(), 3);
}

BOOST_AUTO_TEST_CASE(HeightAtPosition)
{
  vishnyakov::BSTree< int, std::string, std::less< int > > tree;
  tree.push(5, "five");
  tree.push(3, "three");
  tree.push(7, "seven");
  tree.push(2, "two");
  tree.push(4, "four");

  auto it = tree.find(3);
  BOOST_CHECK_EQUAL(tree.height(it), 2);
}

BOOST_AUTO_TEST_CASE(HeightAtInvalidPosition)
{
  vishnyakov::BSTree< int, std::string, std::less< int > > tree;
  auto it = tree.end();
  BOOST_CHECK_EQUAL(tree.height(it), 0);
}

BOOST_AUTO_TEST_SUITE_END()

