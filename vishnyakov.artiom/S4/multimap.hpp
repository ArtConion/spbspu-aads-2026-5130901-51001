#ifndef MULTIMAP_HPP
#define MULTIMAP_HPP

#include "bstree.hpp"

namespace vishnyakov
{
  template< class Key, class Value, class Compare = std::less< Key > >
  class Multimap
  {
  public:
    using key_type = Key;
    using mapped_type = Value;
    using value_type = std::pair< const Key, Value >;
    using iterator = BSTIter< Key, Value, Compare >;
    using const_iterator = BSTCIter< Key, Value, Compare >;

    Multimap() = default;
    Multimap(const Multimap&) = default;
    Multimap(Multimap&&) = default;
    ~Multimap() = default;

    Multimap& operator=(const Multimap&) = default;
    Multimap& operator=(Multimap&&) = default;

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

    size_t count(const Key& key) const
    {
      size_t cnt = 0;
      for (auto it = tree_.begin(); it != tree_.end(); ++it)
      {
        if (it->first == key)
        {
          ++cnt;
        }
      }
      return cnt;
    }

    const Value& at(const Key& key) const
    {
      return tree_.at(key);
    }

    void erase_one(const Key& key)
    {
      if (!tree_.has(key))
      {
        return;
      }
      tree_.drop(key);
    }

    void erase_all(const Key& key)
    {
      while (tree_.has(key))
      {
        tree_.drop(key);
      }
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

    void swap(Multimap& other) noexcept
    {
      tree_.swap(other.tree_);
    }

  private:
    BSTree< Key, Value, Compare > tree_;
  };
}

#endif

