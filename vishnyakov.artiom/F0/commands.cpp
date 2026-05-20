#include "commands.hpp"
#include "route.hpp"
#include <sstream>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <functional>
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

  void printCommandUsage(std::ostream& out, const std::string& cmd)
  {
    if (cmd == "create-map")
    {
      out << "  create-map <map-name>\n";
    }
    else if (cmd == "delete-map")
    {
      out << "  delete-map <map-name>\n";
    }
    else if (cmd == "list-maps")
    {
      out << "  list-maps\n";
    }
    else if (cmd == "add-point")
    {
      out << "  add-point <map-name> <point-name> <x> <z> <type>\n";
    }
    else if (cmd == "remove-point")
    {
      out << "  remove-point <map-name> <point-name>\n";
    }
    else if (cmd == "edit-point")
    {
      out << "  edit-point <map-name> <point-name> <new-name> <x> <z> <type>\n";
    }
    else if (cmd == "show-points")
    {
      out << "  show-points <map-name>\n";
    }
    else if (cmd == "find-nearest")
    {
      out << "  find-nearest <map-name> <x> <z> <k> [type]\n";
    }
    else if (cmd == "find-by-type")
    {
      out << "  find-by-type <map-name> <type>\n";
    }
    else if (cmd == "copy-point")
    {
      out << "  copy-point <src-map> <dst-map> <point-name>\n";
    }
    else if (cmd == "move-point")
    {
      out << "  move-point <src-map> <dst-map> <point-name>\n";
    }
    else if (cmd == "merge-maps")
    {
      out << "  merge-maps <new-map-name> <map-name-1> <map-name-2>\n";
    }
    else if (cmd == "clear-map")
    {
      out << "  clear-map <map-name>\n";
    }
    else if (cmd == "save")
    {
      out << "  save <filename>\n";
    }
    else if (cmd == "load")
    {
      out << "  load <filename>\n";
    }
    else if (cmd == "plan-route-greedy")
    {
      out << "  plan-route-greedy <map-name> <x> <z> <time> <ignore-count> "
          << "[ignore-points...] [-short]\n";
    }
    else if (cmd == "plan-route-2opt")
    {
      out << "  plan-route-2opt <map-name> <x> <z> <time> <ignore-count> "
          << "[ignore-points...] [-short]\n";
    }
    else if (cmd == "plan-route-mst")
    {
      out << "  plan-route-mst <map-name> <x> <z> <time> <ignore-count> "
          << "[ignore-points...] [-short]\n";
    }
    else if (cmd == "plan-route-ant")
    {
      out << "  plan-route-ant <map-name> <x> <z> <time> <ignore-count> "
          << "[ignore-points...] [-short]\n";
    }
    else if (cmd == "best-route")
    {
      out << "  best-route <map-name> <x> <z> <time> <ignore-count> "
          << "[ignore-points...]\n";
    }
    else if (cmd == "help")
    {
      out << "  help\n";
    }
    else if (cmd == "exit")
    {
      out << "  exit\n";
    }
    else
    {
      out << "<UNKNOWN COMMAND>\n";
    }
  }

  void printRouteResult(std::ostream& out, const RouteResult& route,
                        const std::string& algorithmName, bool shortOutput)
  {
    out << "Маршрут (" << algorithmName << "):\n";
    if (!shortOutput)
    {
      int stepNumber = 1;
      for (auto it = route.allStops.cbegin(); it != route.allStops.cend(); ++it)
      {
        const auto& stop = *it;
        double roundedTime = std::round(stop.time * 100.0) / 100.0;
        double roundedTravel = std::round(stop.travelTime * 100.0) / 100.0;
        double roundedDist = std::round(stop.distanceFromPrev * 100.0) / 100.0;

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
          if (stepNumber > 1)
          {
            out << "      - Затраченное время: " << roundedTravel << " мин.\n";
            out << "      - Пройденная дистанция: " << roundedDist << " м.\n";
          }
          out << "      - Текущее время: " << roundedTime << " мин.\n";
          stepNumber++;
        }
        else if (stop.isPoint)
        {
          out << "  " << stepNumber << ". " << stop.name
              << " (" << stop.x << ", " << stop.z << ")\n";
          if (stepNumber > 1)
          {
            out << "      - Затраченное время: " << roundedTravel << " мин.\n";
            out << "      - Пройденная дистанция: " << roundedDist << " м.\n";
          }
          out << "      - Текущее время: " << roundedTime << " мин.\n";
          stepNumber++;
        }
      }
    }

    double roundedDistance = std::round(route.totalDistance * 100.0) / 100.0;
    double roundedTotalTime = std::round(route.totalTime * 100.0) / 100.0;
    double roundedHunger = std::round(route.totalHunger * 100.0) / 100.0;

    int days = static_cast< int >(route.totalTime / CYCLE_LENGTH);
    double minutesInCurrentDay = route.totalTime - (days * CYCLE_LENGTH);
    minutesInCurrentDay = std::round(minutesInCurrentDay * 100.0) / 100.0;

    out << "Общая длина: " << roundedDistance << " блоков\n";
    out << "Общее время: " << roundedTotalTime << " мин. ("
        << days << " д. " << minutesInCurrentDay << " мин.)\n";
    out << "Потрачено голода: " << roundedHunger << " ед.\n";
    out << "Хлеба нужно: " << route.breadNeeded << " шт.\n";
  }

  void processCommands(std::istream& in, World& world, std::ostream& out)
  {
    using CommandHandler = std::function< void(std::istringstream&, std::ostream&) >;

    CuckooHashTable< std::string, CommandHandler > commands;

    commands.add("create-map", [&](std::istringstream& iss, std::ostream& out)
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
        out << "Wrong usage. Use:\n";
        printCommandUsage(out, "create-map");
      }
    });

    commands.add("delete-map", [&](std::istringstream& iss, std::ostream& out)
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
        out << "Wrong usage. Use:\n";
        printCommandUsage(out, "delete-map");
      }
    });

    commands.add("list-maps", [&](std::istringstream&, std::ostream& out)
    {
      world.listMaps(out);
    });

    commands.add("add-point", [&](std::istringstream& iss, std::ostream& out)
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
        out << "Wrong usage. Use:\n";
        printCommandUsage(out, "add-point");
        return;
      }

      if (map->findWaypoint(pointName))
      {
        out << "Wrong usage. Use:\n";
        printCommandUsage(out, "add-point");
        return;
      }

      map->addWaypoint(pointName, x, z, type);
      out << "# Точка \"" << pointName << "\" добавлена на карту \"" << mapName << "\"\n";
    });

    commands.add("remove-point", [&](std::istringstream& iss, std::ostream& out)
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
        out << "Wrong usage. Use:\n";
        printCommandUsage(out, "remove-point");
        return;
      }

      if (!map->removeWaypoint(pointName))
      {
        out << "Wrong usage. Use:\n";
        printCommandUsage(out, "remove-point");
      }
      else
      {
        out << "# Точка \"" << pointName << "\" удалена с карты \"" << mapName << "\"\n";
      }
    });

    commands.add("edit-point", [&](std::istringstream& iss, std::ostream& out)
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
        out << "Wrong usage. Use:\n";
        printCommandUsage(out, "edit-point");
        return;
      }

      Waypoint* wp = map->findWaypoint(pointName);
      if (!wp)
      {
        out << "Wrong usage. Use:\n";
        printCommandUsage(out, "edit-point");
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
        Waypoint newWp(wp->x, wp->z, wp->type);
        map->removeWaypoint(pointName);
        map->addWaypoint(newName, newWp);
      }

      out << "# Точка \"" << pointName << "\" изменена на карте \"" << mapName << "\"\n";
    });

    commands.add("show-points", [&](std::istringstream& iss, std::ostream& out)
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
        out << "Wrong usage. Use:\n";
        printCommandUsage(out, "show-points");
        return;
      }

      if (map->empty())
      {
        out << "<EMPTY>\n";
        return;
      }

      for (auto it = map->begin(); it != map->end(); ++it)
      {
        out << it->first << " " << it->second.x << " " << it->second.z
            << " " << it->second.type << "\n";
      }
    });

    commands.add("find-nearest", [&](std::istringstream& iss, std::ostream& out)
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
        out << "Wrong usage. Use:\n";
        printCommandUsage(out, "find-nearest");
        return;
      }

      List< NearestResult > results;

      for (auto it = map->begin(); it != map->end(); ++it)
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
        auto minIt = results.begin();
        for (auto it = results.begin(); it != results.end(); ++it)
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
      for (auto it = sortedResults.cbegin(); it != sortedResults.cend() && count < k; ++it, ++count)
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
    });

    commands.add("find-by-type", [&](std::istringstream& iss, std::ostream& out)
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
        out << "Wrong usage. Use:\n";
        printCommandUsage(out, "find-by-type");
        return;
      }

      map->findByType(type, out);
    });

    commands.add("copy-point", [&](std::istringstream& iss, std::ostream& out)
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

      if (!src || !dst)
      {
        out << "Wrong usage. Use:\n";
        printCommandUsage(out, "copy-point");
        return;
      }

      const Waypoint* wp = src->findWaypoint(pointName);
      if (!wp)
      {
        out << "Wrong usage. Use:\n";
        printCommandUsage(out, "copy-point");
        return;
      }

      if (dst->findWaypoint(pointName))
      {
        out << "Wrong usage. Use:\n";
        printCommandUsage(out, "copy-point");
        return;
      }

      dst->addWaypoint(pointName, *wp);
      out << "# Точка \"" << pointName << "\" скопирована с карты \""
          << srcMap << "\" на карту \"" << dstMap << "\"\n";
    });

    commands.add("move-point", [&](std::istringstream& iss, std::ostream& out)
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

      if (!src || !dst)
      {
        out << "Wrong usage. Use:\n";
        printCommandUsage(out, "move-point");
        return;
      }

      const Waypoint* wp = src->findWaypoint(pointName);
      if (!wp)
      {
        out << "Wrong usage. Use:\n";
        printCommandUsage(out, "move-point");
        return;
      }

      if (dst->findWaypoint(pointName))
      {
        out << "Wrong usage. Use:\n";
        printCommandUsage(out, "move-point");
        return;
      }

      dst->addWaypoint(pointName, *wp);
      src->removeWaypoint(pointName);
      out << "# Точка \"" << pointName << "\" перемещена с карты \""
          << srcMap << "\" на карту \"" << dstMap << "\"\n";
    });

    commands.add("merge-maps", [&](std::istringstream& iss, std::ostream& out)
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
        out << "Wrong usage. Use:\n";
        printCommandUsage(out, "merge-maps");
      }
    });

    commands.add("clear-map", [&](std::istringstream& iss, std::ostream& out)
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
        out << "Wrong usage. Use:\n";
        printCommandUsage(out, "clear-map");
        return;
      }

      map->clear();
      out << "# Карта \"" << mapName << "\" очищена\n";
    });

    commands.add("save", [&](std::istringstream& iss, std::ostream& out)
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
        out << "Wrong usage. Use:\n";
        printCommandUsage(out, "save");
        return;
      }

      for (auto mapIt = world.begin(); mapIt != world.end(); ++mapIt)
      {
        file << mapIt->getName() << "\n";
        for (auto pointIt = mapIt->begin(); pointIt != mapIt->end(); ++pointIt)
        {
          file << pointIt->first << " "
               << pointIt->second.x << " "
               << pointIt->second.z << " "
               << pointIt->second.type << "\n";
        }
      }

      out << "# Данные сохранены в файл \"" << filename << "\"\n";
    });

    commands.add("load", [&](std::istringstream& iss, std::ostream& out)
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
        out << "Wrong usage. Use:\n";
        printCommandUsage(out, "load");
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
            map->addWaypoint(pointName, x, z, type);
          }
        }
      }

      world = std::move(newWorld);
      out << "# Данные загружены из файла \"" << filename << "\"\n";
    });

    commands.add("plan-route-greedy", [&](std::istringstream& iss, std::ostream& out)
    {
      std::string mapName;
      int startX, startZ;
      double startTime;
      int ignoreCount;
      iss >> mapName >> startX >> startZ >> startTime >> ignoreCount;

      if (!validateRouteParams(mapName, startTime, ignoreCount, out, "plan-route-greedy"))
      {
        return;
      }

      List< std::string > ignorePoints = parseIgnorePoints(iss, ignoreCount);
      bool shortOutput = parseShortFlag(iss);

      const Map* map = world.getMap(mapName);
      if (!map)
      {
        out << "Wrong usage. Use:\n";
        printCommandUsage(out, "plan-route-greedy");
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
    });

    commands.add("plan-route-2opt", [&](std::istringstream& iss, std::ostream& out)
    {
      std::string mapName;
      int startX, startZ;
      double startTime;
      int ignoreCount;
      iss >> mapName >> startX >> startZ >> startTime >> ignoreCount;

      if (!validateRouteParams(mapName, startTime, ignoreCount, out, "plan-route-2opt"))
      {
        return;
      }

      List< std::string > ignorePoints = parseIgnorePoints(iss, ignoreCount);
      bool shortOutput = parseShortFlag(iss);

      const Map* map = world.getMap(mapName);
      if (!map)
      {
        out << "Wrong usage. Use:\n";
        printCommandUsage(out, "plan-route-2opt");
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
    });

    commands.add("plan-route-mst", [&](std::istringstream& iss, std::ostream& out)
    {
      std::string mapName;
      int startX, startZ;
      double startTime;
      int ignoreCount;
      iss >> mapName >> startX >> startZ >> startTime >> ignoreCount;

      if (!validateRouteParams(mapName, startTime, ignoreCount, out, "plan-route-mst"))
      {
        return;
      }

      List< std::string > ignorePoints = parseIgnorePoints(iss, ignoreCount);
      bool shortOutput = parseShortFlag(iss);

      const Map* map = world.getMap(mapName);
      if (!map)
      {
        out << "Wrong usage. Use:\n";
        printCommandUsage(out, "plan-route-mst");
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
    });

    commands.add("plan-route-ant", [&](std::istringstream& iss, std::ostream& out)
    {
      std::string mapName;
      int startX, startZ;
      double startTime;
      int ignoreCount;
      iss >> mapName >> startX >> startZ >> startTime >> ignoreCount;

      if (!validateRouteParams(mapName, startTime, ignoreCount, out, "plan-route-ant"))
      {
        return;
      }

      List< std::string > ignorePoints = parseIgnorePoints(iss, ignoreCount);
      bool shortOutput = parseShortFlag(iss);

      const Map* map = world.getMap(mapName);
      if (!map)
      {
        out << "Wrong usage. Use:\n";
        printCommandUsage(out, "plan-route-ant");
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
    });

    commands.add("best-route", [&](std::istringstream& iss, std::ostream& out)
    {
      std::string mapName;
      int startX, startZ;
      double startTime;
      int ignoreCount;
      iss >> mapName >> startX >> startZ >> startTime >> ignoreCount;

      if (!validateRouteParams(mapName, startTime, ignoreCount, out, "best-route"))
      {
        return;
      }

      List< std::string > ignorePoints = parseIgnorePoints(iss, ignoreCount);

      const Map* map = world.getMap(mapName);
      if (!map)
      {
        out << "Wrong usage. Use:\n";
        printCommandUsage(out, "best-route");
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

      out << "Алгоритм      Длина (блоков)    Время (мин)    Голод (ед)    Хлеба (шт)\n";
      out << "------------------------------------------------------------------------\n";

      const AlgorithmResult* best = nullptr;
      double bestDistance = std::numeric_limits< double >::max();

      for (auto it = results.cbegin(); it != results.cend(); ++it)
      {
        const AlgorithmResult& r = *it;
        out << r.name;
        for (int i = r.name.size(); i < 14; ++i) out << " ";
        out << std::fixed << std::setprecision(2) << r.distance;
        out << "             " << r.time;
        out << "             " << r.hunger;
        out << "             " << r.bread << "\n";

        if (r.distance < bestDistance)
        {
          bestDistance = r.distance;
          best = &r;
        }
      }

      if (best)
      {
        out << "\nЛучший алгоритм: " << best->name << "\n";
        out << "  Длина: " << std::fixed << std::setprecision(2) << best->distance << " блоков\n";
        out << "  Время: " << best->time << " мин.\n";
        out << "  Голод: " << best->hunger << " ед.\n";
        out << "  Хлеб:  " << best->bread << " шт.\n";
      }
    });

    commands.add("help", [&](std::istringstream&, std::ostream& out)
    {
      out << "Доступные команды:\n\n"
          << "Управление картами:\n"
          << "  create-map <name>                     - создать новую карту\n"
          << "  delete-map <name>                     - удалить карту\n"
          << "  list-maps                             - показать все карты\n\n"
          << "Управление точками:\n"
          << "  add-point <map> <name> <x> <z> <type> - добавить точку на карту\n"
          << "  remove-point <map> <name>             - удалить точку\n"
          << "  edit-point <map> <name> <new-name> <x> <z> <type>\n"
          << "    - изменить точку (\"-\" = без изменений)\n"
          << "  show-points <map>                     - показать все точки карты\n\n"
          << "Поиск и навигация:\n"
          << "  find-nearest <map> <x> <z> <k> [type] - найти K ближайших точек\n"
          << "  find-by-type <map> <type>             - найти точки по типу\n"
          << "  copy-point <src> <dst> <name>         - скопировать точку\n"
          << "  move-point <src> <dst> <name>         - переместить точку\n\n"
          << "Операции с картами:\n"
          << "  merge-maps <new> <map1> <map2>        - объединить две карты\n"
          << "  clear-map <map>                       - очистить карту\n\n"
          << "Маршрутизация:\n"
          << "  plan-route-greedy <map> <x> <z> <time> <n> [p...] [-short]\n"
          << "    - жадный алгоритм\n"
          << "  plan-route-2opt <map> <x> <z> <time> <n> [p...] [-short]\n"
          << "    - 2-opt улучшение\n"
          << "  plan-route-mst <map> <x> <z> <time> <n> [p...] [-short]\n"
          << "    - MST (Prim)\n"
          << "  plan-route-ant <map> <x> <z> <time> <n> [p...] [-short]\n"
          << "    - муравьиный алгоритм\n\n"
          << "Сравнение:\n"
          << "  best-route <map> <x> <z> <time> <n> [p...]\n"
          << "    - сравнить все алгоритмы\n\n"
          << "Сохранение и загрузка:\n"
          << "  save <filename>                       - сохранить данные в файл\n"
          << "  load <filename>                       - загрузить данные из файла\n\n"
          << "Прочее:\n"
          << "  help                                  - показать справку\n"
          << "  exit                                  - выйти\n";
    });

    std::string line;
    while (std::getline(in, line))
    {
      if (line.empty())
      {
        continue;
      }

      std::istringstream iss(line);
      std::string cmd;
      iss >> cmd;

      if (cmd == "exit")
      {
        break;
      }

      if (commands.has(cmd))
      {
        commands.at(cmd)(iss, out);
      }
      else
      {
        out << "Unknown command. Use 'help' to see available commands.\n";
      }
    }
  }
}

