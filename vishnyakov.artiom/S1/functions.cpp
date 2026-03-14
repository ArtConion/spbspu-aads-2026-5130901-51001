#include "functions.hpp"

vishnyakov::List< vishnyakov::Sequence > vishnyakov::readInput(std::istream& in)
{
  List< Sequence > seqs;
  LIter< Sequence > curr_seq = seqs.begin();
  std::string name;
  while (in >> name)
  {
    Sequence seq;
    seq.name = name;
    seq.nums = new List< size_t >();
    LIter< size_t > curr_num = seq.nums->begin();

    getWithoutSkips(in);
    while(!isEnd(in))
    {
      size_t num = 0;
      in >> num;
      curr_num = seq.nums->insert_after(curr_num, num);
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

void vishnyakov::clearSequences(List< Sequence >& seqs)
{
  while (!seqs.empty())
  {
    seqs.pop_front();
  }
}

void vishnyakov::outputNames(const List< Sequence >& seqs, std::ostream& out)
{
  LCIter< Sequence > it = seqs.cbegin();
  bool first = true;
  for(; it != seqs.cend(); ++it)
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

bool vishnyakov::outputNums(const List< Sequence >& seqs, std::ostream& out)
{
  size_t max_len = 0;

  for (LCIter< Sequence > curr = seqs.cbegin(); curr != seqs.cend(); ++curr)
  {
    size_t len = 0;
    for (LCIter< size_t > cit = curr->nums->cbegin(); cit != curr->nums->cend(); ++cit)
    {
      ++len;
    }
    max_len = max_len > len ? max_len : len;
  }

  List< List < size_t > > transposed;
  LIter< List < size_t > > last_transp = transposed.begin();
  bool hasNums = false;

  for (size_t i = 0; i < max_len; ++i)
  {
    List< size_t > new_seq;

    for(LCIter< Sequence > cit = seqs.cbegin(); cit != seqs.cend(); ++cit)
    {
      LCIter< size_t > it = cit->nums->cbegin();
      size_t curr = 0;
      while (curr < i && it != cit->nums->cend())
      {
        ++curr;
        ++it;
      }

      if (it != cit->nums->cend())
      {
        new_seq.push_back(*it);
        hasNums = true;
      }
    }

    if (new_seq.begin() != new_seq.end())
    {
      last_transp = transposed.insert_after(transposed.begin(), std::move(new_seq));
    }
  }

  if (hasNums)
  {
    for (LIter< List< size_t > > it = transposed.begin(); it != transposed.end(); ++it)
    {
      bool first = true;
      for (LCIter< size_t > cit = it->cbegin(); cit != it->cend(); ++cit)
      {
        if (!first)
        {
          out << ' ';
        }
        out << *cit;
        first = false;
      }
      out << '\n';
    }
  }

  List< size_t > sums;
  for (LCIter< List < size_t > > it = transposed.cbegin(); it != transposed.cend(); ++it)
  {
    size_t sum = 0;
    for (LCIter< size_t > cit = it->cbegin(); cit != it->cend(); ++cit)
    {
      if (checkedSum(sum, *cit, sum))
      {
        std::cerr << "overflow";
        return 1;
      }
    }
    sums.push_back(sum);
  }

  if (sums.empty())
  {
    out << "0\n";
    return 0;
  }

  bool first = true;
  for (auto it = sums.begin(); it != sums.end(); ++it)
  {
    if (!first)
    {
      out << ' ';
    }
    out << *it;
    first = false;
  }
  out << '\n';
  return 0;
}


