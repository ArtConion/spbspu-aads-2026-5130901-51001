#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include "../common/list.hpp"
#include <cstddef>
#include <utility>
#include <stdexcept>
#include <algorithm>

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
    struct Impl
    {
      List< std::pair< const Key, Value > >* array_;
      std::size_t array_capacity_;
      std::size_t current_index_;
      typename List< std::pair< const Key, Value > >::Iter current_iter_;
    };

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
    struct Impl
    {
      const List< std::pair< const Key, Value > >* array_;
      std::size_t array_capacity_;
      std::size_t current_index_;
      typename List< std::pair< const Key, Value > >::CIter current_iter_;
    };

    Impl* impl_;
  };

  template < class Key, class Value, class Hash, class Equal >
  class HashTable
  {
  public:
    HashTable()
      : array_(nullptr),
        array_capacity_(16),
        size_(0),
        hash_(),
        equal_()
    {
      array_ = static_cast< List< std::pair< const Key, Value > >* >(
        ::operator new(sizeof(List< std::pair< const Key, Value > >) * array_capacity_)
      );

      for (std::size_t i = 0; i < array_capacity_; ++i)
      {
        new (&array_[i]) List< std::pair< const Key, Value > >();
      }
    }

    explicit HashTable(std::size_t initial_capacity)
      : array_(nullptr),
        array_capacity_(initial_capacity > 0 ? initial_capacity : 16),
        size_(0),
        hash_(),
        equal_()
    {
      array_ = static_cast< List< std::pair< const Key, Value > >* >(
        ::operator new(sizeof(List< std::pair< const Key, Value > >) * array_capacity_)
      );

      for (std::size_t i = 0; i < array_capacity_; ++i)
      {
        new (&array_[i]) List< std::pair< const Key, Value > >();
      }
    }

    HashTable(const HashTable& other)
      : array_(nullptr),
        array_capacity_(other.array_capacity_),
        size_(other.size_),
        hash_(other.hash_),
        equal_(other.equal_)
    {
      array_ = static_cast< List< std::pair< const Key, Value > >* >(
        ::operator new(sizeof(List< std::pair< const Key, Value > >) * array_capacity_)
      );

      for (std::size_t i = 0; i < array_capacity_; ++i)
      {
        new (&array_[i]) List< std::pair< const Key, Value > >(other.array_[i]);
      }
    }

    HashTable(HashTable&& other) noexcept
      : array_(other.array_),
        array_capacity_(other.array_capacity_),
        size_(other.size_),
        hash_(std::move(other.hash_)),
        equal_(std::move(other.equal_))
    {
      other.array_ = nullptr;
      other.array_capacity_ = 0;
      other.size_ = 0;
    }

    ~HashTable()
    {
      if (array_)
      {
        for (std::size_t i = 0; i < array_capacity_; ++i)
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

    Iter< Key, Value, Hash, Equal > begin() noexcept;
    Iter< Key, Value, Hash, Equal > end() noexcept;
    CIter< Key, Value, Hash, Equal > begin() const noexcept;
    CIter< Key, Value, Hash, Equal > end() const noexcept;
    CIter< Key, Value, Hash, Equal > cbegin() const noexcept;
    CIter< Key, Value, Hash, Equal > cend() const noexcept;

    bool empty() const noexcept
    {
      return size_ == 0;
    }

    std::size_t size() const noexcept
    {
      return size_;
    }

    std::size_t capacity() const noexcept
    {
      return array_capacity_;
    }

    Value& operator[](const Key& key);
    Value& at(const Key& key);
    const Value& at(const Key& key) const;

    void add(const Key& key, const Value& value);
    void add(Key&& key, Value&& value);
    Value drop(const Key& key);
    bool has(const Key& key) const;
    void rehash(std::size_t new_capacity);
    void clear() noexcept;

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

  private:
    List< std::pair< const Key, Value > >* array_;
    std::size_t array_capacity_;
    std::size_t size_;
    Hash hash_;
    Equal equal_;

    std::size_t index(const Key& key) const
    {
      return hash_(key) % array_capacity_;
    }

    void rehash_impl(std::size_t new_capacity);
  };

  template < class Key, class Value, class Hash, class Equal >
  void swap(HashTable< Key, Value, Hash, Equal >& lhs, HashTable< Key, Value, Hash, Equal >& rhs) noexcept
  {
    lhs.swap(rhs);
  }
}

#endif

