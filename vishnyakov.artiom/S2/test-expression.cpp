#include <boost/test/unit_test.hpp>
#include "expression.hpp"

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
  std::string result;

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

