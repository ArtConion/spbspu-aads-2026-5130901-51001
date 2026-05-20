#include <boost/test/unit_test.hpp>
#include <sstream>
#include <fstream>
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

    BOOST_TEST(out.str() == "# Карта \"Overworld\" создана\nOverworld\n");
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

    std::string expected = "# Карта \"Overworld\" создана\n"
                           "Wrong usage. Use:\n"
                           "  create-map <map-name>\n"
                           "Overworld\n";
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

    std::string expected = "# Карта \"Overworld\" создана\n"
                           "# Карта \"Nether\" создана\n"
                           "# Карта \"Overworld\" удалена\n"
                           "Nether\n";
    BOOST_TEST(out.str() == expected);
  }

  BOOST_AUTO_TEST_CASE(DeleteNonExistentMap)
  {
    World world;
    std::istringstream in("delete-map Nonexistent\nexit\n");
    std::ostringstream out;

    processCommands(in, world, out);

    std::string expected = "Wrong usage. Use:\n"
                           "  delete-map <map-name>\n";
    BOOST_TEST(out.str() == expected);
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

    std::string expected = "# Карта \"Overworld\" создана\n"
                           "# Точка \"home\" добавлена на карту \"Overworld\"\n"
                           "home 100 64 house\n";
    BOOST_TEST(out.str() == expected);
  }

  BOOST_AUTO_TEST_CASE(AddPointToNonExistentMap)
  {
    World world;
    std::istringstream in("add-point Nonexistent home 100 64 house\nexit\n");
    std::ostringstream out;

    processCommands(in, world, out);

    std::string expected = "Wrong usage. Use:\n"
                           "  add-point <map-name> <point-name> <x> <z> <type>\n";
    BOOST_TEST(out.str() == expected);
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

    std::string expected = "# Карта \"Overworld\" создана\n"
                           "# Точка \"home\" добавлена на карту \"Overworld\"\n"
                           "Wrong usage. Use:\n"
                           "  add-point <map-name> <point-name> <x> <z> <type>\n"
                           "home 100 64 house\n";
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

    std::string expected = "# Карта \"Overworld\" создана\n"
                           "# Точка \"home\" добавлена на карту \"Overworld\"\n"
                           "# Точка \"mine\" добавлена на карту \"Overworld\"\n"
                           "# Точка \"home\" удалена с карты \"Overworld\"\n"
                           "mine 250 30 cave\n";
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

    std::string expected = "# Карта \"Overworld\" создана\n"
                           "Wrong usage. Use:\n"
                           "  remove-point <map-name> <point-name>\n";
    BOOST_TEST(out.str() == expected);
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

    std::string expected = "# Карта \"Overworld\" создана\n"
                           "<EMPTY>\n";
    BOOST_TEST(out.str() == expected);
  }

  BOOST_AUTO_TEST_CASE(ShowPointsNonExistentMap)
  {
    World world;
    std::istringstream in("show-points Nonexistent\nexit\n");
    std::ostringstream out;

    processCommands(in, world, out);

    std::string expected = "Wrong usage. Use:\n"
                           "  show-points <map-name>\n";
    BOOST_TEST(out.str() == expected);
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

    std::string expected = "# Карта \"Overworld\" создана\n"
                           "# Точка \"home\" добавлена на карту \"Overworld\"\n"
                           "# Точка \"home\" изменена на карте \"Overworld\"\n"
                           "home 200 80 house\n";
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

    std::string expected = "# Карта \"Overworld\" создана\n"
                           "# Точка \"home\" добавлена на карту \"Overworld\"\n"
                           "# Точка \"home\" изменена на карте \"Overworld\"\n"
                           "home 100 64 castle\n";
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

    std::string expected = "# Карта \"Overworld\" создана\n"
                           "# Точка \"home\" добавлена на карту \"Overworld\"\n"
                           "# Точка \"home\" изменена на карте \"Overworld\"\n"
                           "castle 100 64 house\n";
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

    std::string expected = "# Карта \"Overworld\" создана\n"
                           "# Точка \"home\" добавлена на карту \"Overworld\"\n"
                           "# Точка \"mine\" добавлена на карту \"Overworld\"\n"
                           "# Карта \"Overworld\" очищена\n"
                           "<EMPTY>\n";
    BOOST_TEST(out.str() == expected);
  }

  BOOST_AUTO_TEST_CASE(ClearNonExistentMap)
  {
    World world;
    std::istringstream in("clear-map Nonexistent\nexit\n");
    std::ostringstream out;

    processCommands(in, world, out);

    std::string expected = "Wrong usage. Use:\n"
                           "  clear-map <map-name>\n";
    BOOST_TEST(out.str() == expected);
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
    size_t resultPos = result.find("1. ");
    BOOST_REQUIRE(resultPos != std::string::npos);
    std::string afterResult = result.substr(resultPos);

    BOOST_TEST(afterResult.find("home") != std::string::npos);
    BOOST_TEST(afterResult.find("mine") == std::string::npos);
    BOOST_TEST(afterResult.find("lake") == std::string::npos);
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

    std::string expected = "# Карта \"Overworld\" создана\n"
                           "<EMPTY>\n";
    BOOST_TEST(out.str() == expected);
  }

  BOOST_AUTO_TEST_CASE(FindNearestInvalidMap)
  {
    World world;
    std::istringstream in("find-nearest Nonexistent 110 70 2\nexit\n");
    std::ostringstream out;

    processCommands(in, world, out);

    std::string expected = "Wrong usage. Use:\n"
                           "  find-nearest <map-name> <x> <z> <k> [type]\n";
    BOOST_TEST(out.str() == expected);
  }

  BOOST_AUTO_TEST_CASE(FindByType)
  {
    World world;
    std::istringstream in(
      "create-map Overworld\n"
      "add-point Overworld home 100 64 house\n"
      "add-point Overworld mine 250 30 cave\n"
      "add-point Overworld lake 300 50 water\n"
      "add-point Overworld castle 500 200 house\n"
      "find-by-type Overworld house\nexit\n"
    );
    std::ostringstream out;

    processCommands(in, world, out);

    std::string result = out.str();
    size_t firstHousePos = result.find("home 100 64 house");
    if (firstHousePos == std::string::npos)
    {
      firstHousePos = result.find("castle 500 200 house");
    }
    BOOST_REQUIRE(firstHousePos != std::string::npos);
    std::string afterFirstPoint = result.substr(firstHousePos);

    BOOST_TEST(afterFirstPoint.find("home 100 64 house") != std::string::npos);
    BOOST_TEST(afterFirstPoint.find("castle 500 200 house") != std::string::npos);
    BOOST_TEST(afterFirstPoint.find("mine") == std::string::npos);
    BOOST_TEST(afterFirstPoint.find("lake") == std::string::npos);
  }

  BOOST_AUTO_TEST_CASE(FindByTypeEmpty)
  {
    World world;
    std::istringstream in(
      "create-map Overworld\n"
      "find-by-type Overworld house\nexit\n"
    );
    std::ostringstream out;

    processCommands(in, world, out);

    std::string expected = "# Карта \"Overworld\" создана\n"
                           "<EMPTY>\n";
    BOOST_TEST(out.str() == expected);
  }

  BOOST_AUTO_TEST_CASE(CopyPoint)
  {
    World world;
    std::istringstream in(
      "create-map First\n"
      "create-map Second\n"
      "add-point First home 100 64 house\n"
      "copy-point First Second home\n"
      "show-points Second\nexit\n"
    );
    std::ostringstream out;

    processCommands(in, world, out);

    std::string expected = "# Карта \"First\" создана\n"
                           "# Карта \"Second\" создана\n"
                           "# Точка \"home\" добавлена на карту \"First\"\n"
                           "# Точка \"home\" скопирована с карты \"First\" на карту \"Second\"\n"
                           "home 100 64 house\n";
    BOOST_TEST(out.str() == expected);
  }

  BOOST_AUTO_TEST_CASE(CopyPointAlreadyExists)
  {
    World world;
    std::istringstream in(
      "create-map First\n"
      "create-map Second\n"
      "add-point First home 100 64 house\n"
      "add-point Second home 200 30 cave\n"
      "copy-point First Second home\nexit\n"
    );
    std::ostringstream out;

    processCommands(in, world, out);

    std::string expected = "# Карта \"First\" создана\n"
                           "# Карта \"Second\" создана\n"
                           "# Точка \"home\" добавлена на карту \"First\"\n"
                           "# Точка \"home\" добавлена на карту \"Second\"\n"
                           "Wrong usage. Use:\n"
                           "  copy-point <src-map> <dst-map> <point-name>\n";
    BOOST_TEST(out.str() == expected);
  }

  BOOST_AUTO_TEST_CASE(MovePoint)
  {
    World world;
    std::istringstream in(
      "create-map First\n"
      "create-map Second\n"
      "add-point First home 100 64 house\n"
      "move-point First Second home\n"
      "show-points First\n"
      "show-points Second\nexit\n"
    );
    std::ostringstream out;

    processCommands(in, world, out);

    std::string expected = "# Карта \"First\" создана\n"
                           "# Карта \"Second\" создана\n"
                           "# Точка \"home\" добавлена на карту \"First\"\n"
                           "# Точка \"home\" перемещена с карты \"First\" на карту \"Second\"\n"
                           "<EMPTY>\n"
                           "home 100 64 house\n";
    BOOST_TEST(out.str() == expected);
  }

  BOOST_AUTO_TEST_CASE(MergeMapsCommand)
  {
    World world;
    std::istringstream in(
      "create-map First\n"
      "create-map Second\n"
      "add-point First home 100 64 house\n"
      "add-point First mine 250 30 cave\n"
      "add-point Second lake 300 50 water\n"
      "merge-maps Merged First Second\n"
      "show-points Merged\nexit\n"
    );
    std::ostringstream out;

    processCommands(in, world, out);

    std::string result = out.str();
    BOOST_TEST(result.find("# Карты \"First\" и \"Second\" объединены в \"Merged\"") != std::string::npos);
    BOOST_TEST(result.find("home 100 64 house") != std::string::npos);
    BOOST_TEST(result.find("mine 250 30 cave") != std::string::npos);
    BOOST_TEST(result.find("lake 300 50 water") != std::string::npos);
  }

  BOOST_AUTO_TEST_CASE(MergeMapsInvalid)
  {
    World world;
    std::istringstream in(
      "create-map First\n"
      "merge-maps Merged First Nonexistent\nexit\n"
    );
    std::ostringstream out;

    processCommands(in, world, out);

    std::string expected = "# Карта \"First\" создана\n"
                           "Wrong usage. Use:\n"
                           "  merge-maps <new-map-name> <map-name-1> <map-name-2>\n";
    BOOST_TEST(out.str() == expected);
  }

  BOOST_AUTO_TEST_CASE(SaveAndLoad)
  {
    World world;
    std::istringstream in(
      "create-map First\n"
      "add-point First home 100 64 house\n"
      "add-point First mine 250 30 cave\n"
      "save test_save.txt\n"
      "create-map Second\n"
      "load test_save.txt\n"
      "list-maps\nexit\n"
    );
    std::ostringstream out;

    processCommands(in, world, out);

    std::string result = out.str();
    BOOST_TEST(result.find("# Карта \"First\" создана") != std::string::npos);
    BOOST_TEST(result.find("# Данные сохранены в файл \"test_save.txt\"") != std::string::npos);
    BOOST_TEST(result.find("# Карта \"Second\" создана") != std::string::npos);
    BOOST_TEST(result.find("# Данные загружены из файла \"test_save.txt\"") != std::string::npos);
    BOOST_TEST(result.find("First") != std::string::npos);
    BOOST_TEST(result.find("Second\n") == std::string::npos);

    std::remove("test_save.txt");
  }

  BOOST_AUTO_TEST_CASE(LoadNonExistentFile)
  {
    World world;
    std::istringstream in("load nonexistent.txt\nexit\n");
    std::ostringstream out;

    processCommands(in, world, out);

    std::string expected = "Wrong usage. Use:\n"
                           "  load <filename>\n";
    BOOST_TEST(out.str() == expected);
  }

  BOOST_AUTO_TEST_CASE(GreedyRoute)
  {
    World world;
    std::istringstream in(
      "create-map Test\n"
      "add-point Test home 100 64 house\n"
      "add-point Test mine 250 30 cave\n"
      "add-point Test lake 500 200 water\n"
      "plan-route-greedy Test 0 0 5 0\nexit\n"
    );
    std::ostringstream out;

    processCommands(in, world, out);

    std::string result = out.str();
    BOOST_TEST(result.find("Маршрут (greedy):") != std::string::npos);
    BOOST_TEST(result.find("Старт") != std::string::npos);
    BOOST_TEST(result.find("home") != std::string::npos);
    BOOST_TEST(result.find("mine") != std::string::npos);
    BOOST_TEST(result.find("lake") != std::string::npos);
    BOOST_TEST(result.find("Затраченное время:") != std::string::npos);
    BOOST_TEST(result.find("Пройденная дистанция:") != std::string::npos);
    BOOST_TEST(result.find("Текущее время:") != std::string::npos);
    BOOST_TEST(result.find("Общая длина:") != std::string::npos);
    BOOST_TEST(result.find("Общее время:") != std::string::npos);
    BOOST_TEST(result.find("Потрачено голода:") != std::string::npos);
    BOOST_TEST(result.find("Хлеба нужно:") != std::string::npos);
  }

  BOOST_AUTO_TEST_CASE(GreedyRoute2Opt)
  {
    World world;
    std::istringstream in(
      "create-map Test\n"
      "add-point Test home 100 64 house\n"
      "add-point Test mine 250 30 cave\n"
      "add-point Test lake 500 200 water\n"
      "plan-route-2opt Test 0 0 5 0\nexit\n"
    );
    std::ostringstream out;

    processCommands(in, world, out);

    std::string result = out.str();
    BOOST_TEST(result.find("Маршрут (2-opt):") != std::string::npos);
    BOOST_TEST(result.find("Старт") != std::string::npos);
    BOOST_TEST(result.find("home") != std::string::npos);
    BOOST_TEST(result.find("mine") != std::string::npos);
    BOOST_TEST(result.find("lake") != std::string::npos);
  }

  BOOST_AUTO_TEST_CASE(GreedyRouteMST)
  {
    World world;
    std::istringstream in(
      "create-map Test\n"
      "add-point Test home 100 64 house\n"
      "add-point Test mine 250 30 cave\n"
      "add-point Test lake 500 200 water\n"
      "plan-route-mst Test 0 0 5 0\nexit\n"
    );
    std::ostringstream out;

    processCommands(in, world, out);

    std::string result = out.str();
    BOOST_TEST(result.find("Маршрут (MST):") != std::string::npos);
    BOOST_TEST(result.find("Старт") != std::string::npos);
    BOOST_TEST(result.find("home") != std::string::npos);
    BOOST_TEST(result.find("mine") != std::string::npos);
    BOOST_TEST(result.find("lake") != std::string::npos);
  }

  BOOST_AUTO_TEST_CASE(GreedyRouteAnt)
  {
    World world;
    std::istringstream in(
      "create-map Test\n"
      "add-point Test home 100 64 house\n"
      "add-point Test mine 250 30 cave\n"
      "add-point Test lake 500 200 water\n"
      "plan-route-ant Test 0 0 5 0\nexit\n"
    );
    std::ostringstream out;

    processCommands(in, world, out);

    std::string result = out.str();
    BOOST_TEST(result.find("Маршрут (ant):") != std::string::npos);
    BOOST_TEST(result.find("Старт") != std::string::npos);
    BOOST_TEST(result.find("home") != std::string::npos);
    BOOST_TEST(result.find("mine") != std::string::npos);
    BOOST_TEST(result.find("lake") != std::string::npos);
  }

  BOOST_AUTO_TEST_CASE(GreedyRouteWithIgnore)
  {
    World world;
    std::istringstream in(
      "create-map Test\n"
      "add-point Test home 100 64 house\n"
      "add-point Test mine 250 30 cave\n"
      "add-point Test lake 500 200 water\n"
      "plan-route-greedy Test 0 0 5 1 mine\nexit\n"
    );
    std::ostringstream out;

    processCommands(in, world, out);

    std::string result = out.str();
    size_t resultPos = result.find("Маршрут (greedy):");
    BOOST_REQUIRE(resultPos != std::string::npos);
    std::string afterResult = result.substr(resultPos);

    BOOST_TEST(afterResult.find("home") != std::string::npos);
    BOOST_TEST(afterResult.find("lake") != std::string::npos);
    BOOST_TEST(afterResult.find("mine") == std::string::npos);
  }

  BOOST_AUTO_TEST_CASE(GreedyRouteInvalidMap)
  {
    World world;
    std::istringstream in("plan-route-greedy Nonexistent 0 0 5 0\nexit\n");
    std::ostringstream out;

    processCommands(in, world, out);

    std::string expected = "Wrong usage. Use:\n"
                           "  plan-route-greedy <map-name> <x> <z> <time> <ignore-count> [ignore-points...] [-short]\n";
    BOOST_TEST(out.str() == expected);
  }

  BOOST_AUTO_TEST_CASE(GreedyRouteInvalidTime)
  {
    World world;
    std::istringstream in(
      "create-map Test\n"
      "plan-route-greedy Test 0 0 25 0\nexit\n"
    );
    std::ostringstream out;

    processCommands(in, world, out);

    std::string expected = "# Карта \"Test\" создана\n"
                           "Wrong usage. Use:\n"
                           "  plan-route-greedy <map-name> <x> <z> <time> <ignore-count> [ignore-points...] [-short]\n";
    BOOST_TEST(out.str() == expected);
  }

  BOOST_AUTO_TEST_CASE(GreedyRouteShortFlag)
  {
    World world;
    std::istringstream in(
      "create-map Test\n"
      "add-point Test home 100 64 house\n"
      "add-point Test mine 250 30 cave\n"
      "add-point Test lake 500 200 water\n"
      "plan-route-greedy Test 0 0 5 0 -short\nexit\n"
    );
    std::ostringstream out;

    processCommands(in, world, out);

    std::string result = out.str();

    size_t headerPos = result.find("Маршрут (greedy):");
    BOOST_REQUIRE(headerPos != std::string::npos);
    std::string afterHeader = result.substr(headerPos);

    BOOST_TEST(afterHeader.find("Маршрут (greedy):") != std::string::npos);

    BOOST_TEST(afterHeader.find("Старт") == std::string::npos);
    BOOST_TEST(afterHeader.find("home") == std::string::npos);

    BOOST_TEST(afterHeader.find("Общая длина:") != std::string::npos);
    BOOST_TEST(afterHeader.find("Общее время:") != std::string::npos);
    BOOST_TEST(afterHeader.find("Потрачено голода:") != std::string::npos);
    BOOST_TEST(afterHeader.find("Хлеба нужно:") != std::string::npos);
  }

  BOOST_AUTO_TEST_CASE(GreedyRouteShortFlagWithIgnore)
  {
    World world;
    std::istringstream in(
      "create-map Test\n"
      "add-point Test home 100 64 house\n"
      "add-point Test mine 250 30 cave\n"
      "add-point Test lake 500 200 water\n"
      "plan-route-greedy Test 0 0 5 1 mine -short\nexit\n"
    );
    std::ostringstream out;

    processCommands(in, world, out);

    std::string result = out.str();

    size_t headerPos = result.find("Маршрут (greedy):");
    BOOST_REQUIRE(headerPos != std::string::npos);
    std::string afterHeader = result.substr(headerPos);

    BOOST_TEST(afterHeader.find("mine") == std::string::npos);

    BOOST_TEST(afterHeader.find("Общая длина:") != std::string::npos);
  }

  BOOST_AUTO_TEST_CASE(TwoOptShortFlag)
  {
    World world;
    std::istringstream in(
      "create-map Test\n"
      "add-point Test home 100 64 house\n"
      "add-point Test mine 250 30 cave\n"
      "add-point Test lake 500 200 water\n"
      "plan-route-2opt Test 0 0 5 0 -short\nexit\n"
    );
    std::ostringstream out;

    processCommands(in, world, out);

    std::string result = out.str();
    size_t headerPos = result.find("Маршрут (2-opt):");
    BOOST_REQUIRE(headerPos != std::string::npos);
    std::string afterHeader = result.substr(headerPos);

    BOOST_TEST(afterHeader.find("Маршрут (2-opt):") != std::string::npos);
    BOOST_TEST(afterHeader.find("Старт") == std::string::npos);
    BOOST_TEST(afterHeader.find("Общая длина:") != std::string::npos);
  }

  BOOST_AUTO_TEST_CASE(MSTShortFlag)
  {
    World world;
    std::istringstream in(
      "create-map Test\n"
      "add-point Test home 100 64 house\n"
      "add-point Test mine 250 30 cave\n"
      "add-point Test lake 500 200 water\n"
      "plan-route-mst Test 0 0 5 0 -short\nexit\n"
    );
    std::ostringstream out;

    processCommands(in, world, out);

    std::string result = out.str();
    size_t headerPos = result.find("Маршрут (MST):");
    BOOST_REQUIRE(headerPos != std::string::npos);
    std::string afterHeader = result.substr(headerPos);

    BOOST_TEST(afterHeader.find("Маршрут (MST):") != std::string::npos);
    BOOST_TEST(afterHeader.find("Старт") == std::string::npos);
    BOOST_TEST(afterHeader.find("Общая длина:") != std::string::npos);
  }

  BOOST_AUTO_TEST_CASE(AntShortFlag)
  {
    World world;
    std::istringstream in(
      "create-map Test\n"
      "add-point Test home 100 64 house\n"
      "add-point Test mine 250 30 cave\n"
      "add-point Test lake 500 200 water\n"
      "plan-route-ant Test 0 0 5 0 -short\nexit\n"
    );
    std::ostringstream out;

    processCommands(in, world, out);

    std::string result = out.str();
    size_t headerPos = result.find("Маршрут (ant):");
    BOOST_REQUIRE(headerPos != std::string::npos);
    std::string afterHeader = result.substr(headerPos);

    BOOST_TEST(afterHeader.find("Маршрут (ant):") != std::string::npos);
    BOOST_TEST(afterHeader.find("Старт") == std::string::npos);
    BOOST_TEST(afterHeader.find("Общая длина:") != std::string::npos);
  }

  BOOST_AUTO_TEST_CASE(BestRoute)
  {
    World world;
    std::istringstream in(
      "create-map Test\n"
      "add-point Test home 100 64 house\n"
      "add-point Test mine 250 30 cave\n"
      "add-point Test lake 500 200 water\n"
      "best-route Test 0 0 5 0\nexit\n"
    );
    std::ostringstream out;

    processCommands(in, world, out);

    std::string result = out.str();
    BOOST_TEST(result.find("Сравнение алгоритмов") != std::string::npos);
    BOOST_TEST(result.find("Greedy") != std::string::npos);
    BOOST_TEST(result.find("2-opt") != std::string::npos);
    BOOST_TEST(result.find("MST") != std::string::npos);
    BOOST_TEST(result.find("ACO") != std::string::npos);
    BOOST_TEST(result.find("Лучший алгоритм:") != std::string::npos);
  }

  BOOST_AUTO_TEST_CASE(HelpCommand)
  {
    World world;
    std::istringstream in("help\nexit\n");
    std::ostringstream out;

    processCommands(in, world, out);

    std::string result = out.str();
    BOOST_TEST(result.find("Доступные команды:") != std::string::npos);
    BOOST_TEST(result.find("create-map") != std::string::npos);
    BOOST_TEST(result.find("plan-route-greedy") != std::string::npos);
    BOOST_TEST(result.find("best-route") != std::string::npos);
    BOOST_TEST(result.find("exit") != std::string::npos);
  }

  BOOST_AUTO_TEST_CASE(UnknownCommand)
  {
    World world;
    std::istringstream in("unknown-command\nexit\n");
    std::ostringstream out;

    processCommands(in, world, out);

    BOOST_TEST(out.str() == "Unknown command. Use 'help' to see available commands.\n");
  }

  BOOST_AUTO_TEST_CASE(EmptyLines)
  {
    World world;
    std::istringstream in("\n\ncreate-map Overworld\n\nlist-maps\n\nexit\n\n");
    std::ostringstream out;

    processCommands(in, world, out);

    std::string expected = "# Карта \"Overworld\" создана\nOverworld\n";
    BOOST_TEST(out.str() == expected);
  }

  BOOST_AUTO_TEST_SUITE_END()
}

