#include <boost/test/unit_test.hpp>
#include "vector.hpp"

namespace vishnyakov
{
  BOOST_AUTO_TEST_SUITE(VectorTests)

  BOOST_AUTO_TEST_CASE(ConstructorDefault)
  {
    Vector< int > v;
    BOOST_TEST(v.empty());
    BOOST_TEST(v.size() == 0);
    BOOST_TEST(v.capacity() == 0);
  }

  BOOST_AUTO_TEST_CASE(ConstructorWithCount)
  {
    Vector< int > v(5);
    BOOST_TEST(v.size() == 5);
    BOOST_TEST(v.capacity() >= 5);
    for (size_t i = 0; i < v.size(); ++i)
    {
      BOOST_TEST(v[i] == 0);
    }
  }

  BOOST_AUTO_TEST_CASE(ConstructorWithCountAndValue)
  {
    Vector< int > v(5, 42);
    BOOST_TEST(v.size() == 5);
    for (size_t i = 0; i < v.size(); ++i)
    {
      BOOST_TEST(v[i] == 42);
    }
  }

  BOOST_AUTO_TEST_CASE(CopyConstructor)
  {
    Vector< int > v1;
    v1.push_back(1);
    v1.push_back(2);
    v1.push_back(3);
    Vector< int > v2(v1);
    BOOST_TEST(v2.size() == 3);
    BOOST_TEST(v2[0] == 1);
    BOOST_TEST(v2[1] == 2);
    BOOST_TEST(v2[2] == 3);
  }

  BOOST_AUTO_TEST_CASE(MoveConstructor)
  {
    Vector< int > v1;
    v1.push_back(1);
    v1.push_back(2);
    Vector< int > v2(std::move(v1));
    BOOST_TEST(v2.size() == 2);
    BOOST_TEST(v2[0] == 1);
    BOOST_TEST(v2[1] == 2);
    BOOST_TEST(v1.empty());
  }

  BOOST_AUTO_TEST_CASE(CopyAssignment)
  {
    Vector< int > v1;
    v1.push_back(1);
    v1.push_back(2);
    Vector< int > v2;
    v2 = v1;
    BOOST_TEST(v2.size() == 2);
    BOOST_TEST(v2[0] == 1);
    BOOST_TEST(v2[1] == 2);
  }

  BOOST_AUTO_TEST_CASE(MoveAssignment)
  {
    Vector< int > v1;
    v1.push_back(1);
    v1.push_back(2);
    Vector< int > v2;
    v2 = std::move(v1);
    BOOST_TEST(v2.size() == 2);
    BOOST_TEST(v2[0] == 1);
    BOOST_TEST(v2[1] == 2);
    BOOST_TEST(v1.empty());
  }

  BOOST_AUTO_TEST_CASE(PushBack)
  {
    Vector< int > v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    BOOST_TEST(v.size() == 3);
    BOOST_TEST(v[0] == 1);
    BOOST_TEST(v[1] == 2);
    BOOST_TEST(v[2] == 3);
  }

  BOOST_AUTO_TEST_CASE(PopBack)
  {
    Vector< int > v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.pop_back();
    BOOST_TEST(v.size() == 2);
    BOOST_TEST(v[0] == 1);
    BOOST_TEST(v[1] == 2);
  }

  BOOST_AUTO_TEST_CASE(Reserve)
  {
    Vector< int > v;
    v.reserve(100);
    BOOST_TEST(v.capacity() >= 100);
    BOOST_TEST(v.size() == 0);
  }

  BOOST_AUTO_TEST_CASE(ShrinkToFit)
  {
    Vector< int > v;
    v.reserve(100);
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.shrink_to_fit();
    BOOST_TEST(v.capacity() == 3);
    BOOST_TEST(v.size() == 3);
  }

  BOOST_AUTO_TEST_CASE(At)
  {
    Vector< int > v;
    v.push_back(1);
    v.push_back(2);
    BOOST_TEST(v.at(0) == 1);
    BOOST_TEST(v.at(1) == 2);
    BOOST_CHECK_THROW(v.at(2), std::out_of_range);
  }

  BOOST_AUTO_TEST_CASE(FrontBack)
  {
    Vector< int > v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    BOOST_TEST(v.front() == 1);
    BOOST_TEST(v.back() == 3);
  }

  BOOST_AUTO_TEST_CASE(Insert)
  {
    Vector< int > v;
    v.push_back(1);
    v.push_back(3);
    Vector< int >::iterator it = v.begin();
    ++it;
    v.insert(it, 2);
    BOOST_TEST(v.size() == 3);
    BOOST_TEST(v[0] == 1);
    BOOST_TEST(v[1] == 2);
    BOOST_TEST(v[2] == 3);
  }

  BOOST_AUTO_TEST_CASE(InsertCount)
  {
    Vector< int > v;
    v.push_back(1);
    v.push_back(4);
    Vector< int >::iterator it = v.begin();
    ++it;
    v.insert(it, 2, 42);
    BOOST_TEST(v.size() == 4);
    BOOST_TEST(v[0] == 1);
    BOOST_TEST(v[1] == 42);
    BOOST_TEST(v[2] == 42);
    BOOST_TEST(v[3] == 4);
  }

  BOOST_AUTO_TEST_CASE(Erase)
  {
    Vector< int > v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    Vector< int >::iterator it = v.begin();
    ++it;
    v.erase(it);
    BOOST_TEST(v.size() == 2);
    BOOST_TEST(v[0] == 1);
    BOOST_TEST(v[1] == 3);
  }

  BOOST_AUTO_TEST_CASE(EraseRange)
  {
    Vector< int > v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    Vector< int >::iterator first = v.begin();
    ++first;
    Vector< int >::iterator last = v.end();
    --last;
    v.erase(first, last);
    BOOST_TEST(v.size() == 2);
    BOOST_TEST(v[0] == 1);
    BOOST_TEST(v[1] == 4);
  }

  BOOST_AUTO_TEST_CASE(Clear)
  {
    Vector< int > v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.clear();
    BOOST_TEST(v.empty());
    BOOST_TEST(v.size() == 0);
  }

  BOOST_AUTO_TEST_CASE(Iterators)
  {
    Vector< int > v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    int sum = 0;
    for (Vector< int >::iterator it = v.begin(); it != v.end(); ++it)
    {
      sum += *it;
    }
    BOOST_TEST(sum == 6);
  }

  BOOST_AUTO_TEST_CASE(EqualityOperatorEqual)
  {
    Vector< int > v1;
    v1.push_back(1);
    v1.push_back(2);
    v1.push_back(3);

    Vector< int > v2;
    v2.push_back(1);
    v2.push_back(2);
    v2.push_back(3);

    BOOST_TEST(v1 == v2);
    BOOST_TEST(!(v1 != v2));
  }

  BOOST_AUTO_TEST_CASE(EqualityOperatorDifferentValues)
  {
    Vector< int > v1;
    v1.push_back(1);
    v1.push_back(2);
    v1.push_back(3);

    Vector< int > v2;
    v2.push_back(1);
    v2.push_back(2);
    v2.push_back(4);

    BOOST_TEST(v1 != v2);
    BOOST_TEST(!(v1 == v2));
  }

  BOOST_AUTO_TEST_CASE(EqualityOperatorDifferentSize)
  {
    Vector< int > v1;
    v1.push_back(1);
    v1.push_back(2);
    v1.push_back(3);

    Vector< int > v2;
    v2.push_back(1);
    v2.push_back(2);

    BOOST_TEST(v1 != v2);
    BOOST_TEST(!(v1 == v2));
  }

  BOOST_AUTO_TEST_CASE(EqualityOperatorBothEmpty)
  {
    Vector< int > v1;
    Vector< int > v2;
    BOOST_TEST(v1 == v2);
    BOOST_TEST(!(v1 != v2));
  }

  BOOST_AUTO_TEST_CASE(EqualityOperatorOneEmpty)
  {
    Vector< int > v1;
    v1.push_back(1);

    Vector< int > v2;

    BOOST_TEST(v1 != v2);
    BOOST_TEST(!(v1 == v2));
  }

  BOOST_AUTO_TEST_CASE(EqualityOperatorWithStrings)
  {
    Vector< std::string > v1;
    v1.push_back("hello");
    v1.push_back("world");

    Vector< std::string > v2;
    v2.push_back("hello");
    v2.push_back("world");

    Vector< std::string > v3;
    v3.push_back("hello");
    v3.push_back("WORLD");

    BOOST_TEST(v1 == v2);
    BOOST_TEST(v1 != v3);
  }

  BOOST_AUTO_TEST_SUITE_END()
}

