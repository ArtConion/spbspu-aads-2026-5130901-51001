#include <boost/test/unit_test.hpp>
#include "avltree.hpp"

namespace vishnyakov
{
  BOOST_AUTO_TEST_SUITE(AVLTreeTests)

  BOOST_AUTO_TEST_CASE(ConstructorDefault)
  {
    AVLTree< int, std::string, std::less< int > > tree;
    BOOST_TEST(tree.empty());
    BOOST_TEST(tree.size() == 0);
  }

  BOOST_AUTO_TEST_SUITE_END()
}

