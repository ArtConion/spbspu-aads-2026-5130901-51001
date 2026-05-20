#include <boost/test/unit_test.hpp>
#include <cmath>
#include "route.hpp"

namespace vishnyakov
{
  BOOST_AUTO_TEST_SUITE(RouteTests)

  BOOST_AUTO_TEST_CASE(Interpolate)
  {
    auto coords = interpolate(0, 0, 100, 100, 0.5);
    BOOST_TEST(coords.first == 50);
    BOOST_TEST(coords.second == 50);

    coords = interpolate(0, 0, 100, 200, 0.3);
    BOOST_TEST(coords.first == 30);
    BOOST_TEST(coords.second == 60);
  }

  BOOST_AUTO_TEST_CASE(TraverseSegmentShortDay)
  {
    double startTime = 5.0;
    auto result = traverseSegment("A", 0, 0, "B", 100, 0, startTime, true);

    BOOST_TEST(result.endTime > 5.29);
    BOOST_TEST(result.endTime < 5.30);
    BOOST_TEST(result.totalNightTime == 0.0);
    BOOST_TEST(result.stops.size() == 1);
    BOOST_TEST(result.stops.cbegin()->name == "B");
    BOOST_TEST(!result.stops.cbegin()->isNightStop);
  }

  BOOST_AUTO_TEST_CASE(TraverseSegmentWithNight)
  {
    double startTime = 9.5;
    auto result = traverseSegment("A", 0, 0, "B", 500, 0, startTime, true);

    BOOST_TEST(result.totalNightTime > 9.99);
    BOOST_TEST(result.totalNightTime < 10.01);
    BOOST_TEST(result.endTime > 0.98);
    BOOST_TEST(result.endTime < 1.00);
  }

  BOOST_AUTO_TEST_CASE(TraverseSegmentStartAtNight)
  {
    double startTime = 15.0;
    auto result = traverseSegment("A", 0, 0, "B", 100, 0, startTime, true);

    BOOST_TEST(result.totalNightTime == 5.0);
    BOOST_TEST(result.stops.size() >= 1);
    BOOST_TEST(result.stops.cbegin()->isNightStop);
  }

  BOOST_AUTO_TEST_CASE(BuildGreedyRouteSimple)
  {
    List< std::pair< std::string, Waypoint > > points;
    points.push_back(std::make_pair("home", Waypoint(100, 0, "house")));
    points.push_back(std::make_pair("mine", Waypoint(200, 0, "cave")));

    auto result = buildGreedyRoute(points, 0, 0, 5.0);

    BOOST_TEST(result.totalDistance == 200.0);
    BOOST_TEST(result.totalTime >= 5.0 + 200.0 / 336.0);
    BOOST_TEST(result.totalTime < 5.0 + 200.0 / 336.0 + 0.01);

    int stopCount = 0;
    for (auto it = result.allStops.cbegin(); it != result.allStops.cend(); ++it)
    {
      ++stopCount;
    }
    BOOST_TEST(stopCount == 3);

    auto it = result.allStops.cbegin();
    BOOST_TEST(it->name == "start");
    ++it;
    BOOST_TEST(it->name == "home");
    ++it;
    BOOST_TEST(it->name == "mine");
  }

  BOOST_AUTO_TEST_CASE(BuildGreedyRouteEmpty)
  {
    List< std::pair< std::string, Waypoint > > points;
    auto result = buildGreedyRoute(points, 0, 0, 5.0);
    BOOST_TEST(result.totalDistance == 0.0);
    BOOST_TEST(result.totalTime == 0.0);
    BOOST_TEST(result.allStops.empty());
  }

  BOOST_AUTO_TEST_CASE(GreedyRouteOrder)
  {
    List< std::pair< std::string, Waypoint > > points;
    points.push_back(std::make_pair("far", Waypoint(500, 0, "far")));
    points.push_back(std::make_pair("near", Waypoint(100, 0, "near")));
    points.push_back(std::make_pair("mid", Waypoint(250, 0, "mid")));

    auto result = buildGreedyRoute(points, 0, 0, 5.0);

    auto it = result.allStops.cbegin();
    ++it;
    BOOST_TEST(it->name == "near");
    ++it;
    BOOST_TEST(it->name == "mid");
    ++it;
    BOOST_TEST(it->name == "far");
  }

  BOOST_AUTO_TEST_CASE(HungerAndBread)
  {
    List< std::pair< std::string, Waypoint > > points;
    points.push_back(std::make_pair("home", Waypoint(200, 0, "house")));

    auto result = buildGreedyRoute(points, 0, 0, 5.0);

    BOOST_TEST(result.totalHunger == 5.0);
    BOOST_TEST(result.breadNeeded == 1);
  }

  BOOST_AUTO_TEST_CASE(HungerAndBreadRoundUp)
  {
    List< std::pair< std::string, Waypoint > > points;
    points.push_back(std::make_pair("home", Waypoint(210, 0, "house")));

    auto result = buildGreedyRoute(points, 0, 0, 5.0);

    BOOST_TEST(result.totalHunger == 5.25);
    BOOST_TEST(result.breadNeeded == 2);
  }

  BOOST_AUTO_TEST_SUITE_END()
}

