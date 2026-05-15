#ifndef SIPHASH_HPP
#define SIPHASH_HPP

#include <boost/container_hash/hash.hpp>
#include <cstddef>
#include <string>

namespace vishnyakov
{
  class SipHash
  {
  public:
    std::size_t operator()(const std::string& key) const
    {
      return boost::hash< std::string >()(key);
    }
  };
}

#endif

