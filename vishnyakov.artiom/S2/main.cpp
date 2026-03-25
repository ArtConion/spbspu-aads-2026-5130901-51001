#include "expression.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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

    std::vector< std::string > expressions;
    std::string line;

    while (std::getline(*input, line))
    {
      if (!line.empty())
      {
        expressions.push_back(line);
      }
    }

    if (expressions.empty())
    {
      return 0;
    }

    std::vector< long long > results;

    for (const std::string& expr : expressions)
    {
      results.push_back(evaluateExpression(expr));
    }

    for (size_t i = results.size(); i > 0; --i)
    {
      std::cout << results[i - 1];
      if (i > 1)
      {
        std::cout << " ";
      }
    }
    std::cout << std::endl;
  }
  catch (const std::overflow_error& e)
  {
    std::cerr << "Overflow error: " << e.what() << std::endl;
    return 1;
  }
  catch (const std::runtime_error& e)
  {
    std::cerr << "Runtime error: " << e.what() << std::endl;
    return 1;
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}

