#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include "stack.hpp"
#include "queue.hpp"
#include <string>

namespace vishnyakov
{
  bool willOverflowAdd(long long left, long long right);
  bool willOverflowSub(long long left, long long right);
  bool willOverflowMul(long long left, long long right);

  long long reverseNumber(long long num);

  bool isNumber(const std::string& s);

  int getPriority(const std::string& op);

  bool isOperator(const std::string& token);

  long long applyBinaryOp(const std::string& op, long long left, long long right);

  Queue< std::string > infixToPostfix(const std::string& expression);

  long long evaluatePostfix(const Queue< std::string >& postfix);

  long long evaluateExpression(const std::string& expression);
}

#endif

