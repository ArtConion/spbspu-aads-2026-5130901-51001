#include "commands.hpp"
#include <sstream>

namespace vishnyakov
{
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
          out << "<INVALID COMMAND>\n";
        }
        else if (world.createMap(name))
        {
          // success, nothing to output
        }
        else
        {
          out << "<INVALID COMMAND>\n";
        }
      }
      else if (cmd == "delete-map")
      {
        std::string name;
        iss >> name;

        if (name.empty())
        {
          out << "<INVALID COMMAND>\n";
        }
        else if (world.deleteMap(name))
        {
          // success, nothing to output
        }
        else
        {
          out << "<INVALID COMMAND>\n";
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
          out << "<INVALID COMMAND>\n";
          continue;
        }

        Map* map = world.getMap(mapName);
        if (!map)
        {
          out << "<INVALID COMMAND>\n";
          continue;
        }

        if (map->findWaypoint(pointName))
        {
          out << "<INVALID COMMAND>\n";
          continue;
        }

        map->addWaypoint(pointName, x, z, type);
      }
      else if (cmd == "remove-point")
      {
        std::string mapName, pointName;
        iss >> mapName >> pointName;

        if (mapName.empty() || pointName.empty())
        {
          out << "<INVALID COMMAND>\n";
          continue;
        }

        Map* map = world.getMap(mapName);
        if (!map)
        {
          out << "<INVALID COMMAND>\n";
          continue;
        }

        if (!map->removeWaypoint(pointName))
        {
          out << "<INVALID COMMAND>\n";
        }
      }
      else if (cmd == "edit-point")
      {
        std::string mapName, pointName, newName, xStr, zStr, type;
        iss >> mapName >> pointName >> newName >> xStr >> zStr >> type;

        if (mapName.empty() || pointName.empty())
        {
          out << "<INVALID COMMAND>\n";
          continue;
        }

        Map* map = world.getMap(mapName);
        if (!map)
        {
          out << "<INVALID COMMAND>\n";
          continue;
        }

        Waypoint* wp = map->findWaypoint(pointName);
        if (!wp)
        {
          out << "<INVALID COMMAND>\n";
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
          // Изменение имени требует удаления и добавления с новым ключом
          Waypoint newWp(wp->x, wp->z, wp->type);
          map->removeWaypoint(pointName);
          map->addWaypoint(newName, newWp);
        }
      }
      else if (cmd == "show-points")
      {
        std::string mapName;
        iss >> mapName;

        if (mapName.empty())
        {
          out << "<INVALID COMMAND>\n";
          continue;
        }

        const Map* map = world.getMap(mapName);
        if (!map)
        {
          out << "<INVALID COMMAND>\n";
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
        out << "Not implemented yet\n";
      }
      else if (cmd == "find-by-type")
      {
        out << "Not implemented yet\n";
      }
      else if (cmd == "copy-point")
      {
        out << "Not implemented yet\n";
      }
      else if (cmd == "move-point")
      {
        out << "Not implemented yet\n";
      }
      else if (cmd == "merge-maps")
      {
        out << "Not implemented yet\n";
      }
      else if (cmd == "clear-map")
      {
        std::string mapName;
        iss >> mapName;

        if (mapName.empty())
        {
          out << "<INVALID COMMAND>\n";
          continue;
        }

        Map* map = world.getMap(mapName);
        if (!map)
        {
          out << "<INVALID COMMAND>\n";
          continue;
        }

        map->clear();
      }
      else if (cmd == "plan-route-greedy")
      {
        out << "Not implemented yet\n";
      }
      else if (cmd == "plan-route-2opt")
      {
        out << "Not implemented yet\n";
      }
      else if (cmd == "plan-route-mst")
      {
        out << "Not implemented yet\n";
      }
      else if (cmd == "save")
      {
        out << "Not implemented yet\n";
      }
      else if (cmd == "load")
      {
        out << "Not implemented yet\n";
      }
      else if (cmd == "exit")
      {
        break;
      }
      else
      {
        out << "<INVALID COMMAND>\n";
      }
    }
  }
}

