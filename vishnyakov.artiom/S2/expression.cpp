#include "expression.hpp"
#include <sstream>
#include <cctype>
#include <stdexcept>
#include <climits>

namespace vishnyakov
{
  bool willOverflowAdd(long long left, long long right)
  {
    if (right > 0 && left > LLONG_MAX - right)
    {
      return true;
    }
    if (right < 0 && left < LLONG_MIN - right)
    {
      return true;
    }
    return false;
  }

  bool willOverflowSub(long long left, long long right)
  {
    if (right < 0 && left > LLONG_MAX + right)
    {
      return true;
    }
    if (right > 0 && left < LLONG_MIN + right)
    {
      return true;
    }
    return false;
  }

  bool willOverflowMul(long long left, long long right)
  {
    if (left == 0 || right == 0)
    {
      return false;
    }

    if (left > 0 && right > 0 && left > LLONG_MAX / right)
    {
      return true;
    }
    if (left > 0 && right < 0 && right < LLONG_MIN / left)
    {
      return true;
    }
    if (left < 0 && right > 0 && left < LLONG_MIN / right)
    {
      return true;
    }
    if (left < 0 && right < 0 && left < LLONG_MAX / right)
    {
      return true;
    }
    return false;
  }

  long long reverseNumber(long long num)
  {
    if (num == 0)
    {
      return 0;
    }

    bool negative = num < 0;
    long long absNum = negative ? -num : num;
    long long reversed = 0;

    while (absNum > 0)
    {
      if (reversed > LLONG_MAX / 10)
      {
        throw std::overflow_error("Reverse operation overflow");
      }
      reversed = reversed * 10 + (absNum % 10);
      absNum /= 10;
    }

    long long result = negative ? -reversed : reversed;
    return result;
  }

  bool isNumber(const std::string& s)
  {
    if (s.empty())
    {
      return false;
    }

    size_t start = 0;
    if (s[0] == '-' || s[0] == '+')
    {
      if (s.length() == 1)
      {
        return false;
      }
      start = 1;
    }

    for (size_t i = start; i < s.length(); ++i)
    {
      if (!std::isdigit(s[i]))
      {
        return false;
      }
    }
    return true;
  }

  int getPriority(const std::string& op)
  {
    if (op == "#")
    {
      return 3;
    }
    if (op == "*" || op == "/" || op == "%")
    {
      return 2;
    }
    if (op == "+" || op == "-")
    {
      return 1;
    }
    return 0;
  }

  bool isOperator(const std::string& token)
  {
    return token == "+" || token == "-" || token == "*" ||
           token == "/" || token == "%" || token == "#";
  }

  long long applyBinaryOp(const std::string& op, long long left, long long right)
  {
    if (op == "+")
    {
      if (willOverflowAdd(left, right))
      {
        throw std::overflow_error("Addition overflow");
      }
      return left + right;
    }
    if (op == "-")
    {
      if (willOverflowSub(left, right))
      {
        throw std::overflow_error("Subtraction overflow");
      }
      return left - right;
    }
    if (op == "*")
    {
      if (willOverflowMul(left, right))
      {
        throw std::overflow_error("Multiplication overflow");
      }
      return left * right;
    }
    if (op == "/")
    {
      if (right == 0)
      {
        throw std::runtime_error("Division by zero");
      }
      return left / right;
    }
    if (op == "%")
    {
      if (right == 0)
      {
        throw std::runtime_error("Modulo by zero");
      }
      long long result = left % right;
      if (result != 0 && ((left < 0 && right > 0) || (left > 0 && right < 0)))
      {
        result += right;
      }
      return result;
    }
    throw std::runtime_error("Unknown operator: " + op);
  }

  Queue< std::string > infixToPostfix(const std::string& expression)
  {
    std::istringstream iss(expression);
    std::string token;
    Stack< std::string > operators;
    Queue< std::string > output;

    while (iss >> token)
    {
      if (isNumber(token))
      {
        output.push(token);
      }
      else if (token == "(")
      {
        operators.push(token);
      }
      else if (token == ")")
      {
        while (!operators.empty() && operators.top() != "(")
        {
          output.push(operators.pop());
        }
        if (operators.empty())
        {
          throw std::runtime_error("Mismatched parentheses");
        }
        operators.pop();
      }
      else if (isOperator(token))
      {
        while (!operators.empty() && operators.top() != "(" &&
               getPriority(operators.top()) >= getPriority(token))
        {
          output.push(operators.pop());
        }
        operators.push(token);
      }
      else
      {
        throw std::runtime_error("Invalid token: " + token);
      }
    }

    while (!operators.empty())
    {
      if (operators.top() == "(")
      {
        throw std::runtime_error("Mismatched parentheses");
      }
      output.push(operators.pop());
    }

    return output;
  }

  long long evaluatePostfix(const Queue< std::string >& postfix)
  {
    Stack< long long > values;
    Queue< std::string > temp = postfix;

    while (!temp.empty())
    {
      std::string token = temp.pop();

      if (isNumber(token))
      {
        long long num = std::stoll(token);
        values.push(num);
      }
      else if (token == "#")
      {
        if (values.empty())
        {
          throw std::runtime_error("Not enough operands for unary operator");
        }
        long long operand = values.pop();
        values.push(reverseNumber(operand));
      }
      else if (isOperator(token))
      {
        if (values.size() < 2)
        {
          throw std::runtime_error("Not enough operands for binary operator");
        }
        long long right = values.pop();
        long long left = values.pop();
        values.push(applyBinaryOp(token, left, right));
      }
      else
      {
        throw std::runtime_error("Invalid token in postfix expression: " + token);
      }
    }

    if (values.size() != 1)
    {
      throw std::runtime_error("Invalid expression: stack size is not 1");
    }

    return values.pop();
  }

  long long evaluateExpression(const std::string& expression)
  {
    Queue< std::string > postfix = infixToPostfix(expression);
    return evaluatePostfix(postfix);
  }
}

