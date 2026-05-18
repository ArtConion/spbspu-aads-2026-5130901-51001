#include <boost/test/unit_test.hpp>
#include <sstream>
#include "commands.hpp"

namespace vishnyakov
{
  BOOST_AUTO_TEST_SUITE(CommandTests)

  BOOST_AUTO_TEST_CASE(CreateMapCommand)
  {
    World world;
    std::istringstream in("create-map Overworld\nlist-maps\nexit\n");
    std::ostringstream out;

    processCommands(in, world, out);

    BOOST_TEST(out.str() == "OK\nOverworld\n");
  }

  BOOST_AUTO_TEST_CASE(CreateDuplicateMap)
  {
    World world;
    std::istringstream in(
      "create-map Overworld\n"
      "create-map Overworld\n"
      "list-maps\nexit\n"
    );
    std::ostringstream out;

    processCommands(in, world, out);

    std::string expected = "OK\n<INVALID COMMAND>\nOverworld\n";
    BOOST_TEST(out.str() == expected);
  }

  BOOST_AUTO_TEST_CASE(DeleteMap)
  {
    World world;
    std::istringstream in(
      "create-map Overworld\n"
      "create-map Nether\n"
      "delete-map Overworld\n"
      "list-maps\nexit\n"
    );
    std::ostringstream out;

    processCommands(in, world, out);

    BOOST_TEST(out.str() == "OK\nOK\nOK\nNether\n");
  }

  BOOST_AUTO_TEST_CASE(DeleteNonExistentMap)
  {
    World world;
    std::istringstream in("delete-map Nonexistent\nexit\n");
    std::ostringstream out;

    processCommands(in, world, out);

    BOOST_TEST(out.str() == "<INVALID COMMAND>\n");
  }

  BOOST_AUTO_TEST_CASE(AddPoint)
  {
    World world;
    std::istringstream in(
      "create-map Overworld\n"
      "add-point Overworld home 100 64 house\n"
      "show-points Overworld\nexit\n"
    );
    std::ostringstream out;

    processCommands(in, world, out);

    std::string expected = "OK\nOK\nhome 100 64 house\n";
    BOOST_TEST(out.str() == expected);
  }

  BOOST_AUTO_TEST_CASE(AddPointToNonExistentMap)
  {
    World world;
    std::istringstream in("add-point Nonexistent home 100 64 house\nexit\n");
    std::ostringstream out;

    processCommands(in, world, out);

    BOOST_TEST(out.str() == "<INVALID COMMAND>\n");
  }

  BOOST_AUTO_TEST_CASE(AddDuplicatePoint)
  {
    World world;
    std::istringstream in(
      "create-map Overworld\n"
      "add-point Overworld home 100 64 house\n"
      "add-point Overworld home 200 30 mine\n"
      "show-points Overworld\nexit\n"
    );
    std::ostringstream out;

    processCommands(in, world, out);

    std::string expected = "OK\nOK\n<INVALID COMMAND>\nhome 100 64 house\n";
    BOOST_TEST(out.str() == expected);
  }

  BOOST_AUTO_TEST_CASE(RemovePoint)
  {
    World world;
    std::istringstream in(
      "create-map Overworld\n"
      "add-point Overworld home 100 64 house\n"
      "add-point Overworld mine 250 30 cave\n"
      "remove-point Overworld home\n"
      "show-points Overworld\nexit\n"
    );
    std::ostringstream out;

    processCommands(in, world, out);

    std::string expected = "OK\nOK\nOK\nOK\nmine 250 30 cave\n";
    BOOST_TEST(out.str() == expected);
  }

  BOOST_AUTO_TEST_CASE(RemoveNonExistentPoint)
  {
    World world;
    std::istringstream in(
      "create-map Overworld\n"
      "remove-point Overworld home\nexit\n"
    );
    std::ostringstream out;

    processCommands(in, world, out);

    BOOST_TEST(out.str() == "OK\n<INVALID COMMAND>\n");
  }

  BOOST_AUTO_TEST_CASE(ShowPointsEmpty)
  {
    World world;
    std::istringstream in(
      "create-map Overworld\n"
      "show-points Overworld\nexit\n"
    );
    std::ostringstream out;

    processCommands(in, world, out);

    BOOST_TEST(out.str() == "OK\n<EMPTY>\n");
  }

  BOOST_AUTO_TEST_CASE(ShowPointsNonExistentMap)
  {
    World world;
    std::istringstream in("show-points Nonexistent\nexit\n");
    std::ostringstream out;

    processCommands(in, world, out);

    BOOST_TEST(out.str() == "<INVALID COMMAND>\n");
  }

  BOOST_AUTO_TEST_CASE(EditPointCoordinates)
  {
    World world;
    std::istringstream in(
      "create-map Overworld\n"
      "add-point Overworld home 100 64 house\n"
      "edit-point Overworld home - 200 80 -\n"
      "show-points Overworld\nexit\n"
    );
    std::ostringstream out;

    processCommands(in, world, out);

    std::string expected = "OK\nOK\nOK\nhome 200 80 house\n";
    BOOST_TEST(out.str() == expected);
  }

  BOOST_AUTO_TEST_CASE(EditPointType)
  {
    World world;
    std::istringstream in(
      "create-map Overworld\n"
      "add-point Overworld home 100 64 house\n"
      "edit-point Overworld home - - - castle\n"
      "show-points Overworld\nexit\n"
    );
    std::ostringstream out;

    processCommands(in, world, out);

    std::string expected = "OK\nOK\nOK\nhome 100 64 castle\n";
    BOOST_TEST(out.str() == expected);
  }

  BOOST_AUTO_TEST_CASE(EditPointName)
  {
    World world;
    std::istringstream in(
      "create-map Overworld\n"
      "add-point Overworld home 100 64 house\n"
      "edit-point Overworld home castle - - -\n"
      "show-points Overworld\nexit\n"
    );
    std::ostringstream out;

    processCommands(in, world, out);

    std::string expected = "OK\nOK\nOK\ncastle 100 64 house\n";
    BOOST_TEST(out.str() == expected);
  }

  BOOST_AUTO_TEST_CASE(ClearMap)
  {
    World world;
    std::istringstream in(
      "create-map Overworld\n"
      "add-point Overworld home 100 64 house\n"
      "add-point Overworld mine 250 30 cave\n"
      "clear-map Overworld\n"
      "show-points Overworld\nexit\n"
    );
    std::ostringstream out;

    processCommands(in, world, out);

    std::string expected = "OK\nOK\nOK\nOK\n<EMPTY>\n";
    BOOST_TEST(out.str() == expected);
  }

  BOOST_AUTO_TEST_CASE(ClearNonExistentMap)
  {
    World world;
    std::istringstream in("clear-map Nonexistent\nexit\n");
    std::ostringstream out;

    processCommands(in, world, out);

    BOOST_TEST(out.str() == "<INVALID COMMAND>\n");
  }

  BOOST_AUTO_TEST_CASE(FindNearest)
  {
    World world;
    std::istringstream in(
      "create-map Overworld\n"
      "add-point Overworld home 100 64 house\n"
      "add-point Overworld mine 250 30 cave\n"
      "add-point Overworld lake 300 50 water\n"
      "find-nearest Overworld 110 70 2\nexit\n"
    );
    std::ostringstream out;

    processCommands(in, world, out);

    std::string result = out.str();
    BOOST_TEST(result.find("1. home") != std::string::npos);
    BOOST_TEST(result.find("coef: 100%") != std::string::npos);
    BOOST_TEST(result.find("2. mine") != std::string::npos);
  }

  BOOST_AUTO_TEST_CASE(FindNearestWithTypeFilter)
  {
    World world;
    std::istringstream in(
      "create-map Overworld\n"
      "add-point Overworld home 100 64 house\n"
      "add-point Overworld mine 250 30 cave\n"
      "add-point Overworld lake 300 50 water\n"
      "find-nearest Overworld 110 70 5 house\nexit\n"
    );
    std::ostringstream out;

    processCommands(in, world, out);

    std::string result = out.str();
    BOOST_TEST(result.find("home") != std::string::npos);
    BOOST_TEST(result.find("mine") == std::string::npos);
    BOOST_TEST(result.find("lake") == std::string::npos);
  }

  BOOST_AUTO_TEST_CASE(FindNearestEmpty)
  {
    World world;
    std::istringstream in(
      "create-map Overworld\n"
      "find-nearest Overworld 110 70 2\nexit\n"
    );
    std::ostringstream out;

    processCommands(in, world, out);

    BOOST_TEST(out.str() == "OK\n<EMPTY>\n");
  }

  BOOST_AUTO_TEST_CASE(FindNearestInvalidMap)
  {
    World world;
    std::istringstream in("find-nearest Nonexistent 110 70 2\nexit\n");
    std::ostringstream out;

    processCommands(in, world, out);

    BOOST_TEST(out.str() == "<INVALID COMMAND>\n");
  }

  BOOST_AUTO_TEST_CASE(InvalidCommand)
  {
    World world;
    std::istringstream in("unknown-command\nexit\n");
    std::ostringstream out;

    processCommands(in, world, out);

    BOOST_TEST(out.str() == "<INVALID COMMAND>\n");
  }

  BOOST_AUTO_TEST_CASE(EmptyLines)
  {
    World world;
    std::istringstream in("\n\ncreate-map Overworld\n\nlist-maps\n\nexit\n\n");
    std::ostringstream out;

    processCommands(in, world, out);

    BOOST_TEST(out.str() == "OK\nOverworld\n");
  }

  BOOST_AUTO_TEST_SUITE_END()
}

