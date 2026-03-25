#ifdef BUILD_TESTS

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

BOOST_AUTO_TEST_CASE(TestStackPopEmpty)
{
  vishnyakov::Stack< int > stack;
  BOOST_CHECK_THROW(stack.pop(), std::underflow_error);
}

BOOST_AUTO_TEST_CASE(TestStackWithStrings)
{
  vishnyakov::Stack<std::string> stack;
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
  vishnyakov::Queue<std::string> queue;
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
  BOOST_CHECK_EQUAL(vishnyakov::reverseNumber(1000), 1);
  BOOST_CHECK_EQUAL(vishnyakov::reverseNumber(123456789), 987654321);
}

BOOST_AUTO_TEST_CASE(TestIsNumber)
{
  BOOST_CHECK(vishnyakov::isNumber("123"));
  BOOST_CHECK(vishnyakov::isNumber("-123"));
  BOOST_CHECK(vishnyakov::isNumber("+456"));
  BOOST_CHECK(vishnyakov::isNumber("0"));
  BOOST_CHECK(vishnyakov::isNumber("000"));
  BOOST_CHECK(!vishnyakov::isNumber(""));
  BOOST_CHECK(!vishnyakov::isNumber("abc"));
  BOOST_CHECK(!vishnyakov::isNumber("12a"));
  BOOST_CHECK(!vishnyakov::isNumber("+"));
  BOOST_CHECK(!vishnyakov::isNumber("-"));
  BOOST_CHECK(!vishnyakov::isNumber("12.34"));
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
  BOOST_CHECK_EQUAL(vishnyakov::getPriority(")"), 0);
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
  BOOST_CHECK(!vishnyakov::isOperator("abc"));
}

BOOST_AUTO_TEST_CASE(TestApplyBinaryOp)
{
  BOOST_CHECK_EQUAL(vishnyakov::applyBinaryOp("+", 5, 3), 8);
  BOOST_CHECK_EQUAL(vishnyakov::applyBinaryOp("-", 5, 3), 2);
  BOOST_CHECK_EQUAL(vishnyakov::applyBinaryOp("*", 5, 3), 15);
  BOOST_CHECK_EQUAL(vishnyakov::applyBinaryOp("/", 6, 3), 2);
  BOOST_CHECK_EQUAL(vishnyakov::applyBinaryOp("%", 7, 3), 1);
  BOOST_CHECK_EQUAL(vishnyakov::applyBinaryOp("/", -6, 3), -2);
  BOOST_CHECK_EQUAL(vishnyakov::applyBinaryOp("%", -7, 3), 2);
}

BOOST_AUTO_TEST_CASE(TestApplyBinaryOpDivisionByZero)
{
  BOOST_CHECK_THROW(vishnyakov::applyBinaryOp("/", 5, 0), std::runtime_error);
  BOOST_CHECK_THROW(vishnyakov::applyBinaryOp("%", 5, 0), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(TestApplyBinaryOpUnknownOperator)
{
  BOOST_CHECK_THROW(vishnyakov::applyBinaryOp("^", 5, 3), std::runtime_error);
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
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("20 / ( 2 + 3 )"), 4);
}

BOOST_AUTO_TEST_CASE(TestEvaluateExpressionWithParentheses)
{
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("( 1 + 2 ) * 3"), 9);
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("( 10 - 5 ) * ( 2 + 3 )"), 25);
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("( 20 / ( 2 + 3 ) ) % 4"), 0);
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("( ( 1 + 2 ) * 3 ) / 3"), 3);
}

BOOST_AUTO_TEST_CASE(TestEvaluateExpressionWithUnaryOperator)
{
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("# 123"), 321);
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("# -123"), -321);
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("# 100"), 1);
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("2 + # 123"), 125);
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("# 123 + 4"), 325);
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("# 123 + # 456"), 777);
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("# ( 100 + 20 )"), 21);
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("( # 123 ) * 2"), 642);
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("# # 123"), 123);
}

BOOST_AUTO_TEST_CASE(TestEvaluateExpressionComplex)
{
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("( 10 + 2 ) * ( 3 - 1 ) / 4"), 6);
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("100 % 30 + 20 / 5"), 14);
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("# ( 123 + 321 )"), 444);
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("( # 123 ) + ( # 456 )"), 777);
}

BOOST_AUTO_TEST_CASE(TestInfixToPostfix)
{
  vishnyakov::Queue<std::string> postfix;

  postfix = vishnyakov::infixToPostfix("1 + 2");
  BOOST_CHECK_EQUAL(postfix.pop(), "1");
  BOOST_CHECK_EQUAL(postfix.pop(), "2");
  BOOST_CHECK_EQUAL(postfix.pop(), "+");

  postfix = vishnyakov::infixToPostfix("1 + 2 * 3");
  BOOST_CHECK_EQUAL(postfix.pop(), "1");
  BOOST_CHECK_EQUAL(postfix.pop(), "2");
  BOOST_CHECK_EQUAL(postfix.pop(), "3");
  BOOST_CHECK_EQUAL(postfix.pop(), "*");
  BOOST_CHECK_EQUAL(postfix.pop(), "+");

  postfix = vishnyakov::infixToPostfix("( 1 + 2 ) * 3");
  BOOST_CHECK_EQUAL(postfix.pop(), "1");
  BOOST_CHECK_EQUAL(postfix.pop(), "2");
  BOOST_CHECK_EQUAL(postfix.pop(), "+");
  BOOST_CHECK_EQUAL(postfix.pop(), "3");
  BOOST_CHECK_EQUAL(postfix.pop(), "*");

  postfix = vishnyakov::infixToPostfix("# 123");
  BOOST_CHECK_EQUAL(postfix.pop(), "123");
  BOOST_CHECK_EQUAL(postfix.pop(), "#");

  postfix = vishnyakov::infixToPostfix("2 + # 123");
  BOOST_CHECK_EQUAL(postfix.pop(), "2");
  BOOST_CHECK_EQUAL(postfix.pop(), "123");
  BOOST_CHECK_EQUAL(postfix.pop(), "#");
  BOOST_CHECK_EQUAL(postfix.pop(), "+");
}

BOOST_AUTO_TEST_CASE(TestInvalidExpressions)
{
  BOOST_CHECK_THROW(vishnyakov::evaluateExpression("1 +"), std::runtime_error);
  BOOST_CHECK_THROW(vishnyakov::evaluateExpression("1 2 +"), std::runtime_error);
  BOOST_CHECK_THROW(vishnyakov::evaluateExpression("( 1 + 2"), std::runtime_error);
  BOOST_CHECK_THROW(vishnyakov::evaluateExpression("1 + 2 )"), std::runtime_error);
  BOOST_CHECK_THROW(vishnyakov::evaluateExpression("1 / 0"), std::runtime_error);
  BOOST_CHECK_THROW(vishnyakov::evaluateExpression("abc + 123"), std::runtime_error);
  BOOST_CHECK_THROW(vishnyakov::evaluateExpression("#"), std::runtime_error);
  BOOST_CHECK_THROW(vishnyakov::evaluateExpression("# + 123"), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(TestEdgeCases)
{
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("0"), 0);
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("999"), 999);
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("# 0"), 0);
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("( 1 )"), 1);
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("( ( ( 5 ) ) )"), 5);
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("1 + ( 2 * ( 3 + 4 ) )"), 15);
}

BOOST_AUTO_TEST_CASE(TestAdditionOverflow)
{
  BOOST_CHECK_THROW(vishnyakov::evaluateExpression("9223372036854775807 + 1"), std::overflow_error);
}

BOOST_AUTO_TEST_CASE(TestMultiplicationOverflow)
{
  BOOST_CHECK_THROW(vishnyakov::evaluateExpression("9223372036854775807 * 2"), std::overflow_error);
}

BOOST_AUTO_TEST_CASE(TestMultiplicationUnderflow)
{
  BOOST_CHECK_THROW(vishnyakov::evaluateExpression("( 0 - 9223372036854775807 ) * 2"), std::overflow_error);
}

BOOST_AUTO_TEST_CASE(TestPositiveModWithNegativeNumbers)
{
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("( 0 - 430975 ) % 562"), 79);
}

BOOST_AUTO_TEST_CASE(TestLargeNumbers)
{
  long long large = vishnyakov::evaluateExpression("123456789 + 987654321");
  BOOST_CHECK_EQUAL(large, 1111111110);

  long long reversed = vishnyakov::evaluateExpression("# 123456789");
  BOOST_CHECK_EQUAL(reversed, 987654321);
}

BOOST_AUTO_TEST_CASE(TestExpressionWithMultipleSpaces)
{
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("1   +   2"), 3);
  BOOST_CHECK_EQUAL(vishnyakov::evaluateExpression("(   1   +   2   )   *   3"), 9);
}

BOOST_AUTO_TEST_SUITE_END()

#endif

