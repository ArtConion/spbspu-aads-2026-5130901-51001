#define BOOST_TEST_MODULE StackTest
#include <boost/test/unit_test.hpp>
#include "stack.hpp"

BOOST_AUTO_TEST_SUITE(StackTests)

BOOST_AUTO_TEST_CASE(TestStackPushPop)
{
  vishnyakov::Stack< int > stack;
  BOOST_CHECK(stack.empty());
  BOOST_CHECK_EQUAL(stack.size(), 0);

  stack.push(10);
  BOOST_CHECK(!stack.empty());
  BOOST_CHECK_EQUAL(stack.size(), 1);
  BOOST_CHECK_EQUAL(stack.top(), 10);

  stack.push(20);
  BOOST_CHECK_EQUAL(stack.size(), 2);
  BOOST_CHECK_EQUAL(stack.top(), 20);

  int value = stack.pop();
  BOOST_CHECK_EQUAL(value, 20);
  BOOST_CHECK_EQUAL(stack.size(), 1);
  BOOST_CHECK_EQUAL(stack.top(), 10);

  value = stack.pop();
  BOOST_CHECK_EQUAL(value, 10);
  BOOST_CHECK(stack.empty());
  BOOST_CHECK_EQUAL(stack.size(), 0);
}

BOOST_AUTO_TEST_CASE(TestStackCopy)
{
  vishnyakov::Stack< int > stack1;
  stack1.push(1);
  stack1.push(2);
  stack1.push(3);

  vishnyakov::Stack< int > stack2(stack1);
  BOOST_CHECK_EQUAL(stack2.size(), 3);
  BOOST_CHECK_EQUAL(stack2.pop(), 3);
  BOOST_CHECK_EQUAL(stack2.pop(), 2);
  BOOST_CHECK_EQUAL(stack2.pop(), 1);
}

BOOST_AUTO_TEST_CASE(TestStackAssignment)
{
  vishnyakov::Stack< int > stack1;
  stack1.push(1);
  stack1.push(2);

  vishnyakov::Stack< int > stack2;
  stack2 = stack1;
  BOOST_CHECK_EQUAL(stack2.size(), 2);
  BOOST_CHECK_EQUAL(stack2.pop(), 2);
  BOOST_CHECK_EQUAL(stack2.pop(), 1);
}

BOOST_AUTO_TEST_CASE(TestStackMove)
{
  vishnyakov::Stack< int > stack1;
  stack1.push(1);
  stack1.push(2);

  vishnyakov::Stack<int> stack2(std::move(stack1));
  BOOST_CHECK_EQUAL(stack2.size(), 2);
  BOOST_CHECK_EQUAL(stack2.pop(), 2);
  BOOST_CHECK_EQUAL(stack2.pop(), 1);
  BOOST_CHECK(stack1.empty());
}

BOOST_AUTO_TEST_CASE(TestStackClear)
{
  vishnyakov::Stack< int > stack;
  stack.push(1);
  stack.push(2);
  stack.push(3);
  BOOST_CHECK_EQUAL(stack.size(), 3);

  stack.clear();
  BOOST_CHECK(stack.empty());
  BOOST_CHECK_EQUAL(stack.size(), 0);
}

BOOST_AUTO_TEST_CASE(TestStackSwap)
{
  vishnyakov::Stack< int > stack1;
  stack1.push(1);
  stack1.push(2);

  vishnyakov::Stack< int > stack2;
  stack2.push(3);
  stack2.push(4);

  stack1.swap(stack2);
  BOOST_CHECK_EQUAL(stack1.size(), 2);
  BOOST_CHECK_EQUAL(stack2.size(), 2);
  BOOST_CHECK_EQUAL(stack1.pop(), 4);
  BOOST_CHECK_EQUAL(stack2.pop(), 2);
}

BOOST_AUTO_TEST_CASE(TestStackTopEmpty)
{
  vishnyakov::Stack< int > stack;
  BOOST_CHECK_THROW(stack.top(), std::underflow_error);
}

BOOST_AUTO_TEST_CASE(TestStackPopEmpty)
{
  vishnyakov::Stack< int > stack;
  BOOST_CHECK_THROW(stack.pop(), std::underflow_error);
}

BOOST_AUTO_TEST_CASE(TestStackWithStrings)
{
  vishnyakov::Stack< std::string > stack;
  stack.push("first");
  stack.push("second");
  stack.push("third");

  BOOST_CHECK_EQUAL(stack.size(), 3);
  BOOST_CHECK_EQUAL(stack.top(), "third");
  BOOST_CHECK_EQUAL(stack.pop(), "third");
  BOOST_CHECK_EQUAL(stack.pop(), "second");
  BOOST_CHECK_EQUAL(stack.pop(), "first");
  BOOST_CHECK(stack.empty());
}

BOOST_AUTO_TEST_SUITE_END()

