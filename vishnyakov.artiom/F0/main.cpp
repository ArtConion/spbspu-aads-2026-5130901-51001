#include "commands.hpp"
#include "cuckoo_ht.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <functional>

int main()
{
  using namespace vishnyakov;

  std::cout << "=== Карта Мира с отметками (Waypoints) ===\n";
  std::cout << "Введите 'help' для списка команд или 'exit' для выхода\n\n";

  World world;

  CuckooHashTable< std::string,
            std::function<void(std::istringstream&, std::ostream&, World&)> >
    commands;

  commands.add("create-map", handleCreateMap);
  commands.add("delete-map", handleDeleteMap);
  commands.add("list-maps", handleListMapsAdapter);
  commands.add("add-point", handleAddPoint);
  commands.add("remove-point", handleRemovePoint);
  commands.add("edit-point", handleEditPoint);
  commands.add("show-points", handleShowPoints);
  commands.add("find-nearest", handleFindNearest);
  commands.add("find-by-type", handleFindByType);
  commands.add("copy-point", handleCopyPoint);
  commands.add("move-point", handleMovePoint);
  commands.add("merge-maps", handleMergeMaps);
  commands.add("clear-map", handleClearMap);
  commands.add("save", handleSave);
  commands.add("load", handleLoad);
  commands.add("plan-route-greedy", handlePlanRouteGreedy);
  commands.add("plan-route-2opt", handlePlanRoute2Opt);
  commands.add("plan-route-mst", handlePlanRouteMST);
  commands.add("plan-route-ant", handlePlanRouteAnt);
  commands.add("best-route", handleBestRoute);
  commands.add("help", handleHelpAdapter);

  std::string line;
  while (std::getline(std::cin, line))
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
      std::cout << "\nПрограмма завершена. До свидания!\n";
      break;
    }

    if (commands.has(cmd))
    {
      commands.at(cmd)(iss, std::cout, world);
    }
    else
    {
      std::cout << "Unknown command. Use 'help' to see available commands.\n";
    }
  }

  return 0;
}

