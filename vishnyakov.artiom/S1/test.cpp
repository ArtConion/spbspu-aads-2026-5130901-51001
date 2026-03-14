#define BOOST_TEST_MODULE testLab
#include <boost/test/included/unit_test.hpp>
#include <sstream>
#include <string>
#include "list.hpp"
#include "functions.hpp"

using namespace vishnyakov;

BOOST_AUTO_TEST_SUITE(list_tests)

BOOST_AUTO_TEST_CASE(default_empty)
{
  List< int > lst;
  BOOST_TEST(lst.empty());
  BOOST_TEST(lst.size() == 0);
}

BOOST_AUTO_TEST_CASE(push_front)
{
  List< int > lst;
  lst.push_front(42);
  BOOST_TEST(!lst.empty());
  BOOST_TEST(lst.size() == 1);
  BOOST_TEST(lst.front() == 42);
}

BOOST_AUTO_TEST_CASE(push_back)
{
  List< int > lst;
  lst.push_back(1);
  lst.push_back(2);

  BOOST_TEST(lst.size() == 2);
  LIter< int > it = lst.begin();
  BOOST_TEST(*it == 1);
  ++it;
  BOOST_TEST(*it == 2);
}

BOOST_AUTO_TEST_CASE(pop_front)
{
  List< int > lst;
  lst.push_back(1);
  lst.push_back(2);

  lst.pop_front();
  BOOST_TEST(lst.size() == 1);
  BOOST_TEST(lst.front() == 2);
}

BOOST_AUTO_TEST_CASE(pop_back)
{
  List< int > lst;
  lst.push_back(1);
  lst.push_back(2);

  lst.pop_back();
  BOOST_TEST(lst.size() == 1);
  BOOST_TEST(lst.front() == 1);
}

BOOST_AUTO_TEST_CASE(insert_after)
{
  List< int > lst;
  lst.push_back(1);
  lst.push_back(3);

  LIter< int > pos = lst.begin();
  lst.insert_after(pos, 2);

  LIter< int > it = lst.begin();
  BOOST_TEST(*it == 1);
  ++it;
  BOOST_TEST(*it == 2);
  ++it;
  BOOST_TEST(*it == 3);
}

BOOST_AUTO_TEST_CASE(erase_after)
{
  List< int > lst;
  lst.push_back(1);
  lst.push_back(2);
  lst.push_back(3);

  LIter< int > pos = lst.begin();
  lst.erase_after(pos);

  LIter< int > it = lst.begin();
  BOOST_TEST(*it == 1);
  ++it;
  BOOST_TEST(*it == 3);
}

BOOST_AUTO_TEST_CASE(clear)
{
  List< int > lst;
  lst.push_back(1);
  lst.push_back(2);
  lst.push_back(3);

  lst.clear();
  BOOST_TEST(lst.empty());
  BOOST_TEST(lst.size() == 0);
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  List< int > a;
  a.push_back(10);
  a.push_back(20);

  List< int > b(a);
  BOOST_TEST(b.size() == 2);
  BOOST_TEST(b.front() == 10);
  BOOST_TEST(b.back() == 20);
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
  List< int > a;
  a.push_back(42);

  List< int > b(std::move(a));
  BOOST_TEST(b.size() == 1);
  BOOST_TEST(b.front() == 42);
  BOOST_TEST(a.empty());
}

BOOST_AUTO_TEST_CASE(iterator_increment)
{
  List< int > lst;
  lst.push_back(1);
  lst.push_back(2);

  LIter< int > it = lst.begin();
  BOOST_TEST(*it == 1);
  ++it;
  BOOST_TEST(*it == 2);
  ++it;
  BOOST_CHECK(it == lst.end());
}

BOOST_AUTO_TEST_CASE(const_iterator_conversion)
{
  List< int > lst;
  lst.push_back(42);

  LCIter< int > cit = lst.cbegin();

  BOOST_TEST(*cit == 42);

  BOOST_CHECK(cit != lst.cend());

  ++cit;
  BOOST_CHECK(cit == lst.cend());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(function_tests)

BOOST_AUTO_TEST_CASE(test_isEnd_newline)
{
  std::istringstream ss("\n");
  BOOST_TEST(isEnd(ss));
}

BOOST_AUTO_TEST_CASE(test_isEnd_eof)
{
  std::istringstream ss("");
  ss.setstate(std::ios::eofbit);
  BOOST_TEST(isEnd(ss));
}

BOOST_AUTO_TEST_CASE(test_isEnd_not_end)
{
  std::istringstream ss("42");
  BOOST_TEST(!isEnd(ss));
}

BOOST_AUTO_TEST_CASE(test_getWithoutSkips)
{
  std::istringstream ss("  \t42");
  getWithoutSkips(ss);
  BOOST_TEST(ss.peek() == '4');
}

BOOST_AUTO_TEST_CASE(test_skipLine)
{
  std::istringstream ss("line1\nline2");
  skipLine(ss);
  BOOST_TEST(ss.peek() == 'l');
}

BOOST_AUTO_TEST_CASE(test_checkedSum_no_overflow)
{
  size_t res = 0;
  bool overflow = checkedSum(5, 10, res);
  BOOST_TEST(!overflow);
  BOOST_TEST(res == 15);
}

BOOST_AUTO_TEST_CASE(test_checkedSum_overflow)
{
  size_t max = std::numeric_limits< size_t >::max();
  size_t res = 0;
  bool overflow = checkedSum(max, 1, res);
  BOOST_TEST(overflow);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(integration_tests)

BOOST_AUTO_TEST_CASE(test_readInput_single_sequence)
{
  std::istringstream ss("seq1 1 2 3\n");
  List< Sequence > seqs = readInput(ss);

  BOOST_TEST(seqs.size() == 1);

  LCIter< Sequence > it = seqs.cbegin();
  BOOST_TEST(it->name == "seq1");

  LCIter< size_t > numIt = it->nums->cbegin();
  BOOST_TEST(*numIt == 1);
  ++numIt;
  BOOST_TEST(*numIt == 2);
  ++numIt;
  BOOST_TEST(*numIt == 3);
}

BOOST_AUTO_TEST_CASE(test_readInput_multiple_sequences)
{
  std::istringstream ss("seq1 1 2\nseq2 3 4 5\nseq3 6\n");
  List< Sequence > seqs = readInput(ss);

  BOOST_TEST(seqs.size() == 3);

  LCIter< Sequence > it = seqs.cbegin();
  BOOST_TEST(it->name == "seq1");
  BOOST_TEST(it->nums->front() == 1);

  ++it;
  BOOST_TEST(it->name == "seq2");
  BOOST_TEST(it->nums->front() == 3);

  ++it;
  BOOST_TEST(it->name == "seq3");
  BOOST_TEST(it->nums->front() == 6);
}

BOOST_AUTO_TEST_CASE(test_outputNames)
{
  List< Sequence > seqs;

  Sequence seq1;
  seq1.name = "first";
  seq1.nums = new List< size_t >();

  Sequence seq2;
  seq2.name = "second";
  seq2.nums = new List< size_t >();

  LIter< Sequence > pos = seqs.begin();
  pos = seqs.insert_after(pos, seq1);
  seqs.insert_after(pos, seq2);

  std::ostringstream out;
  outputNames(seqs, out);

  BOOST_TEST(out.str() == "first second\n");

  for (LIter< Sequence > it = seqs.begin(); it != seqs.end(); ++it)
  {
    delete it->nums;
  }
}

BOOST_AUTO_TEST_CASE(test_outputNums)
{
  List< Sequence > seqs;

  Sequence seq1;
  seq1.name = "a";
  seq1.nums = new List< size_t >();
  seq1.nums->push_back(1);
  seq1.nums->push_back(2);

  Sequence seq2;
  seq2.name = "b";
  seq2.nums = new List< size_t >();
  seq2.nums->push_back(3);

  LIter< Sequence > pos = seqs.begin();
  pos = seqs.insert_after(pos, seq1);
  seqs.insert_after(pos, seq2);

  std::ostringstream out;
  int result = outputNums(seqs, out);

  BOOST_TEST(result == 0);
  BOOST_TEST(!out.str().empty());

  for (LIter< Sequence > it = seqs.begin(); it != seqs.end(); ++it)
  {
    delete it->nums;
  }
}

BOOST_AUTO_TEST_SUITE_END()

