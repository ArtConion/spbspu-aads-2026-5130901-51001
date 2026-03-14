#include "functions.hpp"
#include <iostream>

vishnyakov::List< vishnyakov::Sequence > vishnyakov::readInput(std::istream& in)
{
  List< Sequence > seqs;
  std::string name;

  while (in >> name)
  {
    Sequence seq(name);

    getWithoutSkips(in);

    LIter< size_t > curr_num = seq.nums->begin();
    while (!isEnd(in))
    {
      size_t num = 0;
      if (in >> num)
      {
        curr_num = seq.nums->insert_after(curr_num, num);
      }
      else
      {
        in.clear();
        break;
      }
    }
    skipLine(in);

    if (seqs.empty())
    {
      seqs.push_front(std::move(seq));
    }
    else
    {
      LIter< Sequence > last = seqs.begin();
      while (last != seqs.end())
      {
        LIter< Sequence > next = last;
        ++next;
        if (next == seqs.end())
        {
          break;
        }
        last = next;
      }
      seqs.insert_after(last, std::move(seq));
    }
  }
  return seqs;
}

void vishnyakov::getWithoutSkips(std::istream& in)
{
  while (in.peek() == ' ' || in.peek() == '\t')
  {
    in.get();
  }
}

bool vishnyakov::isEnd(std::istream& in)
{
  int c = in.peek();
  return c == '\n' || c == '\r' || c == std::istream::traits_type::eof();
}

void vishnyakov::skipLine(std::istream& in)
{
  while (!isEnd(in))
  {
    in.get();
  }
  if (in.peek() == '\r')
  {
    in.get();
  }
  if (in.peek() == '\n')
  {
    in.get();
  }
}

bool vishnyakov::checkedSum(size_t a, size_t b, size_t& res)
{
  if (b > std::numeric_limits< size_t >::max() - a)
  {
    return true;
  }
  res = a + b;
  return false;
}

void vishnyakov::outputNames(const List< Sequence >& seqs, std::ostream& out)
{
  LCIter< Sequence > it = seqs.cbegin();
  bool first = true;

  for (; it != seqs.cend(); ++it)
  {
    if (!first)
    {
      out << ' ';
    }
    out << it->name;
    first = false;
  }
  out << '\n';
}

int vishnyakov::outputNums(const List< Sequence >& seqs, std::ostream& out)
{
  if (seqs.empty())
  {
    return 0;
  }

  size_t max_len = 0;
  for (LCIter< Sequence > curr = seqs.cbegin(); curr != seqs.cend(); ++curr)
  {
    size_t len = 0;
    for (LCIter< size_t > cit = curr->nums->cbegin(); cit != curr->nums->cend(); ++cit)
    {
      ++len;
    }
    if (len > max_len)
    {
      max_len = len;
    }
  }

  List< List< size_t > > columns;
  bool has_nums = false;

  for (size_t col = 0; col < max_len; ++col)
  {
    List< size_t > column;
    bool col_has_nums = false;

    for (LCIter< Sequence > seq_it = seqs.cbegin(); seq_it != seqs.cend(); ++seq_it)
    {
      size_t index = 0;
      LCIter< size_t > num_it = seq_it->nums->cbegin();

      while (index < col && num_it != seq_it->nums->cend())
      {
        ++index;
        ++num_it;
      }

      if (num_it != seq_it->nums->cend())
      {
        column.push_back(*num_it);
        col_has_nums = true;
        has_nums = true;
      }
    }

    if (col_has_nums)
    {
      if (columns.empty())
      {
        columns.push_front(std::move(column));
      }
      else
      {
        LIter< List< size_t > > last = columns.begin();
        while (last != columns.end())
        {
          LIter< List< size_t > > next = last;
          ++next;
          if (next == columns.end())
          {
            break;
          }
          last = next;
        }
        columns.insert_after(last, std::move(column));
      }
    }
  }

  if (has_nums)
  {
    for (LCIter< List< size_t > > col_it = columns.cbegin(); col_it != columns.cend(); ++col_it)
    {
      bool first_in_col = true;
      for (LCIter< size_t > num_it = col_it->cbegin(); num_it != col_it->cend(); ++num_it)
      {
        if (!first_in_col)
        {
          out << ' ';
        }
        out << *num_it;
        first_in_col = false;
      }
      out << '\n';
    }
  }

  List< size_t > sums;
  bool overflow = false;

  for (LCIter< List< size_t > > col_it = columns.cbegin(); col_it != columns.cend(); ++col_it)
  {
    size_t sum = 0;
    for (LCIter< size_t > num_it = col_it->cbegin(); num_it != col_it->cend(); ++num_it)
    {
      if (checkedSum(sum, *num_it, sum))
      {
        std::cerr << "overflow";
        overflow = true;
        break;
      }
    }

    if (overflow)
    {
      break;
    }
    sums.push_back(sum);
  }

  if (overflow)
  {
    return 1;
  }

  if (sums.empty())
  {
    out << "0\n";
  }
  else
  {
    bool first_sum = true;
    for (LCIter< size_t > sum_it = sums.cbegin(); sum_it != sums.cend(); ++sum_it)
    {
      if (!first_sum)
      {
        out << ' ';
      }
      out << *sum_it;
      first_sum = false;
    }
    out << '\n';
  }

  return 0;
}

