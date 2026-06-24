#ifndef MULTISET_HPP
#define MULTISET_HPP

#include "bstree.hpp"

namespace vishnyakov
{
  template< class Key, class Compare = std::less< Key > >
  class Multiset
  {
  public:
    using key_type = Key;
    using value_type = Key;
    using iterator = BSTIter< Key, int, Compare >;
    using const_iterator = BSTCIter< Key, int, Compare >;

    Multiset() = default;
    Multiset(const Multiset&) = default;
    Multiset(Multiset&&) = default;
    ~Multiset() = default;

    Multiset& operator=(const Multiset&) = default;
    Multiset& operator=(Multiset&&) = default;

    iterator begin() noexcept { return tree_.begin(); }
    iterator end() noexcept { return tree_.end(); }
    const_iterator begin() const noexcept { return tree_.begin(); }
    const_iterator end() const noexcept { return tree_.end(); }
    const_iterator cbegin() const noexcept { return tree_.cbegin(); }
    const_iterator cend() const noexcept { return tree_.cend(); }

    bool empty() const noexcept { return tree_.empty(); }
    size_t size() const noexcept { return tree_.size(); }

    void insert(const Key& key)
    {
      if (!tree_.has(key))
      {
        tree_.push(key, 1);
      }
      else
      {
        int& count = tree_.at(key);
        ++count;
      }
    }

    void insert(Key&& key)
    {
      if (!tree_.has(key))
      {
        tree_.push(std::move(key), 1);
      }
      else
      {
        int& count = tree_.at(key);
        ++count;
      }
    }

    size_t count(const Key& key) const
    {
      if (!tree_.has(key))
      {
        return 0;
      }
      return tree_.at(key);
    }

    bool has(const Key& key) const
    {
      return tree_.has(key);
    }

    void erase_one(const Key& key)
    {
      if (!tree_.has(key))
      {
        return;
      }

      int& count = tree_.at(key);
      --count;

      if (count == 0)
      {
        tree_.drop(key);
      }
    }

    void erase_all(const Key& key)
    {
      if (tree_.has(key))
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

    void swap(Multiset& other) noexcept
    {
      tree_.swap(other.tree_);
    }

  private:
    BSTree< Key, int, Compare > tree_;
  };
}

#endif

