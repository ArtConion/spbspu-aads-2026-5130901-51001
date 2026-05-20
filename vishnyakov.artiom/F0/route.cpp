#include "route.hpp"
#include "commands.hpp"
#include <cmath>
#include <algorithm>
#include <limits>
#include <vector>
#include <random>

namespace vishnyakov
{
  SegmentResult traverseSegment(
    const std::string& nameA,
    int xA, int zA,
    const std::string& nameB,
    int xB, int zB,
    double startTime,
    bool isStartPoint)
  {
    SegmentResult result;
    result.totalNightTime = 0.0;
    double currentTime = startTime;
    double currentX = static_cast< double >(xA);
    double currentZ = static_cast< double >(zA);
    double targetX = static_cast< double >(xB);
    double targetZ = static_cast< double >(zB);

    if (currentTime >= NIGHT_START)
    {
      RouteStop nightStop;
      nightStop.isPoint = isStartPoint;
      nightStop.isNightStop = true;
      if (isStartPoint)
      {
        nightStop.name = nameA;
      }
      else
      {
        nightStop.name = "field";
      }
      nightStop.x = static_cast< int >(currentX);
      nightStop.z = static_cast< int >(currentZ);
      nightStop.time = currentTime;
      nightStop.travelTime = 0.0;
      nightStop.distanceFromPrev = 0.0;
      result.stops.push_back(nightStop);
      result.totalNightTime += CYCLE_LENGTH - currentTime;
      currentTime = 0.0;
    }

    while (true)
    {
      double dx = targetX - currentX;
      double dz = targetZ - currentZ;
      double remainingDistance = std::sqrt(dx * dx + dz * dz);

      if (remainingDistance < 1e-6)
      {
        break;
      }

      if (currentTime >= NIGHT_START)
      {
        RouteStop nightStop;
        nightStop.isPoint = false;
        nightStop.isNightStop = true;
        nightStop.name = "field";
        nightStop.x = static_cast< int >(currentX);
        nightStop.z = static_cast< int >(currentZ);
        nightStop.time = currentTime;
        nightStop.travelTime = 0.0;
        nightStop.distanceFromPrev = 0.0;
        result.stops.push_back(nightStop);
        result.totalNightTime += CYCLE_LENGTH - currentTime;
        currentTime = 0.0;
        continue;
      }

      double timeToNight = NIGHT_START - currentTime;
      double distanceCanGo = timeToNight * SPEED_BLOCKS_PER_MIN;

      if (distanceCanGo >= remainingDistance)
      {
        double travelTime = remainingDistance / SPEED_BLOCKS_PER_MIN;
        currentTime += travelTime;

        RouteStop arrival;
        arrival.isPoint = true;
        arrival.isNightStop = false;
        arrival.name = nameB;
        arrival.x = xB;
        arrival.z = zB;
        arrival.time = currentTime;
        arrival.travelTime = travelTime;
        arrival.distanceFromPrev = remainingDistance;
        result.stops.push_back(arrival);

        currentX = targetX;
        currentZ = targetZ;
        break;
      }
      else
      {
        double fraction = distanceCanGo / remainingDistance;
        currentX = currentX + fraction * dx;
        currentZ = currentZ + fraction * dz;
        double travelTime = distanceCanGo / SPEED_BLOCKS_PER_MIN;
        currentTime = NIGHT_START;

        RouteStop nightStop;
        nightStop.isPoint = false;
        nightStop.isNightStop = true;
        nightStop.name = "field";
        nightStop.x = static_cast< int >(currentX);
        nightStop.z = static_cast< int >(currentZ);
        nightStop.time = currentTime;
        nightStop.travelTime = travelTime;
        nightStop.distanceFromPrev = distanceCanGo;
        result.stops.push_back(nightStop);

        result.totalNightTime += CYCLE_LENGTH - currentTime;
        currentTime = 0.0;
      }
    }

    result.endTime = currentTime;
    return result;
  }

  double distanceBetween(const Waypoint& a, const Waypoint& b)
  {
    double dx = static_cast< double >(a.x - b.x);
    double dz = static_cast< double >(a.z - b.z);
    return std::sqrt(dx * dx + dz * dz);
  }

  RouteResult buildRouteFromOrder(
    const List< std::pair< std::string, Waypoint > >& points,
    int startX, int startZ,
    double startTime)
  {
    RouteResult result;
    result.totalDistance = 0.0;
    result.totalTime = 0.0;
    result.totalNightTime = 0.0;

    if (points.empty())
    {
      return result;
    }

    int currentX = startX;
    int currentZ = startZ;
    double currentTime = startTime;
    bool isAtPoint = false;
    std::string currentPointName = "start";
    List< RouteStop > allStops;

    RouteStop startStop;
    startStop.isPoint = false;
    startStop.isNightStop = false;
    startStop.name = "start";
    startStop.x = startX;
    startStop.z = startZ;
    startStop.time = currentTime;
    startStop.travelTime = 0.0;
    startStop.distanceFromPrev = 0.0;
    allStops.push_back(startStop);

    for (auto it = points.cbegin(); it != points.cend(); ++it)
    {
      const auto& point = *it;
      const Waypoint& wp = point.second;

      SegmentResult seg = traverseSegment(
        currentPointName, currentX, currentZ,
        point.first, wp.x, wp.z,
        currentTime,
        isAtPoint
      );

      double segmentDistance = 0.0;
      for (const auto& stop : seg.stops)
      {
        allStops.push_back(stop);
        segmentDistance += stop.distanceFromPrev;
      }

      result.totalDistance += segmentDistance;
      currentTime = seg.endTime;
      currentX = wp.x;
      currentZ = wp.z;
      currentPointName = point.first;
      isAtPoint = true;
      result.totalNightTime += seg.totalNightTime;
    }

    result.allStops = allStops;
    result.totalTime = currentTime + result.totalNightTime;
    result.totalHunger = result.totalDistance / 40.0;
    result.breadNeeded = static_cast< int >(std::ceil(result.totalHunger / 5.0));

    return result;
  }

  RouteResult buildGreedyRoute(
    const List< std::pair< std::string, Waypoint > >& points,
    int startX, int startZ,
    double startTime)
  {
    if (points.empty())
    {
      RouteResult empty;
      empty.totalDistance = 0.0;
      empty.totalTime = 0.0;
      empty.totalNightTime = 0.0;
      return empty;
    }

    List< bool > visited;
    for (size_t i = 0; i < points.size(); ++i)
    {
      visited.push_back(false);
    }

    int currentX = startX;
    int currentZ = startZ;
    double currentTime = startTime;
    bool isAtPoint = false;
    std::string currentPointName = "start";
    List< std::pair< std::string, Waypoint > > greedyOrder;

    int visitedCount = 0;
    while (visitedCount < static_cast< int >(points.size()))
    {
      int bestIndex = -1;
      double bestDist = std::numeric_limits< double >::max();

      int idx = 0;
      for (auto it = points.cbegin(); it != points.cend(); ++it, ++idx)
      {
        int vIdx = 0;
        for (auto vIt = visited.cbegin(); vIt != visited.cend(); ++vIt, ++vIdx)
        {
          if (vIdx == idx)
          {
            if (*vIt)
            {
              break;
            }
            const auto& wp = it->second;
            double dx = static_cast< double >(wp.x - currentX);
            double dz = static_cast< double >(wp.z - currentZ);
            double dist = std::sqrt(dx * dx + dz * dz);

            if (dist < bestDist)
            {
              bestDist = dist;
              bestIndex = idx;
            }
            break;
          }
        }
      }

      if (bestIndex == -1)
      {
        break;
      }

      int pointIdx = 0;
      const std::pair< std::string, Waypoint >* selectedPoint = nullptr;
      for (auto it = points.cbegin(); it != points.cend(); ++it, ++pointIdx)
      {
        if (pointIdx == bestIndex)
        {
          selectedPoint = &(*it);
          break;
        }
      }

      if (!selectedPoint)
      {
        break;
      }

      greedyOrder.push_back(*selectedPoint);

      const auto& wp = selectedPoint->second;

      SegmentResult seg = traverseSegment(
        currentPointName, currentX, currentZ,
        selectedPoint->first, wp.x, wp.z,
        currentTime,
        isAtPoint
      );

      currentTime = seg.endTime;
      currentX = wp.x;
      currentZ = wp.z;
      currentPointName = selectedPoint->first;
      isAtPoint = true;

      int vIdx = 0;
      for (auto vIt = visited.begin(); vIt != visited.end(); ++vIt, ++vIdx)
      {
        if (vIdx == bestIndex)
        {
          *vIt = true;
          break;
        }
      }
      ++visitedCount;
    }

    return buildRouteFromOrder(greedyOrder, startX, startZ, startTime);
  }

  RouteResult improve2Opt(
    const List< std::pair< std::string, Waypoint > >& points,
    int startX, int startZ,
    double startTime)
  {
    if (points.size() < 3)
    {
      return buildGreedyRoute(points, startX, startZ, startTime);
    }

    std::vector< std::pair< std::string, Waypoint > > pointVec;
    for (auto it = points.cbegin(); it != points.cend(); ++it)
    {
      pointVec.push_back(*it);
    }

    bool improved = true;
    int iteration = 0;
    const int maxIterations = 100;

    while (improved && iteration < maxIterations)
    {
      improved = false;
      ++iteration;

      for (size_t i = 0; i < pointVec.size() - 1; ++i)
      {
        for (size_t j = i + 2; j < pointVec.size(); ++j)
        {
          size_t next_i = i + 1;
          size_t next_j = (j + 1) % pointVec.size();

          const Waypoint& a = pointVec[i].second;
          const Waypoint& b = pointVec[next_i].second;
          const Waypoint& c = pointVec[j].second;
          const Waypoint& d = pointVec[next_j].second;

          double currentDist = distanceBetween(a, b) + distanceBetween(c, d);
          double newDist = distanceBetween(a, c) + distanceBetween(b, d);

          if (newDist < currentDist - 1e-6)
          {
            std::reverse(pointVec.begin() + next_i, pointVec.begin() + j + 1);
            improved = true;
          }
        }
      }
    }

    List< std::pair< std::string, Waypoint > > optimizedPoints;
    for (const auto& p : pointVec)
    {
      optimizedPoints.push_back(p);
    }

    return buildRouteFromOrder(optimizedPoints, startX, startZ, startTime);
  }

  RouteResult buildMSTRoute(
    const List< std::pair< std::string, Waypoint > >& points,
    int startX, int startZ,
    double startTime)
  {
    if (points.empty())
    {
      RouteResult empty;
      empty.totalDistance = 0.0;
      empty.totalTime = 0.0;
      empty.totalNightTime = 0.0;
      return empty;
    }

    size_t n = points.size();
    std::vector< std::pair< std::string, Waypoint > > pointVec;
    for (auto it = points.cbegin(); it != points.cend(); ++it)
    {
      pointVec.push_back(*it);
    }

    std::vector< bool > inMST(n, false);
    std::vector< double > minDist(n, std::numeric_limits< double >::max());
    std::vector< int > parent(n, -1);

    minDist[0] = 0.0;

    for (size_t i = 0; i < n; ++i)
    {
      int u = -1;
      for (size_t j = 0; j < n; ++j)
      {
        if (!inMST[j] && (u == -1 || minDist[j] < minDist[u]))
        {
          u = j;
        }
      }

      if (u == -1)
      {
        break;
      }

      inMST[u] = true;

      for (size_t v = 0; v < n; ++v)
      {
        if (!inMST[v])
        {
          double dist = distanceBetween(pointVec[u].second, pointVec[v].second);
          if (dist < minDist[v])
          {
            minDist[v] = dist;
            parent[v] = u;
          }
        }
      }
    }

    std::vector< std::vector< int > > tree(n);
    for (size_t i = 1; i < n; ++i)
    {
      if (parent[i] != -1)
      {
        tree[parent[i]].push_back(i);
        tree[i].push_back(parent[i]);
      }
    }

    std::vector< int > order;
    std::vector< bool > visited(n, false);

    std::vector< int > stack;
    stack.push_back(0);
    visited[0] = true;

    while (!stack.empty())
    {
      int u = stack.back();
      stack.pop_back();
      order.push_back(u);

      for (int v : tree[u])
      {
        if (!visited[v])
        {
          visited[v] = true;
          stack.push_back(v);
        }
      }
    }

    List< std::pair< std::string, Waypoint > > mstOrder;
    for (int idx : order)
    {
      mstOrder.push_back(pointVec[idx]);
    }

    return buildRouteFromOrder(mstOrder, startX, startZ, startTime);
  }

  RouteResult buildAntRoute(
    const List< std::pair< std::string, Waypoint > >& points,
    int startX, int startZ,
    double startTime,
    int iterations,
    int antsCount)
  {
    if (points.size() < 2)
    {
      return buildGreedyRoute(points, startX, startZ, startTime);
    }

    const double ALPHA = 1.0;
    const double BETA = 2.0;
    const double EVAPORATION = 0.5;
    const double Q = 100.0;

    size_t n = points.size();
    std::vector< std::pair< std::string, Waypoint > > pointVec;
    for (auto it = points.cbegin(); it != points.cend(); ++it)
    {
      pointVec.push_back(*it);
    }

    std::vector< std::vector< double > > dist(n, std::vector< double >(n, 0.0));
    for (size_t i = 0; i < n; ++i)
    {
      for (size_t j = 0; j < n; ++j)
      {
        if (i != j)
        {
          dist[i][j] = distanceBetween(pointVec[i].second, pointVec[j].second);
        }
      }
    }

    std::vector< std::vector< double > > pheromone(n, std::vector< double >(n, 1.0));

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    std::vector< int > bestPath;
    double bestLength = std::numeric_limits< double >::max();

    for (int iter = 0; iter < iterations; ++iter)
    {
      std::vector< std::vector< int > > antPaths(antsCount);
      std::vector< double > antLengths(antsCount, 0.0);

      for (int ant = 0; ant < antsCount; ++ant)
      {
        std::vector< bool > visited(n, false);
        std::vector< int > path;
        path.reserve(n);

        std::uniform_int_distribution<> startDist(0, n - 1);
        int current = startDist(gen);
        path.push_back(current);
        visited[current] = true;

        for (size_t step = 1; step < n; ++step)
        {
          std::vector< double > probs(n, 0.0);
          double sum = 0.0;

          for (size_t next = 0; next < n; ++next)
          {
            if (!visited[next])
            {
              double pherom = std::pow(pheromone[current][next], ALPHA);
              double heuristic = std::pow(1.0 / dist[current][next], BETA);
              probs[next] = pherom * heuristic;
              sum += probs[next];
            }
          }

          if (sum > 0)
          {
            double r = dis(gen);
            double cumulative = 0.0;
            int selected = -1;
            for (size_t next = 0; next < n; ++next)
            {
              if (!visited[next])
              {
                cumulative += probs[next] / sum;
                if (r <= cumulative)
                {
                  selected = next;
                  break;
                }
              }
            }
            if (selected != -1)
            {
              antLengths[ant] += dist[current][selected];
              current = selected;
              path.push_back(current);
              visited[current] = true;
            }
          }
        }

        antPaths[ant] = path;
      }

      for (size_t i = 0; i < n; ++i)
      {
        for (size_t j = 0; j < n; ++j)
        {
          pheromone[i][j] *= (1.0 - EVAPORATION);
        }
      }

      for (int ant = 0; ant < antsCount; ++ant)
      {
        double length = antLengths[ant];
        double delta = Q / length;

        for (size_t k = 0; k < n - 1; ++k)
        {
          int from = antPaths[ant][k];
          int to = antPaths[ant][k + 1];
          pheromone[from][to] += delta;
          pheromone[to][from] += delta;
        }

        if (length < bestLength)
        {
          bestLength = length;
          bestPath = antPaths[ant];
        }
      }
    }

    List< std::pair< std::string, Waypoint > > orderedPoints;
    for (int idx : bestPath)
    {
      orderedPoints.push_back(pointVec[idx]);
    }

    return buildRouteFromOrder(orderedPoints, startX, startZ, startTime);
  }

  // ========== Вспомогательные функции для парсинга ==========

  List< std::string > parseIgnorePoints(std::istringstream& iss, int ignoreCount)
  {
    List< std::string > ignorePoints;
    for (int i = 0; i < ignoreCount; ++i)
    {
      std::string pointName;
      iss >> pointName;
      if (!pointName.empty() && pointName != "-short" && pointName != "--only-results")
      {
        ignorePoints.push_back(pointName);
      }
    }
    return ignorePoints;
  }

  bool parseShortFlag(std::istringstream& iss)
  {
    std::string flag;
    iss >> flag;
    return (flag == "-short" || flag == "--only-results");
  }

  bool validateRouteParams(
    const std::string& mapName,
    double startTime,
    int ignoreCount,
    std::ostream& out,
    const std::string& cmdName)
  {
    if (mapName.empty() || startTime < 0.0 || startTime >= CYCLE_LENGTH || ignoreCount < 0)
    {
      out << "Wrong usage. Use:\n";
      printCommandUsage(out, cmdName);
      return false;
    }
    return true;
  }

  List< std::pair< std::string, Waypoint > > collectPoints(
    const Map* map,
    const List< std::string >& ignorePoints)
  {
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
    return points;
  }
}

