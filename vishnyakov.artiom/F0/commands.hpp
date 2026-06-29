#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "world.hpp"
#include "route.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <functional>

namespace vishnyakov
{
  void printCommandUsage(std::ostream& out, const std::string& cmd);

  void printRouteResult(std::ostream& out,
            const RouteResult& route,
            const std::string& algorithmName,
            bool shortOutput);

  void handleCreateMap(std::istringstream& iss, std::ostream& out, World& world);
  void handleDeleteMap(std::istringstream& iss, std::ostream& out, World& world);
  void handleAddPoint(std::istringstream& iss, std::ostream& out, World& world);
  void handleRemovePoint(std::istringstream& iss, std::ostream& out, World& world);
  void handleEditPoint(std::istringstream& iss, std::ostream& out, World& world);
  void handleShowPoints(std::istringstream& iss, std::ostream& out, World& world);
  void handleFindNearest(std::istringstream& iss, std::ostream& out, World& world);
  void handleFindByType(std::istringstream& iss, std::ostream& out, World& world);
  void handleCopyPoint(std::istringstream& iss, std::ostream& out, World& world);
  void handleMovePoint(std::istringstream& iss, std::ostream& out, World& world);
  void handleMergeMaps(std::istringstream& iss, std::ostream& out, World& world);
  void handleClearMap(std::istringstream& iss, std::ostream& out, World& world);
  void handleSave(std::istringstream& iss, std::ostream& out, World& world);
  void handleLoad(std::istringstream& iss, std::ostream& out, World& world);
  void handlePlanRouteGreedy(std::istringstream& iss, std::ostream& out, World& world);
  void handlePlanRoute2Opt(std::istringstream& iss, std::ostream& out, World& world);
  void handlePlanRouteMST(std::istringstream& iss, std::ostream& out, World& world);
  void handlePlanRouteAnt(std::istringstream& iss, std::ostream& out, World& world);
  void handleBestRoute(std::istringstream& iss, std::ostream& out, World& world);

  void handleListMaps(std::ostream& out, World& world);
  void handleHelp(std::ostream& out, World& world);

  void handleListMapsAdapter(std::istringstream& iss, std::ostream& out, World& world);
  void handleHelpAdapter(std::istringstream& iss, std::ostream& out, World& world);
}

#endif

