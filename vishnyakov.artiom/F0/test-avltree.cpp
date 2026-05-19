#include <boost/test/unit_test.hpp>
#include "avltree.hpp"

namespace vishnyakov
{
  BOOST_AUTO_TEST_SUITE(AVLTreeTests)

  BOOST_AUTO_TEST_CASE(ConstructorDefault)
  {
    AVLTree< int, std::string, std::less< int > > tree;
    BOOST_TEST(tree.empty());
    BOOST_TEST(tree.size() == 0);
  }

  BOOST_AUTO_TEST_CASE(PushAndHas)
  {
    AVLTree< int, std::string, std::less< int > > tree;
    tree.push(1, "one");
    tree.push(2, "two");
    tree.push(3, "three");

    BOOST_TEST(tree.size() == 3);
    BOOST_TEST(!tree.empty());
    BOOST_TEST(tree.has(1));
    BOOST_TEST(tree.has(2));
    BOOST_TEST(tree.has(3));
    BOOST_TEST(!tree.has(4));
  }

  BOOST_AUTO_TEST_CASE(PushDuplicate)
  {
    AVLTree< int, std::string, std::less< int > > tree;
    tree.push(1, "one");
    tree.push(1, "one_again");

    BOOST_TEST(tree.size() == 1);
    BOOST_TEST(tree.has(1));
  }

  BOOST_AUTO_TEST_CASE(Drop)
  {
    AVLTree< int, std::string, std::less< int > > tree;
    tree.push(1, "one");
    tree.push(2, "two");
    tree.push(3, "three");

    std::string val = tree.drop(2);
    BOOST_TEST(val == "two");
    BOOST_TEST(tree.size() == 2);
    BOOST_TEST(!tree.has(2));
    BOOST_TEST(tree.has(1));
    BOOST_TEST(tree.has(3));
  }

  BOOST_AUTO_TEST_CASE(DropNonExistent)
  {
    AVLTree< int, std::string, std::less< int > > tree;
    tree.push(1, "one");
    BOOST_CHECK_THROW(tree.drop(2), std::out_of_range);
  }

  BOOST_AUTO_TEST_CASE(DropRoot)
  {
    AVLTree< int, std::string, std::less< int > > tree;
    tree.push(2, "two");
    tree.push(1, "one");
    tree.push(3, "three");

    std::string val = tree.drop(2);
    BOOST_TEST(val == "two");
    BOOST_TEST(tree.size() == 2);
    BOOST_TEST(tree.has(1));
    BOOST_TEST(tree.has(3));
  }

  BOOST_AUTO_TEST_CASE(DropLeaf)
  {
    AVLTree< int, std::string, std::less< int > > tree;
    tree.push(2, "two");
    tree.push(1, "one");
    tree.push(3, "three");

    std::string val = tree.drop(3);
    BOOST_TEST(val == "three");
    BOOST_TEST(tree.size() == 2);
    BOOST_TEST(tree.has(1));
    BOOST_TEST(tree.has(2));
  }

  BOOST_AUTO_TEST_CASE(DropNodeWithOneChild)
  {
    AVLTree< int, std::string, std::less< int > > tree;
    tree.push(2, "two");
    tree.push(1, "one");
    tree.push(3, "three");
    tree.push(4, "four");

    std::string val = tree.drop(3);
    BOOST_TEST(val == "three");
    BOOST_TEST(tree.size() == 3);
    BOOST_TEST(tree.has(1));
    BOOST_TEST(tree.has(2));
    BOOST_TEST(tree.has(4));
  }

  BOOST_AUTO_TEST_CASE(AvlBalanceAfterInsert)
  {
    AVLTree< int, std::string, std::less< int > > tree;
    tree.push(10, "ten");
    tree.push(20, "twenty");
    tree.push(30, "thirty");

    BOOST_TEST(tree.has(10));
    BOOST_TEST(tree.has(20));
    BOOST_TEST(tree.has(30));
    BOOST_TEST(tree.size() == 3);
  }

  BOOST_AUTO_TEST_CASE(AvlBalanceAfterInsertReverse)
  {
    AVLTree< int, std::string, std::less< int > > tree;
    tree.push(30, "thirty");
    tree.push(20, "twenty");
    tree.push(10, "ten");

    BOOST_TEST(tree.has(10));
    BOOST_TEST(tree.has(20));
    BOOST_TEST(tree.has(30));
    BOOST_TEST(tree.size() == 3);
  }

  BOOST_AUTO_TEST_CASE(AvlBalanceAfterDrop)
  {
    AVLTree< int, std::string, std::less< int > > tree;
    tree.push(10, "ten");
    tree.push(5, "five");
    tree.push(15, "fifteen");
    tree.push(3, "three");
    tree.push(7, "seven");
    tree.push(12, "twelve");
    tree.push(18, "eighteen");

    tree.drop(5);
    BOOST_TEST(tree.size() == 6);
    BOOST_TEST(!tree.has(5));
    BOOST_TEST(tree.has(10));
    BOOST_TEST(tree.has(3));
    BOOST_TEST(tree.has(7));
    BOOST_TEST(tree.has(15));
  }

  BOOST_AUTO_TEST_CASE(CopyConstructor)
  {
    AVLTree< int, std::string, std::less< int > > tree1;
    tree1.push(1, "one");
    tree1.push(2, "two");

    AVLTree< int, std::string, std::less< int > > tree2(tree1);
    BOOST_TEST(tree2.size() == 2);
    BOOST_TEST(tree2.has(1));
    BOOST_TEST(tree2.has(2));
  }

  BOOST_AUTO_TEST_CASE(MoveConstructor)
  {
    AVLTree< int, std::string, std::less< int > > tree1;
    tree1.push(1, "one");
    tree1.push(2, "two");

    AVLTree< int, std::string, std::less< int > > tree2(std::move(tree1));
    BOOST_TEST(tree2.size() == 2);
    BOOST_TEST(tree2.has(1));
    BOOST_TEST(tree2.has(2));
    BOOST_TEST(tree1.empty());
  }

  BOOST_AUTO_TEST_CASE(CopyAssignment)
  {
    AVLTree< int, std::string, std::less< int > > tree1;
    tree1.push(1, "one");
    tree1.push(2, "two");

    AVLTree< int, std::string, std::less< int > > tree2;
    tree2 = tree1;
    BOOST_TEST(tree2.size() == 2);
    BOOST_TEST(tree2.has(1));
    BOOST_TEST(tree2.has(2));
  }

  BOOST_AUTO_TEST_CASE(MoveAssignment)
  {
    AVLTree< int, std::string, std::less< int > > tree1;
    tree1.push(1, "one");
    tree1.push(2, "two");

    AVLTree< int, std::string, std::less< int > > tree2;
    tree2 = std::move(tree1);
    BOOST_TEST(tree2.size() == 2);
    BOOST_TEST(tree2.has(1));
    BOOST_TEST(tree2.has(2));
    BOOST_TEST(tree1.empty());
  }

  BOOST_AUTO_TEST_CASE(Clear)
  {
    AVLTree< int, std::string, std::less< int > > tree;
    tree.push(1, "one");
    tree.push(2, "two");
    tree.push(3, "three");

    BOOST_TEST(tree.size() == 3);
    tree.clear();
    BOOST_TEST(tree.empty());
    BOOST_TEST(tree.size() == 0);
    BOOST_TEST(!tree.has(1));
  }

  BOOST_AUTO_TEST_SUITE_END()
}

