#include <iostream>
#include "functions.hpp"

int main()
{
  vishnyakov::List< vishnyakov::Sequence > seqs = vishnyakov::readInput(std::cin);

  if (seqs.empty())
  {
    std::cout << "0\n";
    return 0;
  }

  vishnyakov::outputNames(seqs, std::cout);
  bool res = vishnyakov::outputNums(seqs, std::cout);

  while (!seqs.empty())
  {
    seqs.pop_front();
  }

  return res;
}

