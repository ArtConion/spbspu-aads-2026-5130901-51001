#include <boost/test/unit_test.hpp>
#include <cmath>
#include "waypoint.hpp"

namespace vishnyakov
{
  BOOST_AUTO_TEST_SUITE(WaypointTests)

  BOOST_AUTO_TEST_CASE(ConstructorDefault)
  {
    Waypoint wp;
    BOOST_TEST(wp.x == 0);
    BOOST_TEST(wp.z == 0);
    BOOST_TEST(wp.type == "");
  }

  BOOST_AUTO_TEST_CASE(ConstructorWithParams)
  {
    Waypoint wp(100, 200, "home");
    BOOST_TEST(wp.x == 100);
    BOOST_TEST(wp.z == 200);
    BOOST_TEST(wp.type == "home");
  }

  BOOST_AUTO_TEST_CASE(ConstructorWithMove)
  {
    std::string type = "mine";
    Waypoint wp(250, 400, std::move(type));
    BOOST_TEST(wp.x == 250);
    BOOST_TEST(wp.z == 400);
    BOOST_TEST(wp.type == "mine");
  }

  BOOST_AUTO_TEST_CASE(Equality)
  {
    Waypoint a(10, 20, "a");
    Waypoint b(10, 20, "a");
    Waypoint c(30, 40, "b");
    BOOST_TEST(a == b);
    BOOST_TEST(a != c);
  }

  BOOST_AUTO_TEST_CASE(DistanceTo)
  {
    Waypoint wp(10, 20, "test");
    double dist = wp.distanceTo(13, 24);
    double expected = std::sqrt(3.0 * 3.0 + 4.0 * 4.0);
    BOOST_TEST(dist == expected);
  }

  BOOST_AUTO_TEST_CASE(OutputStream)
  {
    Waypoint wp(10, 20, "home");
    std::ostringstream out;
    out << wp;
    BOOST_TEST(out.str() == "(10, 20) type=home");
  }

  BOOST_AUTO_TEST_SUITE_END()
}

