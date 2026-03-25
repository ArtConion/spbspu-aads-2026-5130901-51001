#include "expression.hpp"
#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[])
{
  using namespace vishnyakov;

  try
  {
    std::istream* input = &std::cin;
    std::ifstream file;

    if (argc == 2)
    {
      file.open(argv[1]);
      if (!file.is_open())
      {
        std::cerr << "Error: Cannot open file " << argv[1] << std::endl;
        return 1;
      }
      input = &file;
    }
    else if (argc > 2)
    {
      std::cerr << "Error: Too many arguments" << std::endl;
      return 1;
    }

    Queue<std::string> expressions;
    std::string line;

    while (std::getline(*input, line))
    {
      if (line.empty())
      {
        continue;
      }
      expressions.push(line);
    }

    if (expressions.empty())
    {
      return 0;
    }

    Queue<long long> results;
    Queue<std::string> tempExpressions = expressions;

    while (!tempExpressions.empty())
    {
      std::string expr = tempExpressions.pop();
      results.push(evaluateExpression(expr));
    }

    bool first = true;
    Queue<long long> tempResults = results;

    while (!tempResults.empty())
    {
      if (!first)
      {
        std::cout << " ";
      }
      std::cout << tempResults.pop();
      first = false;
    }
    std::cout << std::endl;
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}

