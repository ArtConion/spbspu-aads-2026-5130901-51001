#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include "../common/list.hpp"
#include <cstddef>
#include <utility>
#include <stdexcept>

namespace vishnyakov
{
  template < class Key, class Value, class Hash, class Equal >
  class HashTable;

  template < class Key, class Value, class Hash, class Equal >
  class Iter
  {
    friend class HashTable< Key, Value, Hash, Equal >;

  public:
    Iter();
    Iter(const Iter&) = default;
    Iter& operator=(const Iter&) = default;
    ~Iter() = default;

    std::pair< const Key, Value >& operator*();
    std::pair< const Key, Value >* operator->();

    Iter& operator++();
    Iter operator++(int);

    bool operator==(const Iter& other) const;
    bool operator!=(const Iter& other) const;

  private:
    struct Impl;
    Impl* impl_;
  };

  template < class Key, class Value, class Hash, class Equal >
  class CIter
  {
    friend class HashTable< Key, Value, Hash, Equal >;

  public:
    CIter();
    CIter(const CIter&) = default;
    CIter(const Iter< Key, Value, Hash, Equal >& other);
    ~CIter() = default;

    CIter& operator=(const CIter&) = default;

    const std::pair< const Key, Value >& operator*() const;
    const std::pair< const Key, Value >* operator->() const;

    CIter& operator++();
    CIter operator++(int);

    bool operator==(const CIter& other) const;
    bool operator!=(const CIter& other) const;

  private:
    struct Impl;
    Impl* impl_;
  };

  template < class Key, class Value, class Hash, class Equal >
  class HashTable
  {
  public:
    HashTable();
    explicit HashTable(std::size_t initial_capacity);
    HashTable(const HashTable& other);
    HashTable(HashTable&& other) noexcept;
    ~HashTable();

    HashTable& operator=(const HashTable& other);
    HashTable& operator=(HashTable&& other) noexcept;

    Iter< Key, Value, Hash, Equal > begin() noexcept;
    Iter< Key, Value, Hash, Equal > end() noexcept;
    CIter< Key, Value, Hash, Equal > begin() const noexcept;
    CIter< Key, Value, Hash, Equal > end() const noexcept;
    CIter< Key, Value, Hash, Equal > cbegin() const noexcept;
    CIter< Key, Value, Hash, Equal > cend() const noexcept;

    bool empty() const noexcept;
    std::size_t size() const noexcept;
    std::size_t capacity() const noexcept;

    Value& operator[](const Key& key);
    Value& at(const Key& key);
    const Value& at(const Key& key) const;

    void add(const Key& key, const Value& value);
    void add(Key&& key, Value&& value);
    Value drop(const Key& key);
    bool has(const Key& key) const;
    void rehash(std::size_t new_capacity);
    void clear() noexcept;

    Hash hash_function() const;
    Equal key_eq() const;

    void swap(HashTable& other) noexcept;

  private:
    List< std::pair< const Key, Value > >* array_;
    std::size_t array_capacity_;
    std::size_t size_;
    Hash hash_;
    Equal equal_;

    std::size_t index(const Key& key) const;
    void rehash_impl(std::size_t new_capacity);
  };

  template < class Key, class Value, class Hash, class Equal >
  void swap(HashTable< Key, Value, Hash, Equal >& lhs, HashTable< Key, Value, Hash, Equal >& rhs) noexcept;
}

#endif

