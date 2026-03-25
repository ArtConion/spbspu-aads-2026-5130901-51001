#define BOOST_TEST_MODULE QueueTest
#include <boost/test/unit_test.hpp>
#include "queue.hpp"

BOOST_AUTO_TEST_SUITE(QueueTests)

BOOST_AUTO_TEST_CASE(TestQueuePushPop)
{
  vishnyakov::Queue< int > queue;
  BOOST_CHECK(queue.empty());
  BOOST_CHECK_EQUAL(queue.size(), 0);

  queue.push(10);
  BOOST_CHECK(!queue.empty());
  BOOST_CHECK_EQUAL(queue.size(), 1);
  BOOST_CHECK_EQUAL(queue.front(), 10);
  BOOST_CHECK_EQUAL(queue.back(), 10);

  queue.push(20);
  BOOST_CHECK_EQUAL(queue.size(), 2);
  BOOST_CHECK_EQUAL(queue.front(), 10);
  BOOST_CHECK_EQUAL(queue.back(), 20);

  int value = queue.pop();
  BOOST_CHECK_EQUAL(value, 10);
  BOOST_CHECK_EQUAL(queue.size(), 1);
  BOOST_CHECK_EQUAL(queue.front(), 20);
  BOOST_CHECK_EQUAL(queue.back(), 20);

  value = queue.pop();
  BOOST_CHECK_EQUAL(value, 20);
  BOOST_CHECK(queue.empty());
  BOOST_CHECK_EQUAL(queue.size(), 0);
}

BOOST_AUTO_TEST_CASE(TestQueueCopy)
{
  vishnyakov::Queue< int > queue1;
  queue1.push(1);
  queue1.push(2);
  queue1.push(3);

  vishnyakov::Queue< int > queue2(queue1);
  BOOST_CHECK_EQUAL(queue2.size(), 3);
  BOOST_CHECK_EQUAL(queue2.pop(), 1);
  BOOST_CHECK_EQUAL(queue2.pop(), 2);
  BOOST_CHECK_EQUAL(queue2.pop(), 3);
}

BOOST_AUTO_TEST_CASE(TestQueueAssignment)
{
  vishnyakov::Queue< int > queue1;
  queue1.push(1);
  queue1.push(2);

  vishnyakov::Queue< int > queue2;
  queue2 = queue1;
  BOOST_CHECK_EQUAL(queue2.size(), 2);
  BOOST_CHECK_EQUAL(queue2.pop(), 1);
  BOOST_CHECK_EQUAL(queue2.pop(), 2);
}

BOOST_AUTO_TEST_CASE(TestQueueMove)
{
  vishnyakov::Queue< int > queue1;
  queue1.push(1);
  queue1.push(2);

  vishnyakov::Queue< int > queue2(std::move(queue1));
  BOOST_CHECK_EQUAL(queue2.size(), 2);
  BOOST_CHECK_EQUAL(queue2.pop(), 1);
  BOOST_CHECK_EQUAL(queue2.pop(), 2);
  BOOST_CHECK(queue1.empty());
}

BOOST_AUTO_TEST_CASE(TestQueueClear)
{
  vishnyakov::Queue< int > queue;
  queue.push(1);
  queue.push(2);
  queue.push(3);
  BOOST_CHECK_EQUAL(queue.size(), 3);

  queue.clear();
  BOOST_CHECK(queue.empty());
  BOOST_CHECK_EQUAL(queue.size(), 0);
}

BOOST_AUTO_TEST_CASE(TestQueueSwap)
{
  vishnyakov::Queue< int > queue1;
  queue1.push(1);
  queue1.push(2);

  vishnyakov::Queue< int > queue2;
  queue2.push(3);
  queue2.push(4);

  queue1.swap(queue2);
  BOOST_CHECK_EQUAL(queue1.size(), 2);
  BOOST_CHECK_EQUAL(queue2.size(), 2);
  BOOST_CHECK_EQUAL(queue1.pop(), 3);
  BOOST_CHECK_EQUAL(queue2.pop(), 1);
}

BOOST_AUTO_TEST_CASE(TestQueueFrontEmpty)
{
  vishnyakov::Queue< int > queue;
  BOOST_CHECK_THROW(queue.front(), std::underflow_error);
}

BOOST_AUTO_TEST_CASE(TestQueueBackEmpty)
{
  vishnyakov::Queue< int > queue;
  BOOST_CHECK_THROW(queue.back(), std::underflow_error);
}

BOOST_AUTO_TEST_CASE(TestQueuePopEmpty)
{
  vishnyakov::Queue< int > queue;
  BOOST_CHECK_THROW(queue.pop(), std::underflow_error);
}

BOOST_AUTO_TEST_CASE(TestQueueWithStrings)
{
  vishnyakov::Queue< std::string > queue;
  queue.push("first");
  queue.push("second");
  queue.push("third");

  BOOST_CHECK_EQUAL(queue.size(), 3);
  BOOST_CHECK_EQUAL(queue.front(), "first");
  BOOST_CHECK_EQUAL(queue.back(), "third");
  BOOST_CHECK_EQUAL(queue.pop(), "first");
  BOOST_CHECK_EQUAL(queue.pop(), "second");
  BOOST_CHECK_EQUAL(queue.pop(), "third");
  BOOST_CHECK(queue.empty());
}

BOOST_AUTO_TEST_CASE(TestQueueWithComplexTypes)
{
  vishnyakov::Queue< std::pair< int, int > > queue;
  queue.push(std::make_pair(1, 2));
  queue.push(std::make_pair(3, 4));

  std::pair< int, int > p1 = queue.pop();
  BOOST_CHECK_EQUAL(p1.first, 1);
  BOOST_CHECK_EQUAL(p1.second, 2);

  std::pair< int, int > p2 = queue.pop();
  BOOST_CHECK_EQUAL(p2.first, 3);
  BOOST_CHECK_EQUAL(p2.second, 4);
}

BOOST_AUTO_TEST_SUITE_END()

