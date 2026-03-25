#define BOOST_TEST_MODULE S2
#include <boost/test/unit_test.hpp>
#include "stack.hpp"
#include "queue.hpp"
#include "expression.hpp"

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

BOOST_AUTO_TEST_CASE(TestStackMove)
{
  vishnyakov::Stack< int > stack1;
  stack1.push(1);
  stack1.push(2);

  vishnyakov::Stack< int > stack2(std::move(stack1));
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

BOOST_AUTO_TEST_CASE(TestStackTopEmpty)
{
  vishnyakov::Stack< int > stack;
  BOOST_CHECK_THROW(stack.top(), std::underflow_error);
}

BOOST_AUTO_TEST_SUITE_END()

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

BOOST_AUTO_TEST_CASE(TestQueueFrontEmpty)
{
  vishnyakov::Queue< int > queue;
  BOOST_CHECK_THROW(queue.front(), std::underflow_error);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(ExpressionTests)

BOOST_AUTO_TEST_CASE(TestReverseNumber)
{
  BOOST_CHECK_EQUAL(vishnyakov::reverseNumber(123), 321);
  BOOST_CHECK_EQUAL(vishnyakov::reverseNumber(0), 0);
  BOOST_CHECK_EQUAL(vishnyakov::reverseNumber(1), 1);
  BOOST_CHECK_EQUAL(vishnyakov::reverseNumber(12345), 54321);
  BOOST_CHECK_EQUAL(vishnyakov::reverseNumber(-123), -321);
  BOOST_CHECK_EQUAL(vishnyakov::reverseNumber(100), 1);
  BOOST_CHECK_EQUAL(vishnyakov::reverseNumber(-100), -1);
}

BOOST_AUTO_TEST_CASE(TestIsNumber)
{
  BOOST_CHECK(vishnyakov::isNumber("123"));
  BOOST_CHECK(vishnyakov::isNumber("-123"));
  BOOST_CHECK(vishnyakov::isNumber("+456"));
  BOOST_CHECK(vishnyakov::isNumber("0"));
  BOOST_CHECK(!vishnyakov::isNumber(""));
  BOOST_CHECK(!vishnyakov::isNumber("abc"));
  BOOST_CHECK(!vishnyakov::isNumber("12a"));
  BOOST_CHECK(!vishnyakov::isNumber("+"));
  BOOST_CHECK(!vishnyakov::isNumber("-"));
}

BOOST_AUTO_TEST_CASE(TestGetPriority)
{
  BOOST_CHECK_EQUAL(vishnyakov::getPriority("#"), 3);
  BOOST_CHECK_EQUAL(vishnyakov::getPriority("*"), 2);
  BOOST_CHECK_EQUAL(vishnyakov::getPriority("/"), 2);
  BOOST_CHECK_EQUAL(vishnyakov::getPriority("%"), 2);
  BOOST_CHECK_EQUAL(vishnyakov::getPriority("+"), 1);
  BOOST_CHECK_EQUAL(vishnyakov::getPriority("-"), 1);
  BOOST_CHECK_EQUAL(vishnyakov::getPriority("("), 0);
}

BOOST_AUTO_TEST_CASE(TestIsOperator)
{
  BOOST_CHECK(vishnyakov::isOperator("+"));
  BOOST_CHECK(vishnyakov::isOperator("-"));
  BOOST_CHECK(vishnyakov::isOperator("*"));
  BOOST_CHECK(vishnyakov::isOperator("/"));
  BOOST_CHECK(vishnyakov::isOperator("%"));
  BOOST_CHECK(vishnyakov::isOperator("#"));
  BOOST_CHECK(!vishnyakov::isOperator("("));
  BOOST_CHECK(!vishnyakov::isOperator(")"));
  BOOST_CHECK(!vishnyakov::isOperator("123"));
}

BOOST_AUTO_TEST_CASE(TestApplyBinaryOp)
{
  BOOST_CHECK_EQUAL(vishnyakov::applyBinaryOp("+", 5, 3), 8);
  BOOST_CHECK_EQUAL(vishnyakov::applyBinaryOp("-", 5, 3), 2);
  BOOST_CHECK_EQUAL(vishnyakov::applyBinaryOp("*", 5, 3), 15);
  BOOST_CHECK_EQUAL(vishnyakov::applyBinaryOp("/", 6, 3), 2);
  BOOST_CHECK_EQUAL(vishnyakov::applyBinaryOp("%", 7, 3), 1);
}

BOOST_AUTO_TEST_CASE(TestApplyBinaryOpDivisionByZero)
{
  BOOST_CHECK_THROW(vishnyakov::applyBinaryOp("/", 5, 0), std::runtime_error);
  BOOST_CHECK_THROW(vishnyakov::applyBinaryOp("%", 5, 0), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(TestEvaluateExpressionSimple)
{
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("1 + 2"), 3);
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("10 - 5"), 5);
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("3 * 4"), 12);
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("15 / 3"), 5);
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("10 % 3"), 1);
}

BOOST_AUTO_TEST_CASE(TestEvaluateExpressionWithPrecedence)
{
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("1 + 2 * 3"), 7);
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("10 - 2 * 3"), 4);
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("20 / 2 + 3"), 13);
}

BOOST_AUTO_TEST_CASE(TestEvaluateExpressionWithParentheses)
{
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("( 1 + 2 ) * 3"), 9);
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("( 10 - 5 ) * ( 2 + 3 )"), 25);
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("( 20 / ( 2 + 3 ) ) % 4"), 0);
}

BOOST_AUTO_TEST_CASE(TestEvaluateExpressionWithUnaryOperator)
{
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("# 123"), 321);
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("# -123"), -321);
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("2 + # 123"), 125);
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("# 123 + 4"), 325);
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("# ( 100 + 20 )"), 21);
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("( # 123 ) * 2"), 642);
}

BOOST_AUTO_TEST_CASE(TestAdditionOverflow)
{
  BOOST_CHECK_THROW(vishnyakov::evaluateExpression("9223372036854775807 + 1"), std::overflow_error);
}

BOOST_AUTO_TEST_CASE(TestMultiplicationOverflow)
{
  BOOST_CHECK_THROW(vishnyakov::evaluateExpression("9223372036854775807 * 2"), std::overflow_error);
}

BOOST_AUTO_TEST_CASE(TestPositiveModWithNegativeNumbers)
{
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("( 0 - 430975 ) % 562"), 79);
}

BOOST_AUTO_TEST_SUITE_END()

