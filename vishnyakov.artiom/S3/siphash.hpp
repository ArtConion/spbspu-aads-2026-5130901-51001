#ifndef SIPHASH_HPP
#define SIPHASH_HPP

#include <boost/container_hash/hash.hpp>
#include <cstddef>
#include <string>
#include <utility>

namespace vishnyakov
{
  class SipHash
  {
  public:
    template < class T >
    std::size_t operator()(const T& key) const
    {
      return boost::hash< T >()(key);
    }
  };
}

#endif

