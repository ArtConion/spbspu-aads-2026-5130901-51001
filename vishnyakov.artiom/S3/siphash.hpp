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
    std::size_t operator()(const std::string& key) const
    {
      return boost::hash< std::string >()(key);
    }

    template < class T1, class T2 >
    std::size_t operator()(const std::pair< T1, T2 >& key) const
    {
      std::size_t h1 = (*this)(key.first);
      std::size_t h2 = (*this)(key.second);
      return h1 ^ (h2 << 1);
    }
  };
}

#endif

