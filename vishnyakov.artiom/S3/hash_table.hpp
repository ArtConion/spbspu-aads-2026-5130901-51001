#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <list.hpp>
#include <cstddef>
#include <utility>
#include <stdexcept>
#include <algorithm>

namespace vishnyakov
{
template< class Key, class Value, class Hash, class Equal >
class HashTable
{
public:
  HashTable():
    array_(nullptr),
    array_capacity_(16),
    size_(0),
    hash_(),
    equal_(),
    max_load_factor_(0.75),
    max_chain_length_(4),
    rehash_policy_([](std::size_t current) { return current * 2; })
  {
    array_ = static_cast< List< std::pair< const Key, Value > >* >(
      ::operator new(sizeof(List< std::pair< const Key, Value > >) * array_capacity_)
    );

    for (std::size_t i = 0; i < array_capacity_; ++i)
    {
      new (&array_[i]) List< std::pair< const Key, Value > >();
    }
  }

  explicit HashTable(std::size_t initial_capacity):
    array_(nullptr),
    array_capacity_(initial_capacity > 0 ? initial_capacity : 16),
    size_(0),
    hash_(),
    equal_(),
    max_load_factor_(0.75),
    max_chain_length_(4),
    rehash_policy_([](std::size_t current) { return current * 2; })
  {
    array_ = static_cast< List< std::pair< const Key, Value > >* >(
      ::operator new(sizeof(List< std::pair< const Key, Value > >) * array_capacity_)
    );

    for (std::size_t i = 0; i < array_capacity_; ++i)
    {
      new (&array_[i]) List< std::pair< const Key, Value > >();
    }
  }

  HashTable(const HashTable& other):
    array_(nullptr),
    array_capacity_(other.array_capacity_),
    size_(other.size_),
    hash_(other.hash_),
    equal_(other.equal_),
    max_load_factor_(other.max_load_factor_),
    max_chain_length_(other.max_chain_length_),
    rehash_policy_(other.rehash_policy_)
  {
    array_ = static_cast< List< std::pair< const Key, Value > >* >(
      ::operator new(sizeof(List< std::pair< const Key, Value > >) * array_capacity_)
    );

    for (std::size_t i = 0; i < array_capacity_; ++i)
    {
      new (&array_[i]) List< std::pair< const Key, Value > >(other.array_[i]);
    }
  }

  HashTable(HashTable&& other) noexcept:
    array_(other.array_),
    array_capacity_(other.array_capacity_),
    size_(other.size_),
    hash_(std::move(other.hash_)),
    equal_(std::move(other.equal_)),
    max_load_factor_(other.max_load_factor_),
    max_chain_length_(other.max_chain_length_),
    rehash_policy_(std::move(other.rehash_policy_))
  {
    other.array_ = nullptr;
    other.array_capacity_ = 0;
    other.size_ = 0;
  }

  ~HashTable()
  {
    if (array_)
    {
      for (size_t i = 0; i < array_capacity_; ++i)
      {
        array_[i].~List();
      }

      ::operator delete(array_);
    }
  }

  HashTable& operator=(const HashTable& other)
  {
    if (this != &other)
    {
      HashTable tmp(other);
      swap(tmp);
    }

    return *this;
  }

  HashTable& operator=(HashTable&& other) noexcept
  {
    if (this != &other)
    {
      swap(other);
      other.clear();
    }

    return *this;
  }

  bool empty() const noexcept
  {
    return size_ == 0;
  }

  size_t size() const noexcept
  {
    return size_;
  }

  size_t capacity() const noexcept
  {
    return array_capacity_;
  }

  Value& at(const Key& key)
  {
    size_t idx = index(key);
    List< std::pair< const Key, Value > >& chain = array_[idx];

    for (LIter< std::pair< const Key, Value > > it = chain.begin();
         it != chain.end(); ++it)
    {
      if (equal_(it->first, key))
      {
        return it->second;
      }
    }

    throw std::out_of_range("Key not found");
  }

  const Value& at(const Key& key) const
  {
    size_t idx = index(key);
    const List< std::pair< const Key, Value > >& chain = array_[idx];

    for (LCIter< std::pair< const Key, Value > > it = chain.begin();
         it != chain.end(); ++it)
    {
      if (equal_(it->first, key))
      {
        return it->second;
      }
    }

    throw std::out_of_range("Key not found");
  }

  Value& operator[](const Key& key)
  {
    size_t idx = index(key);
    List< std::pair< const Key, Value > >& chain = array_[idx];

    for (LIter< std::pair< const Key, Value > > it = chain.begin();
         it != chain.end(); ++it)
    {
      if (equal_(it->first, key))
      {
        return it->second;
      }
    }

    chain.push_front(std::pair< const Key, Value >(key, Value()));
    ++size_;

    return chain.front().second;
  }

  void add(const Key& key, const Value& value)
  {
    std::size_t idx = index(key);
    List< std::pair< const Key, Value > >& chain = array_[idx];

    for (LIter< std::pair< const Key, Value > > it = chain.begin();
         it != chain.end(); ++it)
    {
      if (equal_(it->first, key))
      {
        return;
      }
    }

    chain.push_front(std::pair< const Key, Value >(key, value));
    ++size_;

    auto_rehash();
  }

  void add(Key&& key, Value&& value)
  {
    std::size_t idx = index(key);
    List< std::pair< const Key, Value > >& chain = array_[idx];

    for (LIter< std::pair< const Key, Value > > it = chain.begin();
         it != chain.end(); ++it)
    {
      if (equal_(it->first, key))
      {
        return;
      }
    }

    chain.push_front(std::pair< const Key, Value >(std::move(key), std::move(value)));
    ++size_;

    auto_rehash();
  }

  Value drop(const Key& key)
  {
    std::size_t idx = index(key);
    List< std::pair< const Key, Value > >& chain = array_[idx];

    if (chain.empty())
    {
      throw std::out_of_range("Key not found");
    }

    if (equal_(chain.front().first, key))
    {
      Value result = std::move(chain.front().second);
      chain.pop_front();
      --size_;
      return result;
    }

    LIter< std::pair< const Key, Value > > prev = chain.begin();
    LIter< std::pair< const Key, Value > > it = chain.begin();
    ++it;

    for (; it != chain.end(); ++it)
    {
      if (equal_(it->first, key))
      {
        Value result = std::move(it->second);
        chain.erase_after(prev);
        --size_;
        return result;
      }

      prev = it;
    }

    throw std::out_of_range("Key not found");
  }

  bool has(const Key& key) const
  {
    size_t idx = index(key);
    const List< std::pair< const Key, Value > >& chain = array_[idx];

    for (LCIter< std::pair< const Key, Value > > it = chain.begin();
         it != chain.end(); ++it)
    {
      if (equal_(it->first, key))
      {
        return true;
      }
    }

    return false;
  }

  List< Key > get_keys() const
  {
    List< Key > keys;

    for (size_t i = 0; i < array_capacity_; ++i)
    {
      const List< std::pair< const Key, Value > >& chain = array_[i];

      for (LCIter< std::pair< const Key, Value > > it = chain.begin();
           it != chain.end(); ++it)
      {
        keys.push_back(it->first);
      }
    }

    return keys;
  }

  void rehash(size_t new_capacity)
  {
    if (new_capacity == 0)
    {
      new_capacity = 16;
    }

    if (new_capacity == array_capacity_)
    {
      return;
    }

    rehash_impl(new_capacity);
  }

  void clear() noexcept
  {
    for (size_t i = 0; i < array_capacity_; ++i)
    {
      array_[i].clear();
    }

    size_ = 0;
  }

  Hash hash_function() const
  {
    return hash_;
  }

  Equal key_eq() const
  {
    return equal_;
  }

  void swap(HashTable& other) noexcept
  {
    std::swap(array_, other.array_);
    std::swap(array_capacity_, other.array_capacity_);
    std::swap(size_, other.size_);
    std::swap(hash_, other.hash_);
    std::swap(equal_, other.equal_);
  }

  double load_factor() const noexcept
  {
    if (array_capacity_ == 0)
    {
      return 0.0;
    }
    return static_cast< double >(size_) / static_cast< double >(array_capacity_);
  }

  size_t longest_chain() const
  {
    size_t max_chain = 0;

    for (size_t i = 0; i < array_capacity_; ++i)
    {
      size_t chain_size = array_[i].size();
      if (chain_size > max_chain)
      {
        max_chain = chain_size;
      }
    }

    return max_chain;
  }

  size_t max_load_factor() const noexcept
  {
    return max_load_factor_;
  }

  void max_load_factor(size_t factor) noexcept
  {
    max_load_factor_ = factor;
  }

  size_t max_chain_length() const noexcept
  {
    return max_chain_length_;
  }

  void max_chain_length(size_t length) noexcept
  {
    max_chain_length_ = length;
  }

  void set_rehash_policy(std::function< size_t(size_t) > policy)
  {
    rehash_policy_ = policy;
  }

  void auto_rehash()
  {
    if (load_factor() > max_load_factor_)
    {
      size_t new_capacity = rehash_policy_(array_capacity_);
      rehash(new_capacity);
    }

    if (longest_chain() > max_chain_length_)
    {
      size_t new_capacity = rehash_policy_(array_capacity_);
      rehash(new_capacity);
    }
  }

private:
  List< std::pair< const Key, Value > >* array_;
  size_t array_capacity_;
  size_t size_;
  Hash hash_;
  Equal equal_;

  size_t max_load_factor_;
  size_t max_chain_length_;
  std::function< size_t(size_t) > rehash_policy_;

  size_t index(const Key& key) const
  {
    return hash_(key) % array_capacity_;
  }

  void rehash_impl(size_t new_capacity)
  {
    List< std::pair< const Key, Value > >* new_array = static_cast< List< std::pair< const Key, Value > >* >(
      ::operator new(sizeof(List< std::pair< const Key, Value > >) * new_capacity)
    );

    for (size_t i = 0; i < new_capacity; ++i)
    {
      new (&new_array[i]) List< std::pair< const Key, Value > >();
    }

    for (size_t i = 0; i < array_capacity_; ++i)
    {
      List< std::pair< const Key, Value > >& chain = array_[i];

      while (!chain.empty())
      {
        std::pair< const Key, Value > item = std::move(chain.front());
        chain.pop_front();

        size_t new_idx = hash_(item.first) % new_capacity;
        new_array[new_idx].push_front(std::move(item));
      }

      chain.~List();
    }

    ::operator delete(array_);

    array_ = new_array;
    array_capacity_ = new_capacity;
  }
};

template< class Key, class Value, class Hash, class Equal >
void swap(HashTable< Key, Value, Hash, Equal >& lhs, HashTable< Key, Value, Hash, Equal >& rhs) noexcept
{
  lhs.swap(rhs);
}

}

#endif

