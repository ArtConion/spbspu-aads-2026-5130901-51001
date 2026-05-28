#ifndef SIPHASH_HPP
#define SIPHASH_HPP

#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/siphash.hpp>
#include <cstddef>
#include <string>
#include <string_view>
#include <type_traits>

namespace vishnyakov
{
  class SipHash
  {
  private:
    static constexpr std::pair<std::uint64_t, std::uint64_t> KEY = {0x0706050403020100ULL, 0x0f0e0d0c0b0a0908ULL};
    
    struct hasher
    {
      using result_type = std::size_t;
      
      result_type operator()(const void* data, std::size_t len) const noexcept
      {
        boost::hash2::siphash_128 hash(KEY.first, KEY.second);
        hash.append(data, len);
        return static_cast<result_type>(hash.finalize());
      }
    };
    
    hasher hash_impl_;
    
  public:
    SipHash() = default;
    
    template <class T>
    std::size_t operator()(const T& key) const
    {
      if constexpr (std::is_same_v<T, std::string> || 
                    std::is_same_v<T, std::string_view> ||
                    std::is_same_v<T, const char*>)
      {
        if constexpr (std::is_same_v<T, const char*>)
        {
          return hash_impl_(key, std::char_traits<char>::length(key));
        }
        else
        {
          return hash_impl_(key.data(), key.size());
        }
      }
      else if constexpr (std::is_arithmetic_v<T>)
      {
        return hash_impl_(&key, sizeof(T));
      }
      else if constexpr (std::is_same_v<T, std::pair<std::string, std::string>>)
      {
        std::size_t h1 = operator()(key.first);
        std::size_t h2 = operator()(key.second);
        return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
      }
      else
      {
        static_assert(std::is_standard_layout_v<T>, "Type must be standard layout for bytewise hashing");
        return hash_impl_(&key, sizeof(T));
      }
    }
  };
}

#endif

