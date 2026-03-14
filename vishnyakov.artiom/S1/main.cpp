#include <iostream>
#include "functions.hpp"
#include "list.hpp"

int main()
{
  vishnyakov::List< vishnyakov::Sequence > seqs = vishnyakov::readInput(std::cin);

  if (seqs.empty())
  {
    std::cout << "0\n";
    return 0;
  }

  vishnyakov::ouputNames(seqs, std::cout);
  return vishnyakov::outputNums(seqs, std::cout);
}

