#ifndef SIPHASH_HPP
#define SIPHASH_HPP

#include <boost/container_hash/hash.hpp>
#include <boost/hash2/get_integral_result.hpp>
#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/siphash.hpp>
#include <cstddef>
#include <string>
#include <utility>

namespace vishnyakov
{
  class SipHash
  {
  public:
    template <class T>
    std::size_t operator()(const T& key) const
    {
      boost::hash2::siphash_64 hasher{};
      boost::hash2::hash_append(hasher, {}, key);
      return boost::hash2::get_integral_result<std::size_t>(hasher);
    }
  };
}

#endif


