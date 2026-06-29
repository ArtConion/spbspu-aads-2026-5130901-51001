#include "waypoint.hpp"
#include <cmath>

namespace vishnyakov
{
  Waypoint::Waypoint():
    x(0),
    z(0),
    type("")
  {}

  Waypoint::Waypoint(int x_, int z_, const std::string& type_):
    x(x_),
    z(z_),
    type(type_)
  {}

  Waypoint::Waypoint(int x_, int z_, std::string&& type_):
    x(x_),
    z(z_),
    type(std::move(type_))
  {}

  bool Waypoint::operator==(const Waypoint& other) const
  {
    return x == other.x && z == other.z && type == other.type;
  }

  bool Waypoint::operator!=(const Waypoint& other) const
  {
    return !(*this == other);
  }

  double Waypoint::distanceTo(int x_, int z_) const
  {
    double dx = static_cast<double>(x - x_);
    double dz = static_cast<double>(z - z_);
    return std::sqrt(dx * dx + dz * dz);
  }

  std::ostream& operator<<(std::ostream& out, const Waypoint& wp)
  {
    out << "(" << wp.x << ", " << wp.z << ") type=" << wp.type;
    return out;
  }
}

