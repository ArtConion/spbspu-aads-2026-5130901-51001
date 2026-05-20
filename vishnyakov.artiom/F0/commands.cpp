#include "commands.hpp"
#include "route.hpp"
#include <sstream>
#include <algorithm>
#include <cmath>
#include <fstream>

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
      out << "  plan-route-greedy <map-name> <x> <z> <time> <ignore-count> [ignore-points...]\n";
    }
    else if (cmd == "plan-route-2opt")
    {
      out << "  plan-route-2opt <map-name> <x> <z> <time> <ignore-count> [ignore-points...]\n";
    }
    else if (cmd == "plan-route-mst")
    {
      out << "  plan-route-mst <map-name> <x> <z> <time> <ignore-count> [ignore-points...]\n";
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

  void printRouteResult(std::ostream& out, const RouteResult& route, const std::string& algorithmName)
  {
    out << "Маршрут (" << algorithmName << "):\n";
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
        out << "  " << stepNumber << ". Остановка на ночь (" << stop.x << ", " << stop.z << ")\n";
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
        out << "  " << stepNumber << ". " << stop.name << " (" << stop.x << ", " << stop.z << ")\n";
        if (stepNumber > 1)
        {
          out << "      - Затраченное время: " << roundedTravel << " мин.\n";
          out << "      - Пройденная дистанция: " << roundedDist << " м.\n";
        }
        out << "      - Текущее время: " << roundedTime << " мин.\n";
        stepNumber++;
      }
    }

    double roundedDistance = std::round(route.totalDistance * 100.0) / 100.0;
    double roundedTotalTime = std::round(route.totalTime * 100.0) / 100.0;
    double roundedHunger = std::round(route.totalHunger * 100.0) / 100.0;

    int days = static_cast< int >(route.totalTime / CYCLE_LENGTH);
    double minutesInCurrentDay = route.totalTime - (days * CYCLE_LENGTH);
    minutesInCurrentDay = std::round(minutesInCurrentDay * 100.0) / 100.0;

    out << "Общая длина: " << roundedDistance << " блоков\n";
    out << "Общее время: " << roundedTotalTime << " мин. (" << days << " д. " << minutesInCurrentDay << " мин.)\n";
    out << "Потрачено голода: " << roundedHunger << " ед.\n";
    out << "Хлеба нужно: " << route.breadNeeded << " шт.\n";
  }

  void processCommands(std::istream& in, World& world, std::ostream& out)
  {
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

      if (cmd == "create-map")
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
      }
      else if (cmd == "delete-map")
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
      }
      else if (cmd == "list-maps")
      {
        world.listMaps(out);
      }
      else if (cmd == "add-point")
      {
        std::string mapName, pointName, type;
        int x, z;
        iss >> mapName >> pointName >> x >> z >> type;

        if (mapName.empty() || pointName.empty() || type.empty())
        {
          out << "Wrong usage. Use:\n";
          printCommandUsage(out, "add-point");
          continue;
        }

        Map* map = world.getMap(mapName);
        if (!map)
        {
          out << "Wrong usage. Use:\n";
          printCommandUsage(out, "add-point");
          continue;
        }

        if (map->findWaypoint(pointName))
        {
          out << "Wrong usage. Use:\n";
          printCommandUsage(out, "add-point");
          continue;
        }

        map->addWaypoint(pointName, x, z, type);
        out << "# Точка \"" << pointName << "\" добавлена на карту \"" << mapName << "\"\n";
      }
      else if (cmd == "remove-point")
      {
        std::string mapName, pointName;
        iss >> mapName >> pointName;

        if (mapName.empty() || pointName.empty())
        {
          out << "Wrong usage. Use:\n";
          printCommandUsage(out, "remove-point");
          continue;
        }

        Map* map = world.getMap(mapName);
        if (!map)
        {
          out << "Wrong usage. Use:\n";
          printCommandUsage(out, "remove-point");
          continue;
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
      }
      else if (cmd == "edit-point")
      {
        std::string mapName, pointName, newName, xStr, zStr, type;
        iss >> mapName >> pointName >> newName >> xStr >> zStr >> type;

        if (mapName.empty() || pointName.empty())
        {
          out << "Wrong usage. Use:\n";
          printCommandUsage(out, "edit-point");
          continue;
        }

        Map* map = world.getMap(mapName);
        if (!map)
        {
          out << "Wrong usage. Use:\n";
          printCommandUsage(out, "edit-point");
          continue;
        }

        Waypoint* wp = map->findWaypoint(pointName);
        if (!wp)
        {
          out << "Wrong usage. Use:\n";
          printCommandUsage(out, "edit-point");
          continue;
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
      }
      else if (cmd == "show-points")
      {
        std::string mapName;
        iss >> mapName;

        if (mapName.empty())
        {
          out << "Wrong usage. Use:\n";
          printCommandUsage(out, "show-points");
          continue;
        }

        const Map* map = world.getMap(mapName);
        if (!map)
        {
          out << "Wrong usage. Use:\n";
          printCommandUsage(out, "show-points");
          continue;
        }

        if (map->empty())
        {
          out << "<EMPTY>\n";
          continue;
        }

        for (auto it = map->begin(); it != map->end(); ++it)
        {
          out << it->first << " " << it->second.x << " " << it->second.z
              << " " << it->second.type << "\n";
        }
      }
      else if (cmd == "find-nearest")
      {
        std::string mapName;
        int x, z, k;
        std::string typeFilter;
        iss >> mapName >> x >> z >> k >> typeFilter;

        if (mapName.empty() || k <= 0)
        {
          out << "Wrong usage. Use:\n";
          printCommandUsage(out, "find-nearest");
          continue;
        }

        const Map* map = world.getMap(mapName);
        if (!map)
        {
          out << "Wrong usage. Use:\n";
          printCommandUsage(out, "find-nearest");
          continue;
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
          continue;
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
      }
      else if (cmd == "find-by-type")
      {
        std::string mapName, type;
        iss >> mapName >> type;

        if (mapName.empty() || type.empty())
        {
          out << "Wrong usage. Use:\n";
          printCommandUsage(out, "find-by-type");
          continue;
        }

        const Map* map = world.getMap(mapName);
        if (!map)
        {
          out << "Wrong usage. Use:\n";
          printCommandUsage(out, "find-by-type");
          continue;
        }

        map->findByType(type, out);
      }
      else if (cmd == "copy-point")
      {
        std::string srcMap, dstMap, pointName;
        iss >> srcMap >> dstMap >> pointName;

        if (srcMap.empty() || dstMap.empty() || pointName.empty())
        {
          out << "Wrong usage. Use:\n";
          printCommandUsage(out, "copy-point");
          continue;
        }

        Map* src = world.getMap(srcMap);
        Map* dst = world.getMap(dstMap);

        if (!src || !dst)
        {
          out << "Wrong usage. Use:\n";
          printCommandUsage(out, "copy-point");
          continue;
        }

        const Waypoint* wp = src->findWaypoint(pointName);
        if (!wp)
        {
          out << "Wrong usage. Use:\n";
          printCommandUsage(out, "copy-point");
          continue;
        }

        if (dst->findWaypoint(pointName))
        {
          out << "Wrong usage. Use:\n";
          printCommandUsage(out, "copy-point");
          continue;
        }

        dst->addWaypoint(pointName, *wp);
        out << "# Точка \"" << pointName << "\" скопирована с карты \""
            << srcMap << "\" на карту \"" << dstMap << "\"\n";
      }
      else if (cmd == "move-point")
      {
        std::string srcMap, dstMap, pointName;
        iss >> srcMap >> dstMap >> pointName;

        if (srcMap.empty() || dstMap.empty() || pointName.empty())
        {
          out << "Wrong usage. Use:\n";
          printCommandUsage(out, "move-point");
          continue;
        }

        Map* src = world.getMap(srcMap);
        Map* dst = world.getMap(dstMap);

        if (!src || !dst)
        {
          out << "Wrong usage. Use:\n";
          printCommandUsage(out, "move-point");
          continue;
        }

        const Waypoint* wp = src->findWaypoint(pointName);
        if (!wp)
        {
          out << "Wrong usage. Use:\n";
          printCommandUsage(out, "move-point");
          continue;
        }

        if (dst->findWaypoint(pointName))
        {
          out << "Wrong usage. Use:\n";
          printCommandUsage(out, "move-point");
          continue;
        }

        dst->addWaypoint(pointName, *wp);
        src->removeWaypoint(pointName);
        out << "# Точка \"" << pointName << "\" перемещена с карты \""
            << srcMap << "\" на карту \"" << dstMap << "\"\n";
      }
      else if (cmd == "merge-maps")
      {
        std::string newName, name1, name2;
        iss >> newName >> name1 >> name2;

        if (newName.empty() || name1.empty() || name2.empty())
        {
          out << "Wrong usage. Use:\n";
          printCommandUsage(out, "merge-maps");
          continue;
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
      }
      else if (cmd == "clear-map")
      {
        std::string mapName;
        iss >> mapName;

        if (mapName.empty())
        {
          out << "Wrong usage. Use:\n";
          printCommandUsage(out, "clear-map");
          continue;
        }

        Map* map = world.getMap(mapName);
        if (!map)
        {
          out << "Wrong usage. Use:\n";
          printCommandUsage(out, "clear-map");
          continue;
        }

        map->clear();
        out << "# Карта \"" << mapName << "\" очищена\n";
      }
      else if (cmd == "save")
      {
        std::string filename;
        iss >> filename;

        if (filename.empty())
        {
          out << "Wrong usage. Use:\n";
          printCommandUsage(out, "save");
          continue;
        }

        std::ofstream file(filename);
        if (!file.is_open())
        {
          out << "Wrong usage. Use:\n";
          printCommandUsage(out, "save");
          continue;
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
      }
      else if (cmd == "load")
      {
        std::string filename;
        iss >> filename;

        if (filename.empty())
        {
          out << "Wrong usage. Use:\n";
          printCommandUsage(out, "load");
          continue;
        }

        std::ifstream file(filename);
        if (!file.is_open())
        {
          out << "Wrong usage. Use:\n";
          printCommandUsage(out, "load");
          continue;
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
      }
      else if (cmd == "plan-route-greedy")
      {
        std::string mapName;
        int startX, startZ;
        double startTime;
        int ignoreCount;
        iss >> mapName >> startX >> startZ >> startTime >> ignoreCount;

        if (mapName.empty() || startTime < 0.0 || startTime >= CYCLE_LENGTH || ignoreCount < 0)
        {
          out << "Wrong usage. Use:\n";
          printCommandUsage(out, "plan-route-greedy");
          continue;
        }

        List< std::string > ignorePoints;
        for (int i = 0; i < ignoreCount; ++i)
        {
          std::string pointName;
          iss >> pointName;
          if (!pointName.empty())
          {
            ignorePoints.push_back(pointName);
          }
        }

        const Map* map = world.getMap(mapName);
        if (!map)
        {
          out << "Wrong usage. Use:\n";
          printCommandUsage(out, "plan-route-greedy");
          continue;
        }

        List< std::pair< std::string, Waypoint > > points;
        for (auto it = map->begin(); it != map->end(); ++it)
        {
          bool ignored = false;
          for (auto ignIt = ignorePoints.cbegin(); ignIt != ignorePoints.cend(); ++ignIt)
          {
            if (it->first == *ignIt)
            {
              ignored = true;
              break;
            }
          }
          if (!ignored)
          {
            points.push_back(std::make_pair(it->first, it->second));
          }
        }

        if (points.empty())
        {
          out << "<EMPTY>\n";
          continue;
        }

        RouteResult route = buildGreedyRoute(points, startX, startZ, startTime);
        printRouteResult(out, route, "greedy");
      }
      else if (cmd == "plan-route-2opt")
      {
        std::string mapName;
        int startX, startZ;
        double startTime;
        int ignoreCount;
        iss >> mapName >> startX >> startZ >> startTime >> ignoreCount;

        if (mapName.empty() || startTime < 0.0 || startTime >= CYCLE_LENGTH || ignoreCount < 0)
        {
          out << "Wrong usage. Use:\n";
          printCommandUsage(out, "plan-route-2opt");
          continue;
        }

        List< std::string > ignorePoints;
        for (int i = 0; i < ignoreCount; ++i)
        {
          std::string pointName;
          iss >> pointName;
          if (!pointName.empty())
          {
            ignorePoints.push_back(pointName);
          }
        }

        const Map* map = world.getMap(mapName);
        if (!map)
        {
          out << "Wrong usage. Use:\n";
          printCommandUsage(out, "plan-route-2opt");
          continue;
        }

        List< std::pair< std::string, Waypoint > > points;
        for (auto it = map->begin(); it != map->end(); ++it)
        {
          bool ignored = false;
          for (auto ignIt = ignorePoints.cbegin(); ignIt != ignorePoints.cend(); ++ignIt)
          {
            if (it->first == *ignIt)
            {
              ignored = true;
              break;
            }
          }
          if (!ignored)
          {
            points.push_back(std::make_pair(it->first, it->second));
          }
        }

        if (points.empty())
        {
          out << "<EMPTY>\n";
          continue;
        }

        RouteResult route = improve2Opt(points, startX, startZ, startTime);
        printRouteResult(out, route, "2-opt");
      }
      else if (cmd == "plan-route-mst")
      {
        std::string mapName;
        int startX, startZ;
        double startTime;
        int ignoreCount;
        iss >> mapName >> startX >> startZ >> startTime >> ignoreCount;

        if (mapName.empty() || startTime < 0.0 || startTime >= CYCLE_LENGTH || ignoreCount < 0)
        {
          out << "Wrong usage. Use:\n";
          printCommandUsage(out, "plan-route-mst");
          continue;
        }

        List< std::string > ignorePoints;
        for (int i = 0; i < ignoreCount; ++i)
        {
          std::string pointName;
          iss >> pointName;
          if (!pointName.empty())
          {
            ignorePoints.push_back(pointName);
          }
        }

        const Map* map = world.getMap(mapName);
        if (!map)
        {
          out << "Wrong usage. Use:\n";
          printCommandUsage(out, "plan-route-mst");
          continue;
        }

        List< std::pair< std::string, Waypoint > > points;
        for (auto it = map->begin(); it != map->end(); ++it)
        {
          bool ignored = false;
          for (auto ignIt = ignorePoints.cbegin(); ignIt != ignorePoints.cend(); ++ignIt)
          {
            if (it->first == *ignIt)
            {
              ignored = true;
              break;
            }
          }
          if (!ignored)
          {
            points.push_back(std::make_pair(it->first, it->second));
          }
        }

        if (points.empty())
        {
          out << "<EMPTY>\n";
          continue;
        }

        RouteResult route = buildMSTRoute(points, startX, startZ, startTime);
        printRouteResult(out, route, "MST");
      }
      else if (cmd == "help")
      {
        out << "Доступные команды:\n\n"
            << "Управление картами:\n"
            << "  create-map <name>                     - создать новую карту\n"
            << "  delete-map <name>                     - удалить карту\n"
            << "  list-maps                             - показать все карты\n\n"
            << "Управление точками:\n"
            << "  add-point <map> <name> <x> <z> <type> - добавить точку на карту\n"
            << "  remove-point <map> <name>             - удалить точку\n"
            << "  edit-point <map> <name> <new-name> <x> <z> <type> - изменить точку (\"-\" = без изменений)\n"
            << "  show-points <map>                     - показать все точки карты\n\n"
            << "Поиск и навигация:\n"
            << "  find-nearest <map> <x> <z> <k> [type] - найти K ближайших точек\n"
            << "  find-by-type <map> <type>             - найти точки по типу\n"
            << "  copy-point <src> <dst> <name>         - скопировать точку между картами\n"
            << "  move-point <src> <dst> <name>         - переместить точку между картами\n\n"
            << "Операции с картами:\n"
            << "  merge-maps <new> <map1> <map2>        - объединить две карты\n"
            << "  clear-map <map>                       - очистить карту\n\n"
            << "Маршрутизация:\n"
            << "  plan-route-greedy <map> <x> <z> <time> <ignore-count> [points...] - жадный алгоритм\n"
            << "  plan-route-2opt <map> <x> <z> <time> <ignore-count> [points...]   - 2-opt улучшение\n"
            << "  plan-route-mst <map> <x> <z> <time> <ignore-count> [points...]     - MST (Prim)\n\n"
            << "Сохранение и загрузка:\n"
            << "  save <filename>                       - сохранить все данные в файл\n"
            << "  load <filename>                       - загрузить данные из файла\n\n"
            << "Прочее:\n"
            << "  help                                  - показать эту справку\n"
            << "  exit                                  - выйти из программы\n";
      }
      else if (cmd == "exit")
      {
        break;
      }
      else
      {
        out << "Unknown command. Use 'help' to see available commands.\n";
      }
    }
  }
}

