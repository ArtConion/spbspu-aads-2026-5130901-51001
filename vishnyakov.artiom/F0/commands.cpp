#include "commands.hpp"
#include "route.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>

namespace vishnyakov
{
  struct NearestResult
  {
    std::string name;
    int x;
    int z;
    std::string type;
    double distance;
    double coefficient;
  };

  void printStopDetails(std::ostream& out, const RouteStop& stop, int stepNumber)
  {
    double roundedTravel = std::round(stop.travelTime * 100.0) / 100.0;
    double roundedDist = std::round(stop.distanceFromPrev * 100.0) / 100.0;
    
    if (stepNumber > 1)
    {
      out << "      - Затраченное время: " << roundedTravel << " мин.\n";
      out << "      - Пройденная дистанция: " << roundedDist << " м.\n";
    }
  }

  void printCommandUsage(std::ostream& out, const std::string& cmd)
  {
    struct CommandInfo
    {
      std::string name;
      std::string syntax;
      std::string description;
      std::string category;
    };

    List< CommandInfo > allCommands;
    allCommands.push_back({"create-map", "create-map <name>", "создать новую карту", "maps"});
    allCommands.push_back({"delete-map", "delete-map <name>", "удалить карту", "maps"});
    allCommands.push_back({"list-maps", "list-maps", "показать все карты", "maps"});
    
    allCommands.push_back({"add-point", "add-point <map> <name> <x> <z> <type>", "добавить точку на карту", "points"});
    allCommands.push_back({"remove-point", "remove-point <map> <name>", "удалить точку", "points"});
    allCommands.push_back({"edit-point", "edit-point <map> <name> <new-name> <x> <z> <type>", "изменить точку (\"-\" = без изменений)", "points"});
    allCommands.push_back({"show-points", "show-points <map>", "показать все точки карты", "points"});
    
    allCommands.push_back({"find-nearest", "find-nearest <map> <x> <z> <k> [type]", "найти K ближайших точек", "search"});
    allCommands.push_back({"find-by-type", "find-by-type <map> <type>", "найти точки по типу", "search"});
    allCommands.push_back({"copy-point", "copy-point <src> <dst> <name>", "скопировать точку между картами", "search"});
    allCommands.push_back({"move-point", "move-point <src> <dst> <name>", "переместить точку между картами", "search"});
    
    allCommands.push_back({"merge-maps", "merge-maps <new> <map1> <map2>", "объединить две карты", "operations"});
    allCommands.push_back({"clear-map", "clear-map <map>", "очистить карту", "operations"});
    
    allCommands.push_back({"plan-route-greedy", "plan-route-greedy <map> <x> <z> <time> <n> [p...] [-short]", "жадный алгоритм", "routes"});
    allCommands.push_back({"plan-route-2opt", "plan-route-2opt <map> <x> <z> <time> <n> [p...] [-short]", "2-opt улучшение", "routes"});
    allCommands.push_back({"plan-route-mst", "plan-route-mst <map> <x> <z> <time> <n> [p...] [-short]", "MST (Prim)", "routes"});
    allCommands.push_back({"plan-route-ant", "plan-route-ant <map> <x> <z> <time> <n> [p...] [-short]", "муравьиный алгоритм", "routes"});
    allCommands.push_back({"best-route", "best-route <map> <x> <z> <time> <n> [p...]", "сравнить все алгоритмы", "routes"});
    
    allCommands.push_back({"save", "save <filename>", "сохранить все данные в файл", "io"});
    allCommands.push_back({"load", "load <filename>", "загрузить данные из файла", "io"});
    
    allCommands.push_back({"help", "help", "показать справку", "other"});
    allCommands.push_back({"exit", "exit", "выйти из программы", "other"});

    CuckooHashTable< std::string, std::string > categoryNames;
    categoryNames.add("maps", "=== Управление картами ===");
    categoryNames.add("points", "=== Управление точками ===");
    categoryNames.add("search", "=== Поиск и навигация ===");
    categoryNames.add("operations", "=== Операции с картами ===");
    categoryNames.add("routes", "=== Маршрутизация ===");
    categoryNames.add("io", "=== Сохранение и загрузка ===");
    categoryNames.add("other", "=== Прочее ===");

    if (cmd == "all" || cmd.empty())
    {
      out << "Доступные команды:\n\n";

      std::string currentCategory;
      bool firstCategory = true;

      for (LCIter< CommandInfo > it = allCommands.cbegin(); it != allCommands.cend(); ++it)
      {
        if (it->category != currentCategory)
        {
          if (!firstCategory)
          {
            out << "\n";
          }
          firstCategory = false;
          currentCategory = it->category;
          
          if (categoryNames.has(currentCategory))
          {
            out << categoryNames.at(currentCategory) << "\n";
          }
        }

        out << "  " << std::left << std::setw(64) << it->syntax;
        out << " - " << it->description << "\n";
      }
    }
    else
    {
      for (LCIter< CommandInfo > it = allCommands.cbegin(); it != allCommands.cend(); ++it)
      {
        if (it->name == cmd)
        {
          out << "  " << it->syntax << "\n";
          return;
        }
      }
      out << "  Unknown command\n";
    }
  }

  void printRouteResult(std::ostream& out, const RouteResult& route,
                        const std::string& algorithmName, bool shortOutput)
  {
    out << "Маршрут (" << algorithmName << "):\n";
    if (!shortOutput)
    {
      int stepNumber = 1;
      for (LCIter< RouteStop > it = route.allStops.cbegin(); it != route.allStops.cend(); ++it)
      {
        const RouteStop& stop = *it;
        double roundedTime = std::round(stop.time * 100.0) / 100.0;

        if (stop.name == "start")
        {
          out << "  " << stepNumber << ". Старт (" << stop.x << ", " << stop.z << ")\n";
          out << "      - Текущее время: " << roundedTime << " мин.\n";
          stepNumber++;
        }
        else if (stop.isNightStop)
        {
          out << "  " << stepNumber << ". Остановка на ночь ("
              << stop.x << ", " << stop.z << ")\n";
          printStopDetails(out, stop, stepNumber);
          out << "      - Текущее время: " << roundedTime << " мин.\n";
          stepNumber++;
        }
        else if (stop.isPoint)
        {
          out << "  " << stepNumber << ". " << stop.name
              << " (" << stop.x << ", " << stop.z << ")\n";
          printStopDetails(out, stop, stepNumber);
          out << "      - Текущее время: " << roundedTime << " мин.\n";
          stepNumber++;
        }
      }
    }

    RouteStatistics stats = calculateRouteStatistics(route);
    out << "Общая длина: " << stats.roundedDistance << " блоков\n";
    out << "Общее время: " << stats.roundedTotalTime << " мин. ("
        << stats.days << " д. " << stats.minutesInCurrentDay << " мин.)\n";
    out << "Потрачено голода: " << stats.roundedHunger << " ед.\n";
    out << "Хлеба нужно: " << stats.breadNeeded << " шт.\n";
  }


  void handleCreateMap(std::istringstream& iss, std::ostream& out, World& world)
  {
    std::string name;
    iss >> name;
    if (name.empty())
    {
      out << "Wrong usage. Use:\n";
      printCommandUsage(out, "create-map");
    }
    else if (world.createMap(name))
    {
      out << "# Карта \"" << name << "\" создана\n";
    }
    else
    {
      out << "Map already exists\n";
    }
  }

  void handleDeleteMap(std::istringstream& iss, std::ostream& out, World& world)
  {
    std::string name;
    iss >> name;
    if (name.empty())
    {
      out << "Wrong usage. Use:\n";
      printCommandUsage(out, "delete-map");
    }
    else if (world.deleteMap(name))
    {
      out << "# Карта \"" << name << "\" удалена\n";
    }
    else
    {
      out << "Map doesn't exist\n";
    }
  }

  void handleListMaps(std::istringstream&, std::ostream& out, World& world)
  {
    world.listMaps(out);
  }

  void handleAddPoint(std::istringstream& iss, std::ostream& out, World& world)
  {
    std::string mapName, pointName, type;
    int x, z;
    iss >> mapName >> pointName >> x >> z >> type;

    if (mapName.empty() || pointName.empty() || type.empty())
    {
      out << "Wrong usage. Use:\n";
      printCommandUsage(out, "add-point");
      return;
    }

    Map* map = world.getMap(mapName);
    if (!map)
    {
      out << "Map doesn't exist\n";
      return;
    }

    if (map->findWaypoint(pointName))
    {
      out << "Point already exists\n";
      return;
    }

    map->addWaypoint(pointName, x, z, type);
    out << "# Точка \"" << pointName << "\" добавлена на карту \"" << mapName << "\"\n";
  }

  void handleRemovePoint(std::istringstream& iss, std::ostream& out, World& world)
  {
    std::string mapName, pointName;
    iss >> mapName >> pointName;

    if (mapName.empty() || pointName.empty())
    {
      out << "Wrong usage. Use:\n";
      printCommandUsage(out, "remove-point");
      return;
    }

    Map* map = world.getMap(mapName);
    if (!map)
    {
      out << "Map doesn't exist\n";
      return;
    }

    if (!map->removeWaypoint(pointName))
    {
      out << "Point doesn't exist\n";
    }
    else
    {
      out << "# Точка \"" << pointName << "\" удалена с карты \"" << mapName << "\"\n";
    }
  }

  void handleEditPoint(std::istringstream& iss, std::ostream& out, World& world)
  {
    std::string mapName, pointName, newName, xStr, zStr, type;
    iss >> mapName >> pointName >> newName >> xStr >> zStr >> type;

    if (mapName.empty() || pointName.empty())
    {
      out << "Wrong usage. Use:\n";
      printCommandUsage(out, "edit-point");
      return;
    }

    Map* map = world.getMap(mapName);
    if (!map)
    {
      out << "Map doesn't exist\n";
      return;
    }

    Waypoint* wp = map->findWaypoint(pointName);
    if (!wp)
    {
      out << "Point doesn't exist\n";
      return;
    }

    if (xStr != "-")
    {
      wp->x = std::stoi(xStr);
    }
    if (zStr != "-")
    {
      wp->z = std::stoi(zStr);
    }
    if (type != "-" && !type.empty())
    {
      wp->type = type;
    }

    if (newName != "-" && !newName.empty() && newName != pointName)
    {
      if (map->findWaypoint(newName))
      {
        out << "Point with new name already exists\n";
        return;
      }
      Waypoint newWp(wp->x, wp->z, wp->type);
      map->removeWaypoint(pointName);
      map->addWaypoint(newName, newWp);
    }

    out << "# Точка \"" << pointName << "\" изменена на карте \"" << mapName << "\"\n";
  }

  void handleShowPoints(std::istringstream& iss, std::ostream& out, World& world)
  {
    std::string mapName;
    iss >> mapName;

    if (mapName.empty())
    {
      out << "Wrong usage. Use:\n";
      printCommandUsage(out, "show-points");
      return;
    }

    const Map* map = world.getMap(mapName);
    if (!map)
    {
      out << "Map doesn't exist\n";
      return;
    }

    if (map->empty())
    {
      out << "<EMPTY>\n";
      return;
    }

    for (Map::const_iterator it = map->begin(); it != map->end(); ++it)
    {
      out << it->first << " " << it->second.x << " " << it->second.z
          << " " << it->second.type << "\n";
    }
  }

  void handleFindNearest(std::istringstream& iss, std::ostream& out, World& world)
  {
    std::string mapName;
    int x, z, k;
    std::string typeFilter;
    iss >> mapName >> x >> z >> k >> typeFilter;

    if (mapName.empty() || k <= 0)
    {
      out << "Wrong usage. Use:\n";
      printCommandUsage(out, "find-nearest");
      return;
    }

    const Map* map = world.getMap(mapName);
    if (!map)
    {
      out << "Map doesn't exist\n";
      return;
    }

    List< NearestResult > results;

    for (Map::const_iterator it = map->begin(); it != map->end(); ++it)
    {
      const std::string& name = it->first;
      const Waypoint& wp = it->second;

      if (!typeFilter.empty() && wp.type != typeFilter)
      {
        continue;
      }

      double dist = wp.distanceTo(x, z);
      NearestResult nr;
      nr.name = name;
      nr.x = wp.x;
      nr.z = wp.z;
      nr.type = wp.type;
      nr.distance = dist;
      nr.coefficient = 0.0;
      results.push_back(nr);
    }

    if (results.empty())
    {
      out << "<EMPTY>\n";
      return;
    }

    List< NearestResult > sortedResults;
    while (!results.empty())
    {
      LIter< NearestResult > minIt = results.begin();
      for (LIter< NearestResult > it = results.begin(); it != results.end(); ++it)
      {
        if (it->distance < minIt->distance)
        {
          minIt = it;
        }
      }
      sortedResults.push_back(*minIt);
      results.erase(minIt);
    }

    double minDist = sortedResults.cbegin()->distance;
    int count = 0;
    for (LCIter< NearestResult > it = sortedResults.cbegin(); it != sortedResults.cend() && count < k; ++it, ++count)
    {
      NearestResult res = *it;
      if (minDist > 0.0)
      {
        res.coefficient = (res.distance / minDist) * 100.0;
      }
      else
      {
        res.coefficient = (res.distance == 0.0) ? 100.0 : 0.0;
      }

      out << count + 1 << ". " << res.name << " ("
          << res.x << ", " << res.z << ") dist: "
          << res.distance << " coef: " << res.coefficient << "%\n";
    }
  }

  void handleFindByType(std::istringstream& iss, std::ostream& out, World& world)
  {
    std::string mapName, type;
    iss >> mapName >> type;

    if (mapName.empty() || type.empty())
    {
      out << "Wrong usage. Use:\n";
      printCommandUsage(out, "find-by-type");
      return;
    }

    const Map* map = world.getMap(mapName);
    if (!map)
    {
      out << "Map doesn't exist\n";
      return;
    }

    map->findByType(type, out);
  }

  void handleCopyPoint(std::istringstream& iss, std::ostream& out, World& world)
  {
    std::string srcMap, dstMap, pointName;
    iss >> srcMap >> dstMap >> pointName;

    if (srcMap.empty() || dstMap.empty() || pointName.empty())
    {
      out << "Wrong usage. Use:\n";
      printCommandUsage(out, "copy-point");
      return;
    }

    Map* src = world.getMap(srcMap);
    Map* dst = world.getMap(dstMap);

    if (!src)
    {
      out << "Source map doesn't exist\n";
      return;
    }
    if (!dst)
    {
      out << "Destination map doesn't exist\n";
      return;
    }

    const Waypoint* wp = src->findWaypoint(pointName);
    if (!wp)
    {
      out << "Point doesn't exist\n";
      return;
    }

    if (dst->findWaypoint(pointName))
    {
      out << "Point already exists in destination map\n";
      return;
    }

    dst->addWaypoint(pointName, *wp);
    out << "# Точка \"" << pointName << "\" скопирована с карты \""
        << srcMap << "\" на карту \"" << dstMap << "\"\n";
  }

  void handleMovePoint(std::istringstream& iss, std::ostream& out, World& world)
  {
    std::string srcMap, dstMap, pointName;
    iss >> srcMap >> dstMap >> pointName;

    if (srcMap.empty() || dstMap.empty() || pointName.empty())
    {
      out << "Wrong usage. Use:\n";
      printCommandUsage(out, "move-point");
      return;
    }

    Map* src = world.getMap(srcMap);
    Map* dst = world.getMap(dstMap);

    if (!src)
    {
      out << "Source map doesn't exist\n";
      return;
    }
    if (!dst)
    {
      out << "Destination map doesn't exist\n";
      return;
    }

    const Waypoint* wp = src->findWaypoint(pointName);
    if (!wp)
    {
      out << "Point doesn't exist\n";
      return;
    }

    if (dst->findWaypoint(pointName))
    {
      out << "Point already exists in destination map\n";
      return;
    }

    dst->addWaypoint(pointName, *wp);
    src->removeWaypoint(pointName);
    out << "# Точка \"" << pointName << "\" перемещена с карты \""
        << srcMap << "\" на карту \"" << dstMap << "\"\n";
  }

  void handleMergeMaps(std::istringstream& iss, std::ostream& out, World& world)
  {
    std::string newName, name1, name2;
    iss >> newName >> name1 >> name2;

    if (newName.empty() || name1.empty() || name2.empty())
    {
      out << "Wrong usage. Use:\n";
      printCommandUsage(out, "merge-maps");
      return;
    }

    if (world.mergeMaps(newName, name1, name2))
    {
      out << "# Карты \"" << name1 << "\" и \"" << name2
          << "\" объединены в \"" << newName << "\"\n";
    }
    else
    {
      if (!world.hasMap(name1))
      {
        out << "Map \"" << name1 << "\" doesn't exist\n";
      }
      else if (!world.hasMap(name2))
      {
        out << "Map \"" << name2 << "\" doesn't exist\n";
      }
      else if (world.hasMap(newName))
      {
        out << "Map \"" << newName << "\" already exists\n";
      }
      else
      {
        out << "Wrong usage. Use:\n";
        printCommandUsage(out, "merge-maps");
      }
    }
  }

  void handleClearMap(std::istringstream& iss, std::ostream& out, World& world)
  {
    std::string mapName;
    iss >> mapName;

    if (mapName.empty())
    {
      out << "Wrong usage. Use:\n";
      printCommandUsage(out, "clear-map");
      return;
    }

    Map* map = world.getMap(mapName);
    if (!map)
    {
      out << "Map doesn't exist\n";
      return;
    }

    map->clear();
    out << "# Карта \"" << mapName << "\" очищена\n";
  }

  void handleSave(std::istringstream& iss, std::ostream& out, World& world)
  {
    std::string filename;
    iss >> filename;

    if (filename.empty())
    {
      out << "Wrong usage. Use:\n";
      printCommandUsage(out, "save");
      return;
    }

    std::ofstream file(filename);
    if (!file.is_open())
    {
      out << "Cannot open file for writing\n";
      return;
    }

    for (World::iterator mapIt = world.begin(); mapIt != world.end(); ++mapIt)
    {
      file << mapIt->getName() << "\n";
      for (Map::const_iterator pointIt = mapIt->begin(); pointIt != mapIt->end(); ++pointIt)
      {
        file << pointIt->first << " "
             << pointIt->second.x << " "
             << pointIt->second.z << " "
             << pointIt->second.type << "\n";
      }
    }

    out << "# Данные сохранены в файл \"" << filename << "\"\n";
  }

  void handleLoad(std::istringstream& iss, std::ostream& out, World& world)
  {
    std::string filename;
    iss >> filename;

    if (filename.empty())
    {
      out << "Wrong usage. Use:\n";
      printCommandUsage(out, "load");
      return;
    }

    std::ifstream file(filename);
    if (!file.is_open())
    {
      out << "Cannot open file for reading\n";
      return;
    }

    World newWorld;
    std::string mapLine;
    std::string currentMapName;

    while (std::getline(file, mapLine))
    {
      if (mapLine.empty())
      {
        continue;
      }

      if (mapLine.find(' ') == std::string::npos)
      {
        currentMapName = mapLine;
        newWorld.createMap(currentMapName);
      }
      else
      {
        std::istringstream pointIss(mapLine);
        std::string pointName, type;
        int x, z;
        pointIss >> pointName >> x >> z >> type;

        Map* map = newWorld.getMap(currentMapName);
        if (map)
        {
          if (map->findWaypoint(pointName))
          {
            out << "# Предупреждение: точка \"" << pointName
                << "\" пропущена (дубликат)\n";
          }
          else
          {
            map->addWaypoint(pointName, x, z, type);
          }
        }
      }
    }

    world = std::move(newWorld);
    out << "# Данные загружены из файла \"" << filename << "\"\n";
  }

  void handlePlanRouteGreedy(std::istringstream& iss, std::ostream& out, World& world)
  {
    std::string mapName;
    int startX, startZ;
    double startTime;
    int ignoreCount;

    if (!validateRouteParams(iss, out, "plan-route-greedy", mapName, startX, startZ, startTime, ignoreCount))
    {
      return;
    }

    List< std::string > ignorePoints = parseIgnorePoints(iss, ignoreCount);
    bool shortOutput = parseShortFlag(iss);

    const Map* map = world.getMap(mapName);
    if (!map)
    {
      out << "Map doesn't exist\n";
      return;
    }

    List< std::pair< std::string, Waypoint > > points = collectPoints(map, ignorePoints);

    if (points.empty())
    {
      out << "<EMPTY>\n";
      return;
    }

    RouteResult route = buildGreedyRoute(points, startX, startZ, startTime);
    printRouteResult(out, route, "greedy", shortOutput);
  }

  void handlePlanRoute2Opt(std::istringstream& iss, std::ostream& out, World& world)
  {
    std::string mapName;
    int startX, startZ;
    double startTime;
    int ignoreCount;

    if (!validateRouteParams(iss, out, "plan-route-2opt", mapName, startX, startZ, startTime, ignoreCount))
    {
      return;
    }

    List< std::string > ignorePoints = parseIgnorePoints(iss, ignoreCount);
    bool shortOutput = parseShortFlag(iss);

    const Map* map = world.getMap(mapName);
    if (!map)
    {
      out << "Map doesn't exist\n";
      return;
    }

    List< std::pair< std::string, Waypoint > > points = collectPoints(map, ignorePoints);

    if (points.empty())
    {
      out << "<EMPTY>\n";
      return;
    }

    RouteResult route = improve2Opt(points, startX, startZ, startTime);
    printRouteResult(out, route, "2-opt", shortOutput);
  }

  void handlePlanRouteMST(std::istringstream& iss, std::ostream& out, World& world)
  {
    std::string mapName;
    int startX, startZ;
    double startTime;
    int ignoreCount;

    if (!validateRouteParams(iss, out, "plan-route-mst", mapName, startX, startZ, startTime, ignoreCount))
    {
      return;
    }

    List< std::string > ignorePoints = parseIgnorePoints(iss, ignoreCount);
    bool shortOutput = parseShortFlag(iss);

    const Map* map = world.getMap(mapName);
    if (!map)
    {
      out << "Map doesn't exist\n";
      return;
    }

    List< std::pair< std::string, Waypoint > > points = collectPoints(map, ignorePoints);

    if (points.empty())
    {
      out << "<EMPTY>\n";
      return;
    }

    RouteResult route = buildMSTRoute(points, startX, startZ, startTime);
    printRouteResult(out, route, "MST", shortOutput);
  }

  void handlePlanRouteAnt(std::istringstream& iss, std::ostream& out, World& world)
  {
    std::string mapName;
    int startX, startZ;
    double startTime;
    int ignoreCount;

    if (!validateRouteParams(iss, out, "plan-route-ant", mapName, startX, startZ, startTime, ignoreCount))
    {
      return;
    }

    List< std::string > ignorePoints = parseIgnorePoints(iss, ignoreCount);
    bool shortOutput = parseShortFlag(iss);

    const Map* map = world.getMap(mapName);
    if (!map)
    {
      out << "Map doesn't exist\n";
      return;
    }

    List< std::pair< std::string, Waypoint > > points = collectPoints(map, ignorePoints);

    if (points.empty())
    {
      out << "<EMPTY>\n";
      return;
    }

    RouteResult route = buildAntRoute(points, startX, startZ, startTime, 100, 10);
    printRouteResult(out, route, "ant", shortOutput);
  }

  void handleBestRoute(std::istringstream& iss, std::ostream& out, World& world)
  {
    std::string mapName;
    int startX, startZ;
    double startTime;
    int ignoreCount;

    if (!validateRouteParams(iss, out, "best-route", mapName, startX, startZ, startTime, ignoreCount))
    {
      return;
    }

    List< std::string > ignorePoints = parseIgnorePoints(iss, ignoreCount);

    const Map* map = world.getMap(mapName);
    if (!map)
    {
      out << "Map doesn't exist\n";
      return;
    }

    List< std::pair< std::string, Waypoint > > points = collectPoints(map, ignorePoints);

    if (points.empty())
    {
      out << "<EMPTY>\n";
      return;
    }

    out << "Сравнение алгоритмов для карты \"" << mapName << "\" "
        << "(старт " << startX << "," << startZ << ", время " << startTime << "):\n\n";

    struct AlgorithmResult
    {
      std::string name;
      double distance;
      double time;
      double hunger;
      int bread;
      RouteResult route;
    };

    List< AlgorithmResult > results;

    RouteResult greedyRoute = buildGreedyRoute(points, startX, startZ, startTime);
    AlgorithmResult gr;
    gr.name = "Greedy";
    gr.distance = greedyRoute.totalDistance;
    gr.time = greedyRoute.totalTime;
    gr.hunger = greedyRoute.totalHunger;
    gr.bread = greedyRoute.breadNeeded;
    gr.route = greedyRoute;
    results.push_back(gr);

    RouteResult opt2Route = improve2Opt(points, startX, startZ, startTime);
    AlgorithmResult opt2;
    opt2.name = "2-opt";
    opt2.distance = opt2Route.totalDistance;
    opt2.time = opt2Route.totalTime;
    opt2.hunger = opt2Route.totalHunger;
    opt2.bread = opt2Route.breadNeeded;
    opt2.route = opt2Route;
    results.push_back(opt2);

    RouteResult mstRoute = buildMSTRoute(points, startX, startZ, startTime);
    AlgorithmResult mst;
    mst.name = "MST";
    mst.distance = mstRoute.totalDistance;
    mst.time = mstRoute.totalTime;
    mst.hunger = mstRoute.totalHunger;
    mst.bread = mstRoute.breadNeeded;
    mst.route = mstRoute;
    results.push_back(mst);

    RouteResult antRoute = buildAntRoute(points, startX, startZ, startTime, 100, 10);
    AlgorithmResult ant;
    ant.name = "ACO";
    ant.distance = antRoute.totalDistance;
    ant.time = antRoute.totalTime;
    ant.hunger = antRoute.totalHunger;
    ant.bread = antRoute.breadNeeded;
    ant.route = antRoute;
    results.push_back(ant);

    out << "+------------+----------------+------------+------------+------------+\n";
    out << "| Алгоритм   | Длина(блоков)  | Время(мин) | Голод(ед)  | Хлеба(шт)  |\n";
    out << "+------------+----------------+------------+------------+------------+\n";

    const AlgorithmResult* best = NULL;
    double bestDistance = std::numeric_limits<double>::max();

    for (LCIter< AlgorithmResult > it = results.cbegin(); it != results.cend(); ++it)
    {
      const AlgorithmResult& r = *it;
      out << "| " << std::left << std::setw(10) << r.name << " | ";
      out << std::right << std::fixed << std::setprecision(2);
      out << std::setw(14) << r.distance << " | ";
      out << std::setw(10) << r.time << " | ";
      out << std::setw(10) << r.hunger << " | ";
      out << std::setw(10) << r.bread << " |\n";

      if (r.distance < bestDistance)
      {
        bestDistance = r.distance;
        best = &r;
      }
    }
    out << "+------------+----------------+------------+------------+------------+\n";

    if (best)
    {
      out << "\nЛучший алгоритм: " << best->name << "\n";
      out << "  Длина: " << std::fixed << std::setprecision(2) << best->distance << " блоков\n";
      out << "  Время: " << best->time << " мин.\n";
      out << "  Голод: " << best->hunger << " ед.\n";
      out << "  Хлеб:  " << best->bread << " шт.\n";
    }
  }

  void handleHelp(std::istringstream&, std::ostream& out, World&)
  {
    printCommandUsage(out, "all");
  }
}

