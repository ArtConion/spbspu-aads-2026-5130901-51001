#include "world.hpp"

namespace vishnyakov
{
  World::World():
    maps_()
  {}

  World::World(const World& other):
    maps_(other.maps_)
  {}

  World::World(World&& other) noexcept:
    maps_(std::move(other.maps_))
  {}

  World& World::operator=(const World& other)
  {
    if (this != &other)
    {
      World tmp(other);
      maps_ = std::move(tmp.maps_);
    }
    return *this;
  }

  World& World::operator=(World&& other) noexcept
  {
    if (this != &other)
    {
      maps_ = std::move(other.maps_);
    }
    return *this;
  }

  World::iterator World::findMap(const std::string& name)
  {
    for (auto it = maps_.begin(); it != maps_.end(); ++it)
    {
      if (it->getName() == name)
      {
        return it;
      }
    }
    return maps_.end();
  }

  World::const_iterator World::findMap(const std::string& name) const
  {
    for (auto it = maps_.cbegin(); it != maps_.cend(); ++it)
    {
      if (it->getName() == name)
      {
        return it;
      }
    }
    return maps_.cend();
  }

  bool World::createMap(const std::string& name)
  {
    if (findMap(name) != maps_.end())
    {
      return false;
    }
    maps_.push_back(Map(name));
    return true;
  }

  bool World::deleteMap(const std::string& name)
  {
    auto it = findMap(name);
    if (it == maps_.end())
    {
      return false;
    }
    maps_.erase(it);
    return true;
  }

  Map* World::getMap(const std::string& name)
  {
    auto it = findMap(name);
    if (it == maps_.end())
    {
      return nullptr;
    }
    return &(*it);
  }

  const Map* World::getMap(const std::string& name) const
  {
    auto it = findMap(name);
    if (it == maps_.cend())
    {
      return nullptr;
    }
    return &(*it);
  }

  bool World::hasMap(const std::string& name) const
  {
    return findMap(name) != maps_.cend();
  }

  std::size_t World::mapCount() const noexcept
  {
    return maps_.size();
  }

  bool World::empty() const noexcept
  {
    return maps_.empty();
  }

  void World::listMaps(std::ostream& out) const
  {
    if (maps_.empty())
    {
      out << "<EMPTY>\n";
      return;
    }

    for (auto it = maps_.cbegin(); it != maps_.cend(); ++it)
    {
      out << it->getName() << "\n";
    }
  }

  bool World::mergeMaps(const std::string& newName,
                        const std::string& name1,
                        const std::string& name2)
  {
    const Map* map1 = getMap(name1);
    const Map* map2 = getMap(name2);
    if (!map1 || !map2)
    {
      return false;
    }
    if (hasMap(newName))
    {
      return false;
    }

    Map newMap(newName);

    for (auto it = map1->begin(); it != map1->end(); ++it)
    {
      newMap.addWaypoint(it->first, it->second);
    }

    for (auto it = map2->begin(); it != map2->end(); ++it)
    {
      if (!newMap.findWaypoint(it->first))
      {
        newMap.addWaypoint(it->first, it->second);
      }
    }

    maps_.push_back(std::move(newMap));
    return true;
  }

  World::iterator World::begin() noexcept
  {
    return maps_.begin();
  }

  World::iterator World::end() noexcept
  {
    return maps_.end();
  }

  World::const_iterator World::begin() const noexcept
  {
    return maps_.begin();
  }

  World::const_iterator World::end() const noexcept
  {
    return maps_.end();
  }

  void World::clear()
  {
    maps_.clear();
  }
}

