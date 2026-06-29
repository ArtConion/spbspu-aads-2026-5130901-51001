#ifndef CUCKOO_HT_HPP
#define CUCKOO_HT_HPP

#include <cstddef>
#include <utility>
#include <stdexcept>
#include <algorithm>
#include <functional>
#include <siphash.hpp>

namespace vishnyakov
{
  template< class Key, class Value, class Hash = SipHash, class Equal = std::equal_to< Key > >
  class CuckooHashTable
  {
  public:
    CuckooHashTable();
    explicit CuckooHashTable(size_t initial_capacity);
    CuckooHashTable(const CuckooHashTable& other);
    CuckooHashTable(CuckooHashTable&& other) noexcept;
    ~CuckooHashTable();

    CuckooHashTable& operator=(const CuckooHashTable& other);
    CuckooHashTable& operator=(CuckooHashTable&& other) noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;
    size_t capacity() const noexcept;

    void add(const Key& key, const Value& value);
    void add(Key&& key, Value&& value);
    Value drop(const Key& key);
    bool has(const Key& key) const;
    Value& at(const Key& key);
    const Value& at(const Key& key) const;
    Value& operator[](const Key& key);

    void clear() noexcept;
    void swap(CuckooHashTable& other) noexcept;

  private:
    static constexpr size_t DEFAULT_CAPACITY = 16;
    static constexpr size_t MAX_LOOP = 100;
    static constexpr size_t GOLDEN_RATIO = 2654435761U;
    static constexpr double MAX_LOAD_FACTOR = 0.5;

    struct Bucket
    {
      Key key;
      Value value;
      bool occupied;

      Bucket();
    };

    Bucket* table1_;
    Bucket* table2_;
    size_t table_size_;
    size_t size_;
    Hash hash1_;
    Hash hash2_;
    Equal equal_;

    size_t index1(const Key& key) const;
    size_t index2(const Key& key) const;
    void rehash();
    size_t find_bucket(const Key& key) const;
    bool try_insert(Key& key, Value& value, size_t max_loop);
    void allocate_tables(size_t size);
    void deallocate_tables();
    void copy_from(const CuckooHashTable& other);
  };

  template< class Key, class Value, class Hash, class Equal >
  CuckooHashTable< Key, Value, Hash, Equal >::Bucket::Bucket():
    key(),
    value(),
    occupied(false)
  {
  }

  template< class Key, class Value, class Hash, class Equal >
  void CuckooHashTable< Key, Value, Hash, Equal >::allocate_tables(size_t size)
  {
    Bucket* new_table1 = nullptr;
    Bucket* new_table2 = nullptr;

    try
    {
      new_table1 = new Bucket[size];
      new_table2 = new Bucket[size];
    }
    catch (...)
    {
      delete[] new_table1;
      delete[] new_table2;
      throw;
    }

    table1_ = new_table1;
    table2_ = new_table2;
    table_size_ = size;
  }

  template< class Key, class Value, class Hash, class Equal >
  void CuckooHashTable< Key, Value, Hash, Equal >::deallocate_tables()
  {
    delete[] table1_;
    delete[] table2_;
    table1_ = nullptr;
    table2_ = nullptr;
    table_size_ = 0;
  }

  template< class Key, class Value, class Hash, class Equal >
  void CuckooHashTable< Key, Value, Hash, Equal >::copy_from(const CuckooHashTable& other)
  {
    for (size_t i = 0; i < table_size_; ++i)
    {
      table1_[i] = other.table1_[i];
      table2_[i] = other.table2_[i];
    }
    size_ = other.size_;
  }

  template< class Key, class Value, class Hash, class Equal >
  CuckooHashTable< Key, Value, Hash, Equal >::CuckooHashTable():
    table1_(nullptr),
    table2_(nullptr),
    table_size_(0),
    size_(0),
    hash1_(),
    hash2_(),
    equal_()
  {
    allocate_tables(DEFAULT_CAPACITY);
  }

  template< class Key, class Value, class Hash, class Equal >
  CuckooHashTable< Key, Value, Hash, Equal >::CuckooHashTable(size_t initial_capacity):
    table1_(nullptr),
    table2_(nullptr),
    table_size_(0),
    size_(0),
    hash1_(),
    hash2_(),
    equal_()
  {
    size_t cap = (initial_capacity > 0) ? initial_capacity : DEFAULT_CAPACITY;
    allocate_tables(cap);
  }

  template< class Key, class Value, class Hash, class Equal >
  CuckooHashTable< Key, Value, Hash, Equal >::CuckooHashTable(const CuckooHashTable& other):
    table1_(nullptr),
    table2_(nullptr),
    table_size_(other.table_size_),
    size_(0),
    hash1_(other.hash1_),
    hash2_(other.hash2_),
    equal_(other.equal_)
  {
    allocate_tables(other.table_size_);
    copy_from(other);
  }

  template< class Key, class Value, class Hash, class Equal >
  CuckooHashTable< Key, Value, Hash, Equal >::CuckooHashTable(CuckooHashTable&& other) noexcept:
    table1_(other.table1_),
    table2_(other.table2_),
    table_size_(other.table_size_),
    size_(other.size_),
    hash1_(std::move(other.hash1_)),
    hash2_(std::move(other.hash2_)),
    equal_(std::move(other.equal_))
  {
    other.table1_ = nullptr;
    other.table2_ = nullptr;
    other.table_size_ = 0;
    other.size_ = 0;
  }

  template< class Key, class Value, class Hash, class Equal >
  CuckooHashTable< Key, Value, Hash, Equal >::~CuckooHashTable()
  {
    deallocate_tables();
  }

  template< class Key, class Value, class Hash, class Equal >
  CuckooHashTable< Key, Value, Hash, Equal >&
  CuckooHashTable< Key, Value, Hash, Equal >::operator=(const CuckooHashTable& other)
  {
    if (this != &other)
    {
      CuckooHashTable tmp(other);
      swap(tmp);
    }
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  CuckooHashTable< Key, Value, Hash, Equal >&
  CuckooHashTable< Key, Value, Hash, Equal >::operator=(CuckooHashTable&& other) noexcept
  {
    if (this != &other)
    {
      deallocate_tables();

      table1_ = other.table1_;
      table2_ = other.table2_;
      table_size_ = other.table_size_;
      size_ = other.size_;
      hash1_ = std::move(other.hash1_);
      hash2_ = std::move(other.hash2_);
      equal_ = std::move(other.equal_);

      other.table1_ = nullptr;
      other.table2_ = nullptr;
      other.table_size_ = 0;
      other.size_ = 0;
    }
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool CuckooHashTable< Key, Value, Hash, Equal >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t CuckooHashTable< Key, Value, Hash, Equal >::size() const noexcept
  {
    return size_;
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t CuckooHashTable< Key, Value, Hash, Equal >::capacity() const noexcept
  {
    return table_size_;
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t CuckooHashTable< Key, Value, Hash, Equal >::index1(const Key& key) const
  {
    return hash1_(key) % table_size_;
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t CuckooHashTable< Key, Value, Hash, Equal >::index2(const Key& key) const
  {
    return (hash2_(key) * GOLDEN_RATIO) % table_size_;
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t CuckooHashTable< Key, Value, Hash, Equal >::find_bucket(const Key& key) const
  {
    size_t idx1 = index1(key);

    if (table1_[idx1].occupied && equal_(table1_[idx1].key, key))
    {
      return idx1;
    }

    size_t idx2 = index2(key);

    if (table2_[idx2].occupied && equal_(table2_[idx2].key, key))
    {
      return idx2 + table_size_;
    }

    return table_size_ * 2;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool CuckooHashTable< Key, Value, Hash, Equal >::try_insert(Key& key, Value& value,
      size_t max_loop)
  {
    for (size_t loop = 0; loop < max_loop; ++loop)
    {
      size_t idx1 = index1(key);

      if (!table1_[idx1].occupied)
      {
        table1_[idx1].key = std::move(key);
        table1_[idx1].value = std::move(value);
        table1_[idx1].occupied = true;
        ++size_;
        return true;
      }

      if (equal_(table1_[idx1].key, key))
      {
        table1_[idx1].value = std::move(value);
        return true;
      }

      std::swap(key, table1_[idx1].key);
      std::swap(value, table1_[idx1].value);

      size_t idx2 = index2(key);

      if (!table2_[idx2].occupied)
      {
        table2_[idx2].key = std::move(key);
        table2_[idx2].value = std::move(value);
        table2_[idx2].occupied = true;
        ++size_;
        return true;
      }

      if (equal_(table2_[idx2].key, key))
      {
        table2_[idx2].value = std::move(value);
        return true;
      }

      std::swap(key, table2_[idx2].key);
      std::swap(value, table2_[idx2].value);
    }

    return false;
  }

  template< class Key, class Value, class Hash, class Equal >
  void CuckooHashTable< Key, Value, Hash, Equal >::rehash()
  {
    Bucket* old_table1 = table1_;
    Bucket* old_table2 = table2_;
    size_t old_size = table_size_;
    size_t old_count = size_;

    size_t new_size = table_size_ * 2;
    table1_ = nullptr;
    table2_ = nullptr;
    table_size_ = 0;
    size_ = 0;

    try
    {
      allocate_tables(new_size);
    }
    catch (...)
    {
      table1_ = old_table1;
      table2_ = old_table2;
      table_size_ = old_size;
      size_ = old_count;
      throw;
    }

    bool success = true;

    for (size_t i = 0; i < old_size && success; ++i)
    {
      if (old_table1[i].occupied)
      {
        Key key = old_table1[i].key;
        Value value = old_table1[i].value;

        if (!try_insert(key, value, MAX_LOOP * 2))
        {
          success = false;
        }
      }

      if (success && old_table2[i].occupied)
      {
        Key key = old_table2[i].key;
        Value value = old_table2[i].value;

        if (!try_insert(key, value, MAX_LOOP * 2))
        {
          success = false;
        }
      }
    }

    delete[] old_table1;
    delete[] old_table2;

    if (!success)
    {
      CuckooHashTable new_table(table_size_ * 2);

      for (size_t i = 0; i < table_size_; ++i)
      {
        if (table1_[i].occupied)
        {
          new_table.add(table1_[i].key, table1_[i].value);
        }

        if (table2_[i].occupied)
        {
          new_table.add(table2_[i].key, table2_[i].value);
        }
      }

      swap(new_table);
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  void CuckooHashTable< Key, Value, Hash, Equal >::add(const Key& key, const Value& value)
  {
    size_t bucket = find_bucket(key);

    if (bucket != table_size_ * 2)
    {
      if (bucket < table_size_)
      {
        table1_[bucket].value = value;
      }
      else
      {
        table2_[bucket - table_size_].value = value;
      }
      return;
    }

    if (static_cast<double>(size_) / table_size_ >= MAX_LOAD_FACTOR)
    {
      rehash();
    }

    Key k = key;
    Value v = value;

    if (!try_insert(k, v, MAX_LOOP))
    {
      rehash();
      add(key, value);
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  void CuckooHashTable< Key, Value, Hash, Equal >::add(Key&& key, Value&& value)
  {
    size_t bucket = find_bucket(key);

    if (bucket != table_size_ * 2)
    {
      if (bucket < table_size_)
      {
        table1_[bucket].value = std::move(value);
      }
      else
      {
        table2_[bucket - table_size_].value = std::move(value);
      }
      return;
    }

    if (static_cast<double>(size_) / table_size_ >= MAX_LOAD_FACTOR)
    {
      rehash();
    }

    Key k = std::move(key);
    Value v = std::move(value);

    if (!try_insert(k, v, MAX_LOOP))
    {
      rehash();
      add(std::move(k), std::move(v));
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  Value CuckooHashTable< Key, Value, Hash, Equal >::drop(const Key& key)
  {
    const size_t NOT_FOUND = table_size_ * 2;
    size_t bucket = find_bucket(key);

    if (bucket == NOT_FOUND)
    {
      throw std::out_of_range("Key not found");
    }

    const bool in_table1 = (bucket < table_size_);
    const size_t idx = in_table1 ? bucket : (bucket - table_size_);

    Value result;

    if (in_table1)
    {
      result = std::move(table1_[idx].value);
      table1_[idx].occupied = false;
      table1_[idx].key.~Key();
      new (&table1_[idx].key) Key();
    }
    else
    {
      result = std::move(table2_[idx].value);
      table2_[idx].occupied = false;
      table2_[idx].key.~Key();
      new (&table2_[idx].key) Key();
    }

    --size_;
    return result;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool CuckooHashTable< Key, Value, Hash, Equal >::has(const Key& key) const
  {
    return find_bucket(key) != table_size_ * 2;
  }

  template< class Key, class Value, class Hash, class Equal >
  Value& CuckooHashTable< Key, Value, Hash, Equal >::at(const Key& key)
  {
    size_t bucket = find_bucket(key);

    if (bucket == table_size_ * 2)
    {
      throw std::out_of_range("Key not found");
    }

    if (bucket < table_size_)
    {
      return table1_[bucket].value;
    }
    else
    {
      return table2_[bucket - table_size_].value;
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  const Value& CuckooHashTable< Key, Value, Hash, Equal >::at(const Key& key) const
  {
    size_t bucket = find_bucket(key);

    if (bucket == table_size_ * 2)
    {
      throw std::out_of_range("Key not found");
    }

    if (bucket < table_size_)
    {
      return table1_[bucket].value;
    }
    else
    {
      return table2_[bucket - table_size_].value;
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  Value& CuckooHashTable< Key, Value, Hash, Equal >::operator[](const Key& key)
  {
    size_t bucket = find_bucket(key);

    if (bucket != table_size_ * 2)
    {
      if (bucket < table_size_)
      {
        return table1_[bucket].value;
      }
      else
      {
        return table2_[bucket - table_size_].value;
      }
    }

    add(key, Value());

    bucket = find_bucket(key);

    if (bucket < table_size_)
    {
      return table1_[bucket].value;
    }
    else
    {
      return table2_[bucket - table_size_].value;
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  void CuckooHashTable< Key, Value, Hash, Equal >::clear() noexcept
  {
    for (size_t i = 0; i < table_size_; ++i)
    {
      if (table1_[i].occupied)
      {
        table1_[i].occupied = false;
        table1_[i].key.~Key();
        new (&table1_[i].key) Key();
      }

      if (table2_[i].occupied)
      {
        table2_[i].occupied = false;
        table2_[i].key.~Key();
        new (&table2_[i].key) Key();
      }
    }

    size_ = 0;
  }

  template< class Key, class Value, class Hash, class Equal >
  void CuckooHashTable< Key, Value, Hash, Equal >::swap(CuckooHashTable& other) noexcept
  {
    std::swap(table1_, other.table1_);
    std::swap(table2_, other.table2_);
    std::swap(table_size_, other.table_size_);
    std::swap(size_, other.size_);
    std::swap(hash1_, other.hash1_);
    std::swap(hash2_, other.hash2_);
    std::swap(equal_, other.equal_);
  }

  template< class Key, class Value, class Hash, class Equal >
  void swap(CuckooHashTable< Key, Value, Hash, Equal >& lhs,
          CuckooHashTable< Key, Value, Hash, Equal >& rhs) noexcept
  {
    lhs.swap(rhs);
  }
}

#endif

