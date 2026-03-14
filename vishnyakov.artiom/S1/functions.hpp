#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

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

    Sequence(): name(""), nums(nullptr) {}
    explicit Sequence(const std::string& name_): name(name_), nums(new List< size_t >()) {}

    Sequence(const Sequence&) = delete;
    Sequence& operator=(const Sequence&) = delete;

    Sequence(Sequence&& other) noexcept: name(std::move(other.name)), nums(other.nums)
    {
      other.nums = nullptr;
    }

    Sequence& operator=(Sequence&& other) noexcept
    {
      if (this != &other)
      {
        delete nums;
        name = std::move(other.name);
        nums = other.nums;
        other.nums = nullptr;
      }
      return *this;
    }

    ~Sequence()
    {
      delete nums;
    }
  };

  List< Sequence > readInput(std::istream& in);
  void getWithoutSkips(std::istream& in);
  bool isEnd(std::istream& in);
  void skipLine(std::istream& in);
  bool checkedSum(size_t a, size_t b, size_t& res);
  void outputNames(const List< Sequence >& seqs, std::ostream& out);
  int outputNums(const List< Sequence >& seqs, std::ostream& out);
}

#endif

