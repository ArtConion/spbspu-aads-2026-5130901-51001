#ifndef ROUTE_HPP
#define ROUTE_HPP

#include "waypoint.hpp"
#include "map.hpp"
#include "../common/list.hpp"
#include <string>
#include <utility>
#include <sstream>

namespace vishnyakov
{
  const double SPEED_BLOCKS_PER_SEC = 5.6;
  const double SPEED_BLOCKS_PER_MIN = SPEED_BLOCKS_PER_SEC * 60.0;
  const double DAY_START = 0.0;
  const double DAY_END = 10.0;
  const double NIGHT_START = 10.0;
  const double CYCLE_LENGTH = 20.0;

  struct RouteStop
  {
    bool isPoint;
    bool isNightStop;
    std::string name;
    int x;
    int z;
    double time;
    double travelTime;
    double distanceFromPrev;
  };

  struct SegmentResult
  {
    List< RouteStop > stops;
    double endTime;
    double totalNightTime;
  };

  inline std::pair< int, int > interpolate(int x1, int z1, int x2, int z2, double fraction)
  {
    return {
      static_cast< int >(x1 + fraction * (x2 - x1)),
      static_cast< int >(z1 + fraction * (z2 - z1))
    };
  }

  SegmentResult traverseSegment(
    const std::string& nameA,
    int xA, int zA,
    const std::string& nameB,
    int xB, int zB,
    double startTime,
    bool isStartPoint
  );

  struct RouteResult
  {
    List< RouteStop > allStops;
    double totalTime;
    double totalDistance;
    double totalHunger;
    int breadNeeded;
    double totalNightTime;
  };

  RouteResult buildGreedyRoute(
    const List< std::pair< std::string, Waypoint > >& points,
    int startX, int startZ,
    double startTime
  );

  RouteResult improve2Opt(
    const List< std::pair< std::string, Waypoint > >& points,
    int startX, int startZ,
    double startTime
  );

  RouteResult buildMSTRoute(
    const List< std::pair< std::string, Waypoint > >& points,
    int startX, int startZ,
    double startTime
  );

  RouteResult buildAntRoute(
    const List< std::pair< std::string, Waypoint > >& points,
    int startX, int startZ,
    double startTime,
    int iterations = 100,
    int antsCount = 10
  );

  void printCommandUsage(std::ostream& out, const std::string& cmd);

  void printRouteResult(std::ostream& out, const RouteResult& route, const std::string& algorithmName, bool shortOutput);

  List< std::string > parseIgnorePoints(std::istringstream& iss, int ignoreCount);

  bool parseShortFlag(std::istringstream& iss);

  bool validateRouteParams(
    const std::string& mapName,
    double startTime,
    int ignoreCount,
    std::ostream& out,
    const std::string& cmdName
  );

  List< std::pair< std::string, Waypoint > > collectPoints(
    const Map* map,
    const List< std::string >& ignorePoints
  );
}

#endif

