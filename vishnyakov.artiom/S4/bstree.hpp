#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <cstddef>
#include <utility>
#include <stdexcept>
#include <algorithm>
#include "../common/stack.hpp"

namespace vishnyakov
{
  template< class Key, class Value, class Compare >
  class BSTree;

  template< class Key, class Value, class Compare >
  class BSTIter;

  template< class Key, class Value, class Compare >
  class BSTCIter;

  template< class Key, class Value, class Compare >
  class BSTree
  {
  public:
    using value_type = std::pair< const Key, Value >;
    using iterator = BSTIter< Key, Value, Compare >;
    using const_iterator = BSTCIter< Key, Value, Compare >;

    struct Node
    {
      value_type data_;
      Node* left_;
      Node* right_;
      Node* parent_;

      Node(const Key& key, const Value& value, Node* parent = nullptr):
        data_(key, value),
        left_(nullptr),
        right_(nullptr),
        parent_(parent)
      {
      }

      Node(Key&& key, Value&& value, Node* parent = nullptr):
        data_(std::move(key), std::move(value)),
        left_(nullptr),
        right_(nullptr),
        parent_(parent)
      {
      }
    };

    BSTree():
      root_(nullptr),
      size_(0),
      comp_()
    {
    }

    BSTree(const BSTree& other):
      root_(nullptr),
      size_(other.size_),
      comp_(other.comp_)
    {
      root_ = copy_tree(other.root_, nullptr);
    }

    BSTree(BSTree&& other) noexcept:
      root_(other.root_),
      size_(other.size_),
      comp_(std::move(other.comp_))
    {
      other.root_ = nullptr;
      other.size_ = 0;
    }

    ~BSTree()
    {
      clear();
    }

    BSTree& operator=(const BSTree& other)
    {
      if (this != &other)
      {
        BSTree tmp(other);
        swap(tmp);
      }
      return *this;
    }

    BSTree& operator=(BSTree&& other) noexcept
    {
      if (this != &other)
      {
        swap(other);
        other.clear();
      }
      return *this;
    }

    iterator begin() noexcept
    {
      if (!root_)
      {
        return end();
      }

      Node* current = root_;
      Stack< Node* > stack;
      while (current->left_)
      {
        stack.push(current);
        current = current->left_;
      }
      return iterator(current, stack);
    }

    iterator end() noexcept
    {
      return iterator(nullptr);
    }

    const_iterator begin() const noexcept
    {
      if (!root_)
      {
        return end();
      }

      const Node* current = root_;
      Stack< const Node* > stack;
      while (current->left_)
      {
        stack.push(current);
        current = current->left_;
      }
      return const_iterator(current, stack);
    }

    const_iterator end() const noexcept
    {
      return const_iterator(nullptr);
    }

    const_iterator cbegin() const noexcept
    {
      return begin();
    }

    const_iterator cend() const noexcept
    {
      return end();
    }

    bool empty() const noexcept
    {
      return size_ == 0;
    }

    size_t size() const noexcept
    {
      return size_;
    }

    void push(const Key& key, const Value& value)
    {
      if (!root_)
      {
        root_ = new Node(key, value, nullptr);
        ++size_;
        return;
      }

      Node* current = root_;
      Node* parent = nullptr;

      while (current)
      {
        parent = current;
        if (comp_(key, current->data_.first))
        {
          current = current->left_;
        }
        else if (comp_(current->data_.first, key))
        {
          current = current->right_;
        }
        else
        {
          return;
        }
      }

      Node* new_node = new Node(key, value, parent);
      if (comp_(key, parent->data_.first))
      {
        parent->left_ = new_node;
      }
      else
      {
        parent->right_ = new_node;
      }
      ++size_;
    }

    void push(Key&& key, Value&& value)
    {
      if (!root_)
      {
        root_ = new Node(std::move(key), std::move(value), nullptr);
        ++size_;
        return;
      }

      Node* current = root_;
      Node* parent = nullptr;

      while (current)
      {
        parent = current;
        if (comp_(key, current->data_.first))
        {
          current = current->left_;
        }
        else if (comp_(current->data_.first, key))
        {
          current = current->right_;
        }
        else
        {
          return;
        }
      }

      Node* new_node = new Node(std::move(key), std::move(value), parent);
      if (comp_(key, parent->data_.first))
      {
        parent->left_ = new_node;
      }
      else
      {
        parent->right_ = new_node;
      }
      ++size_;
    }

    bool has(const Key& key) const
    {
      return find_node(key) != nullptr;
    }

    Value& at(const Key& key)
    {
      Node* node = find_node(key);
      if (!node)
      {
        throw std::out_of_range("Key not found");
      }
      return node->data_.second;
    }

    const Value& at(const Key& key) const
    {
      Node* node = find_node(key);
      if (!node)
      {
        throw std::out_of_range("Key not found");
      }
      return node->data_.second;
    }

    Value& operator[](const Key& key)
    {
      Node* node = find_node(key);
      if (node)
      {
        return node->data_.second;
      }
      push(key, Value());
      node = find_node(key);
      return node->data_.second;
    }

    Value drop(const Key& key)
    {
      Node* node = find_node(key);
      if (!node)
      {
        throw std::out_of_range("Key not found");
      }
      Value result = std::move(node->data_.second);
      root_ = erase_node(root_, key);
      --size_;
      return result;
    }

    iterator find(const Key& key)
    {
      Node* node = find_node(key);
      return iterator(node);
    }

    const_iterator find(const Key& key) const
    {
      Node* node = find_node(key);
      return const_iterator(node);
    }

    iterator rotate_left(iterator pos)
    {
      Node* node = pos.node_;
      if (!node || !node->right_)
      {
        return pos;
      }

      Node* new_root = rotate_left_impl(node);
      return iterator(new_root, pos.stack_);
    }

    iterator rotate_right(iterator pos)
    {
      Node* node = pos.node_;
      if (!node || !node->left_)
      {
        return pos;
      }

      Node* new_root = rotate_right_impl(node);
      return iterator(new_root, pos.stack_);
    }

    iterator rotate_left_large(iterator pos)
    {
      Node* node = pos.node_;
      if (!node || !node->right_ || !node->right_->left_)
      {
        return pos;
      }

      Node* new_root = rotate_right_impl(node->right_);
      node->right_ = new_root;
      new_root->parent_ = node;
      new_root = rotate_left_impl(node);
      return iterator(new_root, pos.stack_);
    }

    iterator rotate_right_large(iterator pos)
    {
      Node* node = pos.node_;
      if (!node || !node->left_ || !node->left_->right_)
      {
        return pos;
      }

      Node* new_root = rotate_left_impl(node->left_);
      node->left_ = new_root;
      new_root->parent_ = node;
      new_root = rotate_right_impl(node);
      return iterator(new_root, pos.stack_);
    }

    size_t height() const
    {
      return get_height(root_);
    }

    size_t height(const_iterator pos) const
    {
      if (pos.node_)
      {
        return get_height(const_cast< Node* >(pos.node_));
      }
      return 0;
    }

    void clear()
    {
      delete_tree(root_);
      root_ = nullptr;
      size_ = 0;
    }

    void swap(BSTree& other) noexcept
    {
      std::swap(root_, other.root_);
      std::swap(size_, other.size_);
      std::swap(comp_, other.comp_);
    }

    Compare key_comp() const
    {
      return comp_;
    }

  private:
    Node* root_;
    size_t size_;
    Compare comp_;

    Node* copy_tree(Node* node, Node* parent)
    {
      if (!node)
      {
        return nullptr;
      }
      Node* new_node = new Node(node->data_.first, node->data_.second, parent);
      new_node->left_ = copy_tree(node->left_, new_node);
      new_node->right_ = copy_tree(node->right_, new_node);
      return new_node;
    }

    void delete_tree(Node* node)
    {
      if (!node)
      {
        return;
      }
      delete_tree(node->left_);
      delete_tree(node->right_);
      delete node;
    }

    Node* find_node(const Key& key) const
    {
      Node* current = root_;
      while (current)
      {
        if (comp_(key, current->data_.first))
        {
          current = current->left_;
        }
        else if (comp_(current->data_.first, key))
        {
          current = current->right_;
        }
        else
        {
          return current;
        }
      }
      return nullptr;
    }

    Node* min_node(Node* node) const
    {
      while (node && node->left_)
      {
        node = node->left_;
      }
      return node;
    }

    Node* max_node(Node* node) const
    {
      while (node && node->right_)
      {
        node = node->right_;
      }
      return node;
    }

    size_t get_height(Node* node) const
    {
      if (!node)
      {
        return 0;
      }
      return 1 + std::max(get_height(node->left_), get_height(node->right_));
    }

    Node* erase_node(Node* node, const Key& key)
    {
      if (!node)
      {
        return nullptr;
      }

      if (comp_(key, node->data_.first))
      {
        node->left_ = erase_node(node->left_, key);
        if (node->left_)
        {
          node->left_->parent_ = node;
        }
      }
      else if (comp_(node->data_.first, key))
      {
        node->right_ = erase_node(node->right_, key);
        if (node->right_)
        {
          node->right_->parent_ = node;
        }
      }
      else
      {
        if (!node->left_)
        {
          Node* right = node->right_;
          if (right)
          {
            right->parent_ = node->parent_;
          }
          delete node;
          return right;
        }
        else if (!node->right_)
        {
          Node* left = node->left_;
          if (left)
          {
            left->parent_ = node->parent_;
          }
          delete node;
          return left;
        }
        else
        {
          Node* min = min_node(node->right_);
          const_cast< Key& >(node->data_.first) = std::move(min->data_.first);
          node->data_.second = std::move(min->data_.second);
          node->right_ = erase_node(node->right_, min->data_.first);
          if (node->right_)
          {
            node->right_->parent_ = node;
          }
        }
      }
      return node;
    }

    Node* rotate_left_impl(Node* node)
    {
      Node* right = node->right_;
      Node* parent = node->parent_;
      node->right_ = right->left_;
      if (right->left_)
      {
        right->left_->parent_ = node;
      }
      right->left_ = node;
      node->parent_ = right;
      right->parent_ = parent;
      if (parent)
      {
        if (parent->left_ == node)
        {
          parent->left_ = right;
        }
        else
        {
          parent->right_ = right;
        }
      }
      else
      {
        root_ = right;
      }
      return right;
    }

    Node* rotate_right_impl(Node* node)
    {
      Node* left = node->left_;
      Node* parent = node->parent_;
      node->left_ = left->right_;
      if (left->right_)
      {
        left->right_->parent_ = node;
      }
      left->right_ = node;
      node->parent_ = left;
      left->parent_ = parent;
      if (parent)
      {
        if (parent->left_ == node)
        {
          parent->left_ = left;
        }
        else
        {
          parent->right_ = left;
        }
      }
      else
      {
        root_ = left;
      }
      return left;
    }
  };

  template< class Key, class Value, class Compare >
  class BSTIter
  {
    friend class BSTree< Key, Value, Compare >;
    friend class BSTCIter< Key, Value, Compare >;

  public:
    BSTIter():
      node_(nullptr),
      stack_()
    {
    }

    BSTIter(const BSTIter&) = default;
    BSTIter& operator=(const BSTIter&) = default;
    ~BSTIter() = default;

    std::pair< const Key, Value >& operator*()
    {
      return node_->data_;
    }

    std::pair< const Key, Value >* operator->()
    {
      return &node_->data_;
    }

    BSTIter& operator++()
    {
      if (node_->right_)
      {
        node_ = node_->right_;
        while (node_->left_)
        {
          stack_.push(node_);
          node_ = node_->left_;
        }
      }
      else if (!stack_.empty())
      {
        node_ = stack_.pop();
      }
      else
      {
        node_ = nullptr;
      }
      return *this;
    }

    BSTIter operator++(int)
    {
      BSTIter tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const BSTIter& other) const
    {
      return node_ == other.node_;
    }

    bool operator!=(const BSTIter& other) const
    {
      return !(*this == other);
    }

  private:
    using Node = typename BSTree< Key, Value, Compare >::Node;
    Node* node_;
    Stack< Node* > stack_;

    BSTIter(Node* node, const Stack< Node* >& stack = Stack< Node* >()):
      node_(node),
      stack_(stack)
    {
    }
  };

  template< class Key, class Value, class Compare >
  class BSTCIter
  {
    friend class BSTree< Key, Value, Compare >;

  public:
    BSTCIter():
      node_(nullptr),
      stack_()
    {
    }

    BSTCIter(const BSTCIter&) = default;

    BSTCIter(const BSTIter< Key, Value, Compare >& other):
      node_(other.node_),
      stack_()
    {
    }

    ~BSTCIter() = default;

    BSTCIter& operator=(const BSTCIter&) = default;

    const std::pair< const Key, Value >& operator*() const
    {
      return node_->data_;
    }

    const std::pair< const Key, Value >* operator->() const
    {
      return &node_->data_;
    }

    BSTCIter& operator++()
    {
      if (node_->right_)
      {
        node_ = node_->right_;
        while (node_->left_)
        {
          stack_.push(node_);
          node_ = node_->left_;
        }
      }
      else if (!stack_.empty())
      {
        node_ = stack_.pop();
      }
      else
      {
        node_ = nullptr;
      }
      return *this;
    }

    BSTCIter operator++(int)
    {
      BSTCIter tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const BSTCIter& other) const
    {
      return node_ == other.node_;
    }

    bool operator!=(const BSTCIter& other) const
    {
      return !(*this == other);
    }

  private:
    using Node = typename BSTree< Key, Value, Compare >::Node;
    const Node* node_;
    Stack< const Node* > stack_;

    BSTCIter(const Node* node, const Stack< const Node* >& stack = Stack< const Node* >()):
      node_(node),
      stack_(stack)
    {
    }
  };

  template< class Key, class Value, class Compare >
  void swap(BSTree< Key, Value, Compare >& lhs, BSTree< Key, Value, Compare >& rhs) noexcept
  {
    lhs.swap(rhs);
  }
}

#endif

