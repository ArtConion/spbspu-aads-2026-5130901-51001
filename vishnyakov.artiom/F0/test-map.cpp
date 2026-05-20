#include <boost/test/unit_test.hpp>
#include "map.hpp"

namespace vishnyakov
{
  BOOST_AUTO_TEST_SUITE(MapTests)

  BOOST_AUTO_TEST_CASE(ConstructorDefault)
  {
    Map map;
    BOOST_TEST(map.getName() == "");
    BOOST_TEST(map.empty());
    BOOST_TEST(map.size() == 0);
  }

  BOOST_AUTO_TEST_CASE(ConstructorWithName)
  {
    Map map("Overworld");
    BOOST_TEST(map.getName() == "Overworld");
    BOOST_TEST(map.empty());
  }

  BOOST_AUTO_TEST_CASE(CopyConstructor)
  {
    Map map1("Test");
    map1.addWaypoint("home", 100, 64, "house");
    Map map2(map1);
    BOOST_TEST(map2.getName() == "Test");
    BOOST_TEST(map2.size() == 1);
    BOOST_TEST(map2.findWaypoint("home") != nullptr);
  }

  BOOST_AUTO_TEST_CASE(MoveConstructor)
  {
    Map map1("Test");
    map1.addWaypoint("home", 100, 64, "house");
    Map map2(std::move(map1));
    BOOST_TEST(map2.getName() == "Test");
    BOOST_TEST(map2.size() == 1);
  }

  BOOST_AUTO_TEST_CASE(CopyAssignment)
  {
    Map map1("Test");
    map1.addWaypoint("home", 100, 64, "house");
    Map map2;
    map2 = map1;
    BOOST_TEST(map2.getName() == "Test");
    BOOST_TEST(map2.size() == 1);
  }

  BOOST_AUTO_TEST_CASE(MoveAssignment)
  {
    Map map1("Test");
    map1.addWaypoint("home", 100, 64, "house");
    Map map2;
    map2 = std::move(map1);
    BOOST_TEST(map2.getName() == "Test");
    BOOST_TEST(map2.size() == 1);
  }

  BOOST_AUTO_TEST_CASE(AddAndFindWaypoint)
  {
    Map map("Overworld");
    map.addWaypoint("home", 100, 64, "house");
    map.addWaypoint("mine", 250, 30, "cave");

    BOOST_TEST(map.size() == 2);
    BOOST_TEST(!map.empty());

    const Waypoint* wp = map.findWaypoint("home");
    BOOST_REQUIRE(wp != nullptr);
    BOOST_TEST(wp->x == 100);
    BOOST_TEST(wp->z == 64);
    BOOST_TEST(wp->type == "house");

    wp = map.findWaypoint("mine");
    BOOST_REQUIRE(wp != nullptr);
    BOOST_TEST(wp->x == 250);
    BOOST_TEST(wp->z == 30);
    BOOST_TEST(wp->type == "cave");
  }

  BOOST_AUTO_TEST_CASE(FindNonExistentWaypoint)
  {
    Map map("Overworld");
    map.addWaypoint("home", 100, 64, "house");

    const Waypoint* wp = map.findWaypoint("nonexistent");
    BOOST_TEST(wp == nullptr);
  }

  BOOST_AUTO_TEST_CASE(RemoveWaypoint)
  {
    Map map("Overworld");
    map.addWaypoint("home", 100, 64, "house");
    map.addWaypoint("mine", 250, 30, "cave");

    BOOST_TEST(map.size() == 2);
    BOOST_TEST(map.removeWaypoint("home"));
    BOOST_TEST(map.size() == 1);
    BOOST_TEST(map.findWaypoint("home") == nullptr);
    BOOST_TEST(map.findWaypoint("mine") != nullptr);
  }

  BOOST_AUTO_TEST_CASE(RemoveNonExistentWaypoint)
  {
    Map map("Overworld");
    map.addWaypoint("home", 100, 64, "house");

    BOOST_TEST(!map.removeWaypoint("nonexistent"));
    BOOST_TEST(map.size() == 1);
  }

  BOOST_AUTO_TEST_CASE(Clear)
  {
    Map map("Overworld");
    map.addWaypoint("home", 100, 64, "house");
    map.addWaypoint("mine", 250, 30, "cave");
    map.addWaypoint("lake", 300, 50, "water");

    BOOST_TEST(map.size() == 3);
    map.clear();
    BOOST_TEST(map.empty());
    BOOST_TEST(map.size() == 0);
    BOOST_TEST(map.findWaypoint("home") == nullptr);
  }

  BOOST_AUTO_TEST_CASE(IteratorTraversal)
  {
    Map map("Overworld");
    map.addWaypoint("cave", 500, 300, "stone");
    map.addWaypoint("home", 100, 64, "house");
    map.addWaypoint("mine", 250, 30, "cave");

    std::vector< std::string > names;
    for (auto it = map.begin(); it != map.end(); ++it)
    {
      names.push_back(it->first);
    }

    std::vector< std::string > expected = {"cave", "home", "mine"};
    BOOST_TEST(names == expected);
  }

  BOOST_AUTO_TEST_CASE(FindByTypeWithIndex)
  {
    Map map("Overworld");
    map.addWaypoint("home", 100, 64, "house");
    map.addWaypoint("mine", 250, 30, "cave");
    map.addWaypoint("lake", 300, 50, "water");
    map.addWaypoint("castle", 500, 200, "house");

    std::ostringstream out;
    map.findByType("house", out);

    std::string result = out.str();
    BOOST_TEST(result.find("home 100 64 house") != std::string::npos);
    BOOST_TEST(result.find("castle 500 200 house") != std::string::npos);
    BOOST_TEST(result.find("mine") == std::string::npos);
    BOOST_TEST(result.find("lake") == std::string::npos);
  }

  BOOST_AUTO_TEST_CASE(FindByTypeEmpty)
  {
    Map map("Overworld");
    std::ostringstream out;
    map.findByType("house", out);
    BOOST_TEST(out.str() == "<EMPTY>\n");
  }

  BOOST_AUTO_TEST_SUITE_END()
}

