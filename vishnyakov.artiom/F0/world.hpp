#ifndef WORLD_HPP
#define WORLD_HPP

#include "../common/list.hpp"
#include "map.hpp"
#include <string>
#include <iostream>

namespace vishnyakov
{
  class World
  {
  public:
    using MapList = List< Map >;
    using iterator = LIter< Map >;
    using const_iterator = LCIter< Map >;

    World();
    World(const World& other);
    World(World&& other) noexcept;
    ~World() = default;

    World& operator=(const World& other);
    World& operator=(World&& other) noexcept;

    bool createMap(const std::string& name);
    bool deleteMap(const std::string& name);
    Map* getMap(const std::string& name);
    const Map* getMap(const std::string& name) const;
    bool hasMap(const std::string& name) const;
    std::size_t mapCount() const noexcept;
    bool empty() const noexcept;

    void listMaps(std::ostream& out) const;

    bool mergeMaps(const std::string& newName,
                   const std::string& name1,
                   const std::string& name2);

    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;

    void clear();

  private:
    MapList maps_;

    iterator findMap(const std::string& name);
    const_iterator findMap(const std::string& name) const;
  };
}

#endif

