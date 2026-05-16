#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include "list.hpp"
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
    Iter()
      : impl_(nullptr)
    {}

    Iter(const Iter& other)
      : impl_(nullptr)
    {
      if (other.impl_)
      {
        impl_ = new Impl(*other.impl_);
      }
    }

    Iter& operator=(const Iter& other)
    {
      if (this != &other)
      {
        Iter tmp(other);
        swap(tmp);
      }

      return *this;
    }

    ~Iter()
    {
      delete impl_;
    }

    void swap(Iter& other) noexcept
    {
      std::swap(impl_, other.impl_);
    }

    std::pair< const Key, Value >& operator*()
    {
      if (!impl_ || impl_->current_iter_ == impl_->array_[impl_->current_index_].end())
      {
        throw std::runtime_error("Dereferencing end iterator");
      }

      return *impl_->current_iter_;
    }

    std::pair< const Key, Value >* operator->()
    {
      if (!impl_ || impl_->current_iter_ == impl_->array_[impl_->current_index_].end())
      {
        throw std::runtime_error("Dereferencing end iterator");
      }

      return &(*impl_->current_iter_);
    }

    Iter& operator++()
    {
      if (!impl_)
      {
        throw std::runtime_error("Incrementing invalid iterator");
      }

      ++(impl_->current_iter_);

      while (impl_->current_index_ < impl_->array_capacity_ &&
             impl_->current_iter_ == impl_->array_[impl_->current_index_].end())
      {
        ++(impl_->current_index_);

        if (impl_->current_index_ < impl_->array_capacity_)
        {
          impl_->current_iter_ = impl_->array_[impl_->current_index_].begin();
        }
      }

      return *this;
    }

    Iter operator++(int)
    {
      Iter tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const Iter& other) const
    {
      if (impl_ == nullptr && other.impl_ == nullptr)
      {
        return true;
      }

      if (impl_ == nullptr || other.impl_ == nullptr)
      {
        return false;
      }

      return impl_->array_ == other.impl_->array_ &&
             impl_->current_index_ == other.impl_->current_index_ &&
             impl_->current_iter_ == other.impl_->current_iter_;
    }

    bool operator!=(const Iter& other) const
    {
      return !(*this == other);
    }

  private:
    struct Impl
    {
      List< std::pair< const Key, Value > >* array_;
      std::size_t array_capacity_;
      std::size_t current_index_;
      typename List< std::pair< const Key, Value > >::Iter current_iter_;

      Impl(List< std::pair< const Key, Value > >* arr, std::size_t cap)
        : array_(arr),
          array_capacity_(cap),
          current_index_(0),
          current_iter_()
      {
        while (current_index_ < array_capacity_ &&
               array_[current_index_].empty())
        {
          ++current_index_;
        }

        if (current_index_ < array_capacity_)
        {
          current_iter_ = array_[current_index_].begin();
        }
      }

      Impl(const Impl& other)
        : array_(other.array_),
          array_capacity_(other.array_capacity_),
          current_index_(other.current_index_),
          current_iter_(other.current_iter_)
      {}
    };

    Impl* impl_;

    Iter(List< std::pair< const Key, Value > >* array, std::size_t capacity)
      : impl_(new Impl(array, capacity))
    {}

    static Iter make_end(List< std::pair< const Key, Value > >* array, std::size_t capacity)
    {
      Iter result;
      result.impl_ = new Impl(array, capacity);
      result.impl_->current_index_ = capacity;
      return result;
    }
  };

  template < class Key, class Value, class Hash, class Equal >
  class CIter
  {
    friend class HashTable< Key, Value, Hash, Equal >;

  public:
    CIter()
      : impl_(nullptr)
    {}

    CIter(const CIter& other)
      : impl_(nullptr)
    {
      if (other.impl_)
      {
        impl_ = new Impl(*other.impl_);
      }
    }

    CIter(const Iter< Key, Value, Hash, Equal >& other)
    {
      if (other.impl_ != nullptr)
      {
        impl_ = new Impl(*other.impl_);
      }
    }

    CIter& operator=(const CIter& other)
    {
      if (this != &other)
      {
        CIter tmp(other);
        swap(tmp);
      }

      return *this;
    }

    ~CIter()
    {
      delete impl_;
    }

    void swap(CIter& other) noexcept
    {
      std::swap(impl_, other.impl_);
    }

    const std::pair< const Key, Value >& operator*() const
    {
      if (!impl_ || impl_->current_iter_ == impl_->array_[impl_->current_index_].end())
      {
        throw std::runtime_error("Dereferencing end iterator");
      }

      return *impl_->current_iter_;
    }

    const std::pair< const Key, Value >* operator->() const
    {
      if (!impl_ || impl_->current_iter_ == impl_->array_[impl_->current_index_].end())
      {
        throw std::runtime_error("Dereferencing end iterator");
      }

      return &(*impl_->current_iter_);
    }

    CIter& operator++()
    {
      if (!impl_)
      {
        throw std::runtime_error("Incrementing invalid iterator");
      }

      ++(impl_->current_iter_);

      while (impl_->current_index_ < impl_->array_capacity_ &&
             impl_->current_iter_ == impl_->array_[impl_->current_index_].end())
      {
        ++(impl_->current_index_);

        if (impl_->current_index_ < impl_->array_capacity_)
        {
          impl_->current_iter_ = impl_->array_[impl_->current_index_].begin();
        }
      }

      return *this;
    }

    CIter operator++(int)
    {
      CIter tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const CIter& other) const
    {
      if (impl_ == nullptr && other.impl_ == nullptr)
      {
        return true;
      }

      if (impl_ == nullptr || other.impl_ == nullptr)
      {
        return false;
      }

      return impl_->array_ == other.impl_->array_ &&
             impl_->current_index_ == other.impl_->current_index_ &&
             impl_->current_iter_ == other.impl_->current_iter_;
    }

    bool operator!=(const CIter& other) const
    {
      return !(*this == other);
    }

  private:
    struct Impl
    {
      const List< std::pair< const Key, Value > >* array_;
      std::size_t array_capacity_;
      std::size_t current_index_;
      typename List< std::pair< const Key, Value > >::CIter current_iter_;

      Impl(const List< std::pair< const Key, Value > >* arr, std::size_t cap)
        : array_(arr),
          array_capacity_(cap),
          current_index_(0),
          current_iter_()
      {
        while (current_index_ < array_capacity_ &&
               array_[current_index_].empty())
        {
          ++current_index_;
        }

        if (current_index_ < array_capacity_)
        {
          current_iter_ = array_[current_index_].begin();
        }
      }

      Impl(const Impl& other)
        : array_(other.array_),
          array_capacity_(other.array_capacity_),
          current_index_(other.current_index_),
          current_iter_(other.current_iter_)
      {}
    };

    Impl* impl_;

    CIter(const List< std::pair< const Key, Value > >* array, std::size_t capacity)
      : impl_(new Impl(array, capacity))
    {}

    static CIter make_end(const List< std::pair< const Key, Value > >* array, std::size_t capacity)
    {
      CIter result;
      result.impl_ = new Impl(array, capacity);
      result.impl_->current_index_ = capacity;
      return result;
    }
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

    Iter< Key, Value, Hash, Equal > begin() noexcept
    {
      return Iter< Key, Value, Hash, Equal >(array_, array_capacity_);
    }

    Iter< Key, Value, Hash, Equal > end() noexcept
    {
      return Iter< Key, Value, Hash, Equal >::make_end(array_, array_capacity_);
    }

    CIter< Key, Value, Hash, Equal > begin() const noexcept
    {
      return CIter< Key, Value, Hash, Equal >(array_, array_capacity_);
    }

    CIter< Key, Value, Hash, Equal > end() const noexcept
    {
      return CIter< Key, Value, Hash, Equal >::make_end(array_, array_capacity_);
    }

    CIter< Key, Value, Hash, Equal > cbegin() const noexcept
    {
      return CIter< Key, Value, Hash, Equal >(array_, array_capacity_);
    }

    CIter< Key, Value, Hash, Equal > cend() const noexcept
    {
      return CIter< Key, Value, Hash, Equal >::make_end(array_, array_capacity_);
    }

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

    Value& operator[](const Key& key)
    {
      std::size_t idx = index(key);
      List< std::pair< const Key, Value > >& chain = array_[idx];

      for (std::pair< const Key, Value >& item : chain)
      {
        if (equal_(item.first, key))
        {
          return item.second;
        }
      }

      chain.push_front(std::pair< const Key, Value >(key, Value()));
      ++size_;

      return chain.front().second;
    }

    Value& at(const Key& key)
    {
      std::size_t idx = index(key);
      List< std::pair< const Key, Value > >& chain = array_[idx];

      for (std::pair< const Key, Value >& item : chain)
      {
        if (equal_(item.first, key))
        {
          return item.second;
        }
      }

      throw std::out_of_range("Key not found");
    }

    const Value& at(const Key& key) const
    {
      std::size_t idx = index(key);
      const List< std::pair< const Key, Value > >& chain = array_[idx];

      for (const std::pair< const Key, Value >& item : chain)
      {
        if (equal_(item.first, key))
        {
          return item.second;
        }
      }

      throw std::out_of_range("Key not found");
    }

    void add(const Key& key, const Value& value)
    {
      std::size_t idx = index(key);
      List< std::pair< const Key, Value > >& chain = array_[idx];

      for (std::pair< const Key, Value >& item : chain)
      {
        if (equal_(item.first, key))
        {
          return;
        }
      }

      chain.push_front(std::pair< const Key, Value >(key, value));
      ++size_;
    }

    void add(Key&& key, Value&& value)
    {
      std::size_t idx = index(key);
      List< std::pair< const Key, Value > >& chain = array_[idx];

      for (std::pair< const Key, Value >& item : chain)
      {
        if (equal_(item.first, key))
        {
          return;
        }
      }

      chain.push_front(std::pair< const Key, Value >(std::move(key), std::move(value)));
      ++size_;
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

      typename List< std::pair< const Key, Value > >::Iter prev = chain.begin();
      typename List< std::pair< const Key, Value > >::Iter it = chain.begin();
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
      std::size_t idx = index(key);
      const List< std::pair< const Key, Value > >& chain = array_[idx];

      for (const std::pair< const Key, Value >& item : chain)
      {
        if (equal_(item.first, key))
        {
          return true;
        }
      }

      return false;
    }

    void rehash(std::size_t new_capacity)
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
      for (std::size_t i = 0; i < array_capacity_; ++i)
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

    void rehash_impl(std::size_t new_capacity)
    {
      List< std::pair< const Key, Value > >* new_array = static_cast< List< std::pair< const Key, Value > >* >(
        ::operator new(sizeof(List< std::pair< const Key, Value > >) * new_capacity)
      );

      for (std::size_t i = 0; i < new_capacity; ++i)
      {
        new (&new_array[i]) List< std::pair< const Key, Value > >();
      }

      for (std::size_t i = 0; i < array_capacity_; ++i)
      {
        List< std::pair< const Key, Value > >& chain = array_[i];

        while (!chain.empty())
        {
          std::pair< const Key, Value > item = std::move(chain.front());
          chain.pop_front();

          std::size_t new_idx = hash_(item.first) % new_capacity;
          new_array[new_idx].push_front(std::move(item));
        }

        chain.~List();
      }

      ::operator delete(array_);

      array_ = new_array;
      array_capacity_ = new_capacity;
    }
  };

  template < class Key, class Value, class Hash, class Equal >
  void swap(HashTable< Key, Value, Hash, Equal >& lhs, HashTable< Key, Value, Hash, Equal >& rhs) noexcept
  {
    lhs.swap(rhs);
  }
}

#endif

