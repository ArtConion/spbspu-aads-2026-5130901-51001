#ifndef MULTIMAP_HPP
#define MULTIMAP_HPP

#include "bstree.hpp"
#include "list.hpp"
#include <string>
#include <utility>

namespace vishnyakov
{
  template< class Key, class Value, class Compare = std::less< Key > >
  class Multimap
  {
  public:
    using key_type = Key;
    using mapped_type = Value;
    using value_type = std::pair< const Key, Value >;

    class iterator
    {
    public:
      iterator() = default;

      iterator(typename BSTree< Key, vishnyakov::List< Value >, Compare >::iterator tree_it,
               vishnyakov::LIter< Value > list_it)
        : tree_it_(tree_it), list_it_(list_it)
      {
      }

      value_type operator*() const
      {
        return std::make_pair(tree_it_->first, *list_it_);
      }

      iterator& operator++()
      {
        ++list_it_;
        if (list_it_ == tree_it_->second.end())
        {
          ++tree_it_;
          if (tree_it_ != tree_it_.end())
          {
            list_it_ = tree_it_->second.begin();
          }
        }
        return *this;
      }

      iterator operator++(int)
      {
        iterator tmp = *this;
        ++(*this);
        return tmp;
      }

      bool operator==(const iterator& other) const
      {
        return tree_it_ == other.tree_it_ && list_it_ == other.list_it_;
      }

      bool operator!=(const iterator& other) const
      {
        return !(*this == other);
      }

    private:
      typename BSTree< Key, vishnyakov::List< Value >, Compare >::iterator tree_it_;
      vishnyakov::LIter< Value > list_it_;
    };

    class const_iterator
    {
    public:
      const_iterator() = default;

      const_iterator(typename BSTree< Key, vishnyakov::List< Value >, Compare >::const_iterator tree_it,
                     vishnyakov::LCIter< Value > list_it)
        : tree_it_(tree_it), list_it_(list_it)
      {
      }

      value_type operator*() const
      {
        return std::make_pair(tree_it_->first, *list_it_);
      }

      const_iterator& operator++()
      {
        ++list_it_;
        if (list_it_ == tree_it_->second.end())
        {
          ++tree_it_;
          if (tree_it_ != tree_it_.end())
          {
            list_it_ = tree_it_->second.begin();
          }
        }
        return *this;
      }

      const_iterator operator++(int)
      {
        const_iterator tmp = *this;
        ++(*this);
        return tmp;
      }

      bool operator==(const const_iterator& other) const
      {
        return tree_it_ == other.tree_it_ && list_it_ == other.list_it_;
      }

      bool operator!=(const const_iterator& other) const
      {
        return !(*this == other);
      }

    private:
      typename BSTree< Key, vishnyakov::List< Value >, Compare >::const_iterator tree_it_;
      vishnyakov::LCIter< Value > list_it_;
    };

    Multimap() = default;
    Multimap(const Multimap&) = default;
    Multimap(Multimap&&) = default;
    ~Multimap() = default;

    Multimap& operator=(const Multimap&) = default;
    Multimap& operator=(Multimap&&) = default;

    iterator begin()
    {
      auto tree_it = tree_.begin();
      if (tree_it == tree_.end())
      {
        return end();
      }
      return iterator(tree_it, tree_it->second.begin());
    }

    iterator end()
    {
      return iterator(tree_.end(), vishnyakov::LIter< Value >(nullptr));
    }

    const_iterator begin() const
    {
      auto tree_it = tree_.begin();
      if (tree_it == tree_.end())
      {
        return end();
      }
      return const_iterator(tree_it, tree_it->second.begin());
    }

    const_iterator end() const
    {
      return const_iterator(tree_.end(), vishnyakov::LCIter< Value >(nullptr));
    }

    const_iterator cbegin() const
    {
      return begin();
    }

    const_iterator cend() const
    {
      return end();
    }

    bool empty() const noexcept
    {
      return tree_.empty();
    }

    size_t size() const noexcept
    {
      size_t total = 0;
      for (auto it = tree_.begin(); it != tree_.end(); ++it)
      {
        total += it->second.size();
      }
      return total;
    }

    void insert(const Key& key, const Value& value)
    {
      if (!tree_.has(key))
      {
        vishnyakov::List< Value > values;
        values.push_back(value);
        tree_.push(key, values);
      }
      else
      {
        vishnyakov::List< Value >& values = tree_.at(key);
        values.push_back(value);
      }
    }

    void insert(Key&& key, Value&& value)
    {
      if (!tree_.has(key))
      {
        vishnyakov::List< Value > values;
        values.push_back(std::move(value));
        tree_.push(std::move(key), values);
      }
      else
      {
        vishnyakov::List< Value >& values = tree_.at(key);
        values.push_back(std::move(value));
      }
    }

    bool has(const Key& key) const
    {
      return tree_.has(key);
    }

    size_t count(const Key& key) const
    {
      if (!tree_.has(key))
      {
        return 0;
      }
      return tree_.at(key).size();
    }

    const vishnyakov::List< Value >& at(const Key& key) const
    {
      return tree_.at(key);
    }

    void erase_one(const Key& key)
    {
      if (!tree_.has(key))
      {
        return;
      }

      vishnyakov::List< Value >& values = tree_.at(key);
      if (!values.empty())
      {
        values.pop_front();
        if (values.empty())
        {
          tree_.drop(key);
        }
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
      auto tree_it = tree_.find(key);
      if (tree_it == tree_.end())
      {
        return end();
      }
      return iterator(tree_it, tree_it->second.begin());
    }

    const_iterator find(const Key& key) const
    {
      auto tree_it = tree_.find(key);
      if (tree_it == tree_.end())
      {
        return end();
      }
      return const_iterator(tree_it, tree_it->second.begin());
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
    BSTree< Key, vishnyakov::List< Value >, Compare > tree_;
  };
}

#endif

