#ifndef SET_HPP
#define SET_HPP

#include "bstree.hpp"

namespace vishnyakov
{
  template< class Key, class Compare = std::less< Key > >
  class Set
  {
  public:
    using key_type = Key;
    using value_type = Key;
    using iterator = BSTIter< Key, bool, Compare >;
    using const_iterator = BSTCIter< Key, bool, Compare >;

    Set() = default;
    Set(const Set&) = default;
    Set(Set&&) = default;
    ~Set() = default;

    Set& operator=(const Set&) = default;
    Set& operator=(Set&&) = default;

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
      tree_.push(key, true);
    }

    void insert(Key&& key)
    {
      tree_.push(std::move(key), true);
    }

    bool has(const Key& key) const
    {
      return tree_.has(key);
    }

    void erase(const Key& key)
    {
      tree_.drop(key);
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

    void swap(Set& other) noexcept
    {
      tree_.swap(other.tree_);
    }

  private:
    BSTree< Key, bool, Compare > tree_;
  };
}

#endif

