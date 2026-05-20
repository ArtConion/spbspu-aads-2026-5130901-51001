#include "map.hpp"

namespace vishnyakov
{
  Map::Map():
    name_(""),
    waypoints_(),
    typeIndex_()
  {}

  Map::Map(const std::string& name):
    name_(name),
    waypoints_(),
    typeIndex_()
  {}

  Map::Map(const Map& other):
    name_(other.name_),
    waypoints_(other.waypoints_),
    typeIndex_()
  {
    for (auto it = waypoints_.begin(); it != waypoints_.end(); ++it)
    {
      typeIndex_[it->second.type].push_back(it->first);
    }
  }

  Map::Map(Map&& other) noexcept:
    name_(std::move(other.name_)),
    waypoints_(std::move(other.waypoints_)),
    typeIndex_(std::move(other.typeIndex_))
  {}

  Map& Map::operator=(const Map& other)
  {
    if (this != &other)
    {
      name_ = other.name_;
      waypoints_ = other.waypoints_;
      typeIndex_.clear();
      for (auto it = waypoints_.begin(); it != waypoints_.end(); ++it)
      {
        typeIndex_[it->second.type].push_back(it->first);
      }
    }
    return *this;
  }

  Map& Map::operator=(Map&& other) noexcept
  {
    if (this != &other)
    {
      name_ = std::move(other.name_);
      waypoints_ = std::move(other.waypoints_);
      typeIndex_ = std::move(other.typeIndex_);
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
    typeIndex_[type].push_back(name);
  }

  void Map::addWaypoint(const std::string& name, const Waypoint& wp)
  {
    waypoints_.push(name, wp);
    typeIndex_[wp.type].push_back(name);
  }

  void Map::addWaypoint(const std::string& name, Waypoint&& wp)
  {
    waypoints_.push(name, std::move(wp));
    typeIndex_[wp.type].push_back(name);
  }

  bool Map::removeWaypoint(const std::string& name)
  {
    const Waypoint* wp = findWaypoint(name);
    if (!wp)
    {
      return false;
    }

    std::string type = wp->type;
    waypoints_.drop(name);

    if (typeIndex_.has(type))
    {
      List< std::string >& names = typeIndex_.at(type);
      for (auto it = names.begin(); it != names.end(); ++it)
      {
        if (*it == name)
        {
          names.erase(it);
          break;
        }
      }
      if (names.empty())
      {
        typeIndex_.drop(type);
      }
    }

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

  void Map::findByType(const std::string& type, std::ostream& out) const
  {
    if (!typeIndex_.has(type))
    {
      out << "<EMPTY>\n";
      return;
    }

    const List< std::string >& names = typeIndex_.at(type);
    for (auto it = names.cbegin(); it != names.cend(); ++it)
    {
      const Waypoint* wp = findWaypoint(*it);
      if (wp)
      {
        out << *it << " " << wp->x << " " << wp->z << " " << wp->type << "\n";
      }
    }
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
    typeIndex_.clear();
  }
}

