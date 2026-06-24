#ifndef MAP_HPP
#define MAP_HPP

#include "bstree.hpp"

namespace vishnyakov
{
  template< class Key, class Value, class Compare = std::less< Key > >
  class Map
  {
  public:
    using key_type = Key;
    using mapped_type = Value;
    using value_type = std::pair< const Key, Value >;
    using iterator = BSTIter< Key, Value, Compare >;
    using const_iterator = BSTCIter< Key, Value, Compare >;

    Map() = default;
    Map(const Map&) = default;
    Map(Map&&) = default;
    ~Map() = default;

    Map& operator=(const Map&) = default;
    Map& operator=(Map&&) = default;

    iterator begin() noexcept { return tree_.begin(); }
    iterator end() noexcept { return tree_.end(); }
    const_iterator begin() const noexcept { return tree_.begin(); }
    const_iterator end() const noexcept { return tree_.end(); }
    const_iterator cbegin() const noexcept { return tree_.cbegin(); }
    const_iterator cend() const noexcept { return tree_.cend(); }

    bool empty() const noexcept { return tree_.empty(); }
    size_t size() const noexcept { return tree_.size(); }

    void insert(const Key& key, const Value& value)
    {
      tree_.push(key, value);
    }

    void insert(Key&& key, Value&& value)
    {
      tree_.push(std::move(key), std::move(value));
    }

    bool has(const Key& key) const
    {
      return tree_.has(key);
    }

    Value& at(const Key& key)
    {
      return tree_.at(key);
    }

    const Value& at(const Key& key) const
    {
      return tree_.at(key);
    }

    Value& operator[](const Key& key)
    {
      return tree_[key];
    }

    Value drop(const Key& key)
    {
      return tree_.drop(key);
    }

    iterator find(const Key& key)
    {
      return tree_.find(key);
    }

    const_iterator find(const Key& key) const
    {
      return tree_.find(key);
    }

    void clear()
    {
      tree_.clear();
    }

    void swap(Map& other) noexcept
    {
      tree_.swap(other.tree_);
    }

  private:
    BSTree< Key, Value, Compare > tree_;
  };
}

#endif

