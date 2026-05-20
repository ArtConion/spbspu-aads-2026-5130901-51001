#ifndef CUCKOO_HT_HPP
#define CUCKOO_HT_HPP

#include <cstddef>
#include <utility>
#include <stdexcept>
#include <algorithm>
#include <functional>
#include "../common/siphash.hpp"

namespace vishnyakov
{
  template< class Key, class Value, class Hash = SipHash, class Equal = std::equal_to< Key > >
  class CuckooHashTable
  {
  public:
    CuckooHashTable();
    explicit CuckooHashTable(std::size_t initial_capacity);
    CuckooHashTable(const CuckooHashTable& other);
    CuckooHashTable(CuckooHashTable&& other) noexcept;
    ~CuckooHashTable();

    CuckooHashTable& operator=(const CuckooHashTable& other);
    CuckooHashTable& operator=(CuckooHashTable&& other) noexcept;

    bool empty() const noexcept;
    std::size_t size() const noexcept;
    std::size_t capacity() const noexcept;

    void add(const Key& key, const Value& value);
    void add(Key&& key, Value&& value);
    Value drop(const Key& key);
    bool has(const Key& key) const;
    Value& at(const Key& key);
    const Value& at(const Key& key) const;
    Value& operator[](const Key& key);

    void clear() noexcept;

  private:
    static constexpr std::size_t DEFAULT_CAPACITY = 16;
    static constexpr std::size_t MAX_LOOP = 100;
    static constexpr std::size_t GOLDEN_RATIO = 2654435761U;

    struct Bucket
    {
      Key key;
      Value value;
      bool occupied;

      Bucket(): key(), value(), occupied(false) {}
    };

    Bucket* table1_;
    Bucket* table2_;
    std::size_t table_size_;
    std::size_t size_;
    Hash hash1_;
    Hash hash2_;
    Equal equal_;

    std::size_t index1(const Key& key) const;
    std::size_t index2(const Key& key) const;
    void rehash();
    int find_bucket(const Key& key) const;
  };

  template< class Key, class Value, class Hash, class Equal >
  CuckooHashTable< Key, Value, Hash, Equal >::CuckooHashTable():
    table1_(nullptr),
    table2_(nullptr),
    table_size_(DEFAULT_CAPACITY),
    size_(0),
    hash1_(),
    hash2_(),
    equal_()
  {
    table1_ = new Bucket[table_size_];
    table2_ = new Bucket[table_size_];
  }

  template< class Key, class Value, class Hash, class Equal >
  CuckooHashTable< Key, Value, Hash, Equal >::CuckooHashTable(std::size_t initial_capacity):
    table1_(nullptr),
    table2_(nullptr),
    table_size_(initial_capacity > 0 ? initial_capacity : DEFAULT_CAPACITY),
    size_(0),
    hash1_(),
    hash2_(),
    equal_()
  {
    table1_ = new Bucket[table_size_];
    table2_ = new Bucket[table_size_];
  }

  template< class Key, class Value, class Hash, class Equal >
  CuckooHashTable< Key, Value, Hash, Equal >::CuckooHashTable(const CuckooHashTable& other):
    table1_(nullptr),
    table2_(nullptr),
    table_size_(other.table_size_),
    size_(other.size_),
    hash1_(other.hash1_),
    hash2_(other.hash2_),
    equal_(other.equal_)
  {
    table1_ = new Bucket[table_size_];
    table2_ = new Bucket[table_size_];
    for (std::size_t i = 0; i < table_size_; ++i)
    {
      table1_[i] = other.table1_[i];
      table2_[i] = other.table2_[i];
    }
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
    delete[] table1_;
    delete[] table2_;
  }

  template< class Key, class Value, class Hash, class Equal >
  CuckooHashTable< Key, Value, Hash, Equal >& CuckooHashTable< Key, Value, Hash, Equal >::operator=(const CuckooHashTable& other)
  {
    if (this != &other)
    {
      CuckooHashTable tmp(other);
      std::swap(table1_, tmp.table1_);
      std::swap(table2_, tmp.table2_);
      std::swap(table_size_, tmp.table_size_);
      std::swap(size_, tmp.size_);
      std::swap(hash1_, tmp.hash1_);
      std::swap(hash2_, tmp.hash2_);
      std::swap(equal_, tmp.equal_);
    }
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  CuckooHashTable< Key, Value, Hash, Equal >& CuckooHashTable< Key, Value, Hash, Equal >::operator=(CuckooHashTable&& other) noexcept
  {
    if (this != &other)
    {
      delete[] table1_;
      delete[] table2_;
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
  std::size_t CuckooHashTable< Key, Value, Hash, Equal >::size() const noexcept
  {
    return size_;
  }

  template< class Key, class Value, class Hash, class Equal >
  std::size_t CuckooHashTable< Key, Value, Hash, Equal >::capacity() const noexcept
  {
    return table_size_;
  }

  template< class Key, class Value, class Hash, class Equal >
  std::size_t CuckooHashTable< Key, Value, Hash, Equal >::index1(const Key& key) const
  {
    return hash1_(key) % table_size_;
  }

  template< class Key, class Value, class Hash, class Equal >
  std::size_t CuckooHashTable< Key, Value, Hash, Equal >::index2(const Key& key) const
  {
    return (hash2_(key) * GOLDEN_RATIO) % table_size_;
  }

  template< class Key, class Value, class Hash, class Equal >
  int CuckooHashTable< Key, Value, Hash, Equal >::find_bucket(const Key& key) const
  {
    std::size_t idx1 = index1(key);
    if (table1_[idx1].occupied && equal_(table1_[idx1].key, key))
    {
      return static_cast<int>(idx1);
    }

    std::size_t idx2 = index2(key);
    if (table2_[idx2].occupied && equal_(table2_[idx2].key, key))
    {
      return static_cast<int>(idx2 + table_size_);
    }

    return -1;
  }

  template< class Key, class Value, class Hash, class Equal >
  void CuckooHashTable< Key, Value, Hash, Equal >::add(const Key& key, const Value& value)
  {
    if (has(key))
    {
      return;
    }

    if (size_ >= table_size_)
    {
      rehash();
    }

    Key k = key;
    Value v = value;

    for (std::size_t loop = 0; loop < MAX_LOOP; ++loop)
    {
      std::size_t idx1 = index1(k);
      if (!table1_[idx1].occupied)
      {
        table1_[idx1].key = k;
        table1_[idx1].value = v;
        table1_[idx1].occupied = true;
        ++size_;
        return;
      }
      std::swap(k, table1_[idx1].key);
      std::swap(v, table1_[idx1].value);

      std::size_t idx2 = index2(k);
      if (!table2_[idx2].occupied)
      {
        table2_[idx2].key = k;
        table2_[idx2].value = v;
        table2_[idx2].occupied = true;
        ++size_;
        return;
      }
      std::swap(k, table2_[idx2].key);
      std::swap(v, table2_[idx2].value);
    }

    rehash();
    add(key, value);
  }

  template< class Key, class Value, class Hash, class Equal >
  void CuckooHashTable< Key, Value, Hash, Equal >::add(Key&& key, Value&& value)
  {
    add(key, value);
  }

  template< class Key, class Value, class Hash, class Equal >
  void CuckooHashTable< Key, Value, Hash, Equal >::rehash()
  {
    std::size_t new_size = table_size_ * 2;
    CuckooHashTable new_table(new_size);

    for (std::size_t i = 0; i < table_size_; ++i)
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

    std::swap(table1_, new_table.table1_);
    std::swap(table2_, new_table.table2_);
    std::swap(table_size_, new_table.table_size_);
    std::swap(size_, new_table.size_);
  }

  template< class Key, class Value, class Hash, class Equal >
  void CuckooHashTable< Key, Value, Hash, Equal >::clear() noexcept
  {
    for (std::size_t i = 0; i < table_size_; ++i)
    {
      table1_[i].occupied = false;
      table2_[i].occupied = false;
    }
    size_ = 0;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool CuckooHashTable< Key, Value, Hash, Equal >::has(const Key& key) const
  {
    return find_bucket(key) != -1;
  }

  template< class Key, class Value, class Hash, class Equal >
  Value& CuckooHashTable< Key, Value, Hash, Equal >::at(const Key& key)
  {
    int bucket = find_bucket(key);
    if (bucket == -1)
    {
      throw std::out_of_range("Key not found");
    }
    if (bucket < static_cast<int>(table_size_))
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
    int bucket = find_bucket(key);
    if (bucket == -1)
    {
      throw std::out_of_range("Key not found");
    }
    if (bucket < static_cast<int>(table_size_))
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
    int bucket = find_bucket(key);
    if (bucket != -1)
    {
      if (bucket < static_cast<int>(table_size_))
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
    if (bucket < static_cast<int>(table_size_))
    {
      return table1_[bucket].value;
    }
    else
    {
      return table2_[bucket - table_size_].value;
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  Value CuckooHashTable< Key, Value, Hash, Equal >::drop(const Key& key)
  {
    int bucket = find_bucket(key);
    if (bucket == -1)
    {
      throw std::out_of_range("Key not found");
    }
    Value result;
    if (bucket < static_cast<int>(table_size_))
    {
      result = std::move(table1_[bucket].value);
      table1_[bucket].occupied = false;
    }
    else
    {
      result = std::move(table2_[bucket - table_size_].value);
      table2_[bucket - table_size_].occupied = false;
    }
    --size_;
    return result;
  }
}

#endif

