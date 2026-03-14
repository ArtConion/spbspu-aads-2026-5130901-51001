#ifndef INPUT_HPP
#define INPUT_HPP

#include "list.hpp"
#include <iostream>
#include <cstring>
#include <limits>

namespace vishnyakov
{
  struct Sequence
  {
    std::string name;
    List< size_t >* nums;
  };

  List< Sequence > readInput(std::istream& in);

  void getWithoutSkips(std::istream& in);
  bool isEnd(std::istream& in);
  void skipLine(std::istream& in);
  bool checkedSum(size_t a, size_t b, size_t& res);
  void clearSequences(List< Sequence >& seqs);

  void outputNames(const List< Sequence >& seqs, std::ostream& out);
  bool outputNums(const List< Sequence >& seqs, std::ostream& out);
}

#endif

