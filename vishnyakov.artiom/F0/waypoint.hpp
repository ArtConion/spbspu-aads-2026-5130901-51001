#ifndef WAYPOINT_HPP
#define WAYPOINT_HPP

#include <string>
#include <iostream>

namespace vishnyakov
{
  struct Waypoint
  {
    int x;
    int z;
    std::string type;

    Waypoint();
    Waypoint(int x_, int z_, const std::string& type_);
    Waypoint(int x_, int z_, std::string&& type_);
    Waypoint(const Waypoint& other) = default;
    Waypoint(Waypoint&& other) noexcept = default;

    Waypoint& operator=(const Waypoint& other) = default;
    Waypoint& operator=(Waypoint&& other) noexcept = default;

    bool operator==(const Waypoint& other) const;
    bool operator!=(const Waypoint& other) const;

    double distanceTo(int x_, int z_) const;
  };

  std::ostream& operator<<(std::ostream& out, const Waypoint& wp);
}

#endif

