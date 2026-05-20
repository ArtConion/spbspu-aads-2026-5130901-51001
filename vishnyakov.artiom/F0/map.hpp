#ifndef MAP_HPP
#define MAP_HPP

#include "avltree.hpp"
#include "cuckoo_ht.hpp"
#include "waypoint.hpp"
#include <string>

namespace vishnyakov
{
  class Map
  {
  public:
    using WaypointTree = AVLTree< std::string, Waypoint, std::less< std::string > >;
    using iterator = WaypointTree::iterator;
    using const_iterator = WaypointTree::const_iterator;

    Map();
    explicit Map(const std::string& name);
    Map(const Map& other);
    Map(Map&& other) noexcept;
    ~Map() = default;

    Map& operator=(const Map& other);
    Map& operator=(Map&& other) noexcept;

    const std::string& getName() const noexcept;

    void addWaypoint(const std::string& name, int x, int z, const std::string& type);
    void addWaypoint(const std::string& name, const Waypoint& wp);
    void addWaypoint(const std::string& name, Waypoint&& wp);
    bool removeWaypoint(const std::string& name);
    const Waypoint* findWaypoint(const std::string& name) const;
    Waypoint* findWaypoint(const std::string& name);

    void findByType(const std::string& type, std::ostream& out) const;

    bool empty() const noexcept;
    std::size_t size() const noexcept;

    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;

    void clear();

  private:
    std::string name_;
    WaypointTree waypoints_;
    CuckooHashTable< std::string, List< std::string > > typeIndex_;
  };
}

#endif

