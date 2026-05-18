#include "map.hpp"

namespace vishnyakov
{
  Map::Map():
    name_(""),
    waypoints_()
  {}

  Map::Map(const std::string& name):
    name_(name),
    waypoints_()
  {}

  Map::Map(const Map& other):
    name_(other.name_),
    waypoints_(other.waypoints_)
  {}

  Map::Map(Map&& other) noexcept:
    name_(std::move(other.name_)),
    waypoints_(std::move(other.waypoints_))
  {}

  Map& Map::operator=(const Map& other)
  {
    if (this != &other)
    {
      Map tmp(other);
      swap(tmp);
    }
    return *this;
  }

  Map& Map::operator=(Map&& other) noexcept
  {
    if (this != &other)
    {
      name_ = std::move(other.name_);
      waypoints_ = std::move(other.waypoints_);
    }
    return *this;
  }

  const std::string& Map::getName() const noexcept
  {
    return name_;
  }

  void Map::addWaypoint(const std::string& name, int x, int z, const std::string& type)
  {
    Waypoint wp(x, z, type);
    waypoints_.push(name, wp);
  }

  void Map::addWaypoint(const std::string& name, Waypoint&& wp)
  {
    waypoints_.push(name, std::move(wp));
  }

  bool Map::removeWaypoint(const std::string& name)
  {
    if (!waypoints_.has(name))
    {
      return false;
    }
    waypoints_.drop(name);
    return true;
  }

  const Waypoint* Map::findWaypoint(const std::string& name) const
  {
    auto it = waypoints_.find(name);
    if (it == waypoints_.end())
    {
      return nullptr;
    }
    return &(it->second);
  }

  Waypoint* Map::findWaypoint(const std::string& name)
  {
    auto it = waypoints_.find(name);
    if (it == waypoints_.end())
    {
      return nullptr;
    }
    return &(it->second);
  }

  bool Map::empty() const noexcept
  {
    return waypoints_.empty();
  }

  std::size_t Map::size() const noexcept
  {
    return waypoints_.size();
  }

  Map::iterator Map::begin() noexcept
  {
    return waypoints_.begin();
  }

  Map::iterator Map::end() noexcept
  {
    return waypoints_.end();
  }

  Map::const_iterator Map::begin() const noexcept
  {
    return waypoints_.begin();
  }

  Map::const_iterator Map::end() const noexcept
  {
    return waypoints_.end();
  }

  void Map::clear()
  {
    waypoints_.clear();
  }
}

