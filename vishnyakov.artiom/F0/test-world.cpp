#include <boost/test/unit_test.hpp>
#include <sstream>
#include "world.hpp"

namespace vishnyakov
{
  BOOST_AUTO_TEST_SUITE(WorldTests)

  BOOST_AUTO_TEST_CASE(ConstructorDefault)
  {
    World world;
    BOOST_TEST(world.empty());
    BOOST_TEST(world.mapCount() == 0);
  }

  BOOST_AUTO_TEST_CASE(CreateMap)
  {
    World world;
    BOOST_TEST(world.createMap("Overworld"));
    BOOST_TEST(world.hasMap("Overworld"));
    BOOST_TEST(world.mapCount() == 1);
  }

  BOOST_AUTO_TEST_CASE(CreateMapDuplicate)
  {
    World world;
    world.createMap("Overworld");
    BOOST_TEST(!world.createMap("Overworld"));
    BOOST_TEST(world.mapCount() == 1);
  }

  BOOST_AUTO_TEST_CASE(GetMap)
  {
    World world;
    world.createMap("Overworld");
    Map* map = world.getMap("Overworld");
    BOOST_REQUIRE(map != nullptr);
    BOOST_TEST(map->getName() == "Overworld");
    BOOST_TEST(map->empty());

    const World& constWorld = world;
    const Map* constMap = constWorld.getMap("Overworld");
    BOOST_REQUIRE(constMap != nullptr);
    BOOST_TEST(constMap->getName() == "Overworld");
  }

  BOOST_AUTO_TEST_CASE(GetNonExistentMap)
  {
    World world;
    world.createMap("Overworld");
    Map* map = world.getMap("Nonexistent");
    BOOST_TEST(map == nullptr);
  }

  BOOST_AUTO_TEST_CASE(HasMap)
  {
    World world;
    world.createMap("Overworld");
    BOOST_TEST(world.hasMap("Overworld"));
    BOOST_TEST(!world.hasMap("Nonexistent"));
  }

  BOOST_AUTO_TEST_CASE(DeleteMap)
  {
    World world;
    world.createMap("Overworld");
    world.createMap("Nether");
    BOOST_TEST(world.mapCount() == 2);
    BOOST_TEST(world.deleteMap("Overworld"));
    BOOST_TEST(world.mapCount() == 1);
    BOOST_TEST(!world.hasMap("Overworld"));
    BOOST_TEST(world.hasMap("Nether"));
  }

  BOOST_AUTO_TEST_CASE(DeleteNonExistentMap)
  {
    World world;
    world.createMap("Overworld");
    BOOST_TEST(!world.deleteMap("Nonexistent"));
    BOOST_TEST(world.mapCount() == 1);
  }

  BOOST_AUTO_TEST_CASE(ListMaps)
  {
    World world;
    world.createMap("Zoo");
    world.createMap("Alpha");
    world.createMap("Beta");

    std::ostringstream out;
    world.listMaps(out);

    std::string expected = "Zoo\nAlpha\nBeta\n";
    BOOST_TEST(out.str() == expected);
  }

  BOOST_AUTO_TEST_CASE(ListMapsEmpty)
  {
    World world;
    std::ostringstream out;
    world.listMaps(out);
    BOOST_TEST(out.str() == "<EMPTY>\n");
  }

  BOOST_AUTO_TEST_CASE(MergeMaps)
  {
    World world;
    world.createMap("First");
    world.createMap("Second");

    Map* first = world.getMap("First");
    Map* second = world.getMap("Second");

    first->addWaypoint("home", 100, 64, "house");
    first->addWaypoint("mine", 250, 30, "cave");
    second->addWaypoint("mine", 250, 30, "cave");
    second->addWaypoint("lake", 300, 50, "water");

    BOOST_TEST(world.mergeMaps("Merged", "First", "Second"));
    BOOST_TEST(world.hasMap("Merged"));

    const Map* merged = world.getMap("Merged");
    BOOST_REQUIRE(merged != nullptr);
    BOOST_TEST(merged->size() == 3);
    BOOST_TEST(merged->findWaypoint("home") != nullptr);
    BOOST_TEST(merged->findWaypoint("mine") != nullptr);
    BOOST_TEST(merged->findWaypoint("lake") != nullptr);
  }

  BOOST_AUTO_TEST_CASE(MergeMapsWithDuplicateNewName)
  {
    World world;
    world.createMap("First");
    world.createMap("Second");
    world.createMap("Existing");

    BOOST_TEST(!world.mergeMaps("Existing", "First", "Second"));
  }

  BOOST_AUTO_TEST_CASE(MergeMapsWithMissingSource)
  {
    World world;
    world.createMap("First");

    BOOST_TEST(!world.mergeMaps("Merged", "First", "Nonexistent"));
    BOOST_TEST(!world.hasMap("Merged"));
  }

  BOOST_AUTO_TEST_CASE(Clear)
  {
    World world;
    world.createMap("First");
    world.createMap("Second");
    world.createMap("Third");
    BOOST_TEST(world.mapCount() == 3);
    world.clear();
    BOOST_TEST(world.empty());
    BOOST_TEST(world.mapCount() == 0);
  }

  BOOST_AUTO_TEST_CASE(IteratorTraversal)
  {
    World world;
    world.createMap("C");
    world.createMap("A");
    world.createMap("B");

    std::vector< std::string > names;
    for (auto it = world.begin(); it != world.end(); ++it)
    {
      names.push_back(it->getName());
    }

    std::vector< std::string > expected = {"C", "A", "B"};
    BOOST_TEST(names == expected);
  }

  BOOST_AUTO_TEST_SUITE_END()
}

