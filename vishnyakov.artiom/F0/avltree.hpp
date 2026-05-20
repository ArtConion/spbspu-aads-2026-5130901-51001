#ifndef AVLTREE_HPP
#define AVLTREE_HPP

#include <cstddef>
#include <utility>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include "../common/stack.hpp"

namespace vishnyakov
{
  template< class Key, class Value, class Compare >
  class AVLTree;

  template< class Key, class Value, class Compare >
  class AVLIter;

  template< class Key, class Value, class Compare >
  class AVLCIter;

  template< class Key, class Value, class Compare = std::less< Key > >
  class AVLTree
  {
  public:
    using value_type = std::pair< const Key, Value >;
    using iterator = AVLIter< Key, Value, Compare >;
    using const_iterator = AVLCIter< Key, Value, Compare >;

    struct Node
    {
      value_type data_;
      Node* left_;
      Node* right_;
      Node* parent_;
      int height_;

      Node(const Key& key, const Value& value, Node* parent = nullptr):
        data_(key, value),
        left_(nullptr),
        right_(nullptr),
        parent_(parent),
        height_(1)
      {
      }

      Node(Key&& key, Value&& value, Node* parent = nullptr):
        data_(std::move(key), std::move(value)),
        left_(nullptr),
        right_(nullptr),
        parent_(parent),
        height_(1)
      {
      }
    };

    AVLTree();
    AVLTree(const AVLTree& other);
    AVLTree(AVLTree&& other) noexcept;
    ~AVLTree();

    AVLTree& operator=(const AVLTree& other);
    AVLTree& operator=(AVLTree&& other) noexcept;

    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    bool empty() const noexcept;
    std::size_t size() const noexcept;

    void push(const Key& key, const Value& value);
    void push(Key&& key, Value&& value);
    bool has(const Key& key) const;
    Value drop(const Key& key);

    iterator find(const Key& key);
    const_iterator find(const Key& key) const;

    void clear();
    void swap(AVLTree& other) noexcept;

    Compare key_comp() const;

  private:
    Node* root_;
    std::size_t size_;
    Compare comp_;

    Node* find_node(const Key& key) const;
    Node* copy_tree(Node* node, Node* parent);
    void delete_tree(Node* node);

    int get_height(Node* node) const;
    int get_balance(Node* node) const;
    void update_height(Node* node);

    Node* rotate_left(Node* node);
    Node* rotate_right(Node* node);
    Node* balance(Node* node);

    Node* insert_node(Node* node, const Key& key, const Value& value);
    Node* insert_node(Node* node, Key&& key, Value&& value);
    Node* erase_node(Node* node, const Key& key);
    Node* min_node(Node* node) const;
  };

  template< class Key, class Value, class Compare >
  class AVLIter
  {
    friend class AVLTree< Key, Value, Compare >;
    friend class AVLCIter< Key, Value, Compare >;

  public:
    AVLIter();
    AVLIter(const AVLIter&) = default;
    AVLIter& operator=(const AVLIter&) = default;
    ~AVLIter() = default;

    std::pair< const Key, Value >& operator*();
    std::pair< const Key, Value >* operator->();

    AVLIter& operator++();
    AVLIter operator++(int);

    bool operator==(const AVLIter& other) const;
    bool operator!=(const AVLIter& other) const;

  private:
    using Node = typename AVLTree< Key, Value, Compare >::Node;
    Node* node_;
    Stack< Node* > stack_;

    AVLIter(Node* node, const Stack< Node* >& stack = Stack< Node* >());
  };

  template< class Key, class Value, class Compare >
  class AVLCIter
  {
    friend class AVLTree< Key, Value, Compare >;

  public:
    AVLCIter();
    AVLCIter(const AVLCIter&) = default;
    AVLCIter(const AVLIter< Key, Value, Compare >& other);
    ~AVLCIter() = default;

    AVLCIter& operator=(const AVLCIter&) = default;

    const std::pair< const Key, Value >& operator*() const;
    const std::pair< const Key, Value >* operator->() const;

    AVLCIter& operator++();
    AVLCIter operator++(int);

    bool operator==(const AVLCIter& other) const;
    bool operator!=(const AVLCIter& other) const;

  private:
    using Node = typename AVLTree< Key, Value, Compare >::Node;
    const Node* node_;
    Stack< const Node* > stack_;

    AVLCIter(const Node* node, const Stack< const Node* >& stack = Stack< const Node* >());
  };

  // ========== AVLTree implementation ==========

  template< class Key, class Value, class Compare >
  AVLTree< Key, Value, Compare >::AVLTree():
    root_(nullptr),
    size_(0),
    comp_()
  {
  }

  template< class Key, class Value, class Compare >
  AVLTree< Key, Value, Compare >::AVLTree(const AVLTree& other):
    root_(nullptr),
    size_(other.size_),
    comp_(other.comp_)
  {
    root_ = copy_tree(other.root_, nullptr);
  }

  template< class Key, class Value, class Compare >
  AVLTree< Key, Value, Compare >::AVLTree(AVLTree&& other) noexcept:
    root_(other.root_),
    size_(other.size_),
    comp_(std::move(other.comp_))
  {
    other.root_ = nullptr;
    other.size_ = 0;
  }

  template< class Key, class Value, class Compare >
  AVLTree< Key, Value, Compare >::~AVLTree()
  {
    clear();
  }

  template< class Key, class Value, class Compare >
  AVLTree< Key, Value, Compare >& AVLTree< Key, Value, Compare >::operator=(const AVLTree& other)
  {
    if (this != &other)
    {
      AVLTree tmp(other);
      swap(tmp);
    }
    return *this;
  }

  template< class Key, class Value, class Compare >
  AVLTree< Key, Value, Compare >& AVLTree< Key, Value, Compare >::operator=(AVLTree&& other) noexcept
  {
    if (this != &other)
    {
      swap(other);
      other.clear();
    }
    return *this;
  }

  template< class Key, class Value, class Compare >
  typename AVLTree< Key, Value, Compare >::iterator AVLTree< Key, Value, Compare >::begin() noexcept
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

  template< class Key, class Value, class Compare >
  typename AVLTree< Key, Value, Compare >::iterator AVLTree< Key, Value, Compare >::end() noexcept
  {
    return iterator(nullptr);
  }

  template< class Key, class Value, class Compare >
  typename AVLTree< Key, Value, Compare >::const_iterator AVLTree< Key, Value, Compare >::begin() const noexcept
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

  template< class Key, class Value, class Compare >
  typename AVLTree< Key, Value, Compare >::const_iterator AVLTree< Key, Value, Compare >::end() const noexcept
  {
    return const_iterator(nullptr);
  }

  template< class Key, class Value, class Compare >
  typename AVLTree< Key, Value, Compare >::const_iterator AVLTree< Key, Value, Compare >::cbegin() const noexcept
  {
    return begin();
  }

  template< class Key, class Value, class Compare >
  typename AVLTree< Key, Value, Compare >::const_iterator AVLTree< Key, Value, Compare >::cend() const noexcept
  {
    return end();
  }

  template< class Key, class Value, class Compare >
  bool AVLTree< Key, Value, Compare >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class Key, class Value, class Compare >
  std::size_t AVLTree< Key, Value, Compare >::size() const noexcept
  {
    return size_;
  }

  template< class Key, class Value, class Compare >
  void AVLTree< Key, Value, Compare >::push(const Key& key, const Value& value)
  {
    if (has(key)) return;
    root_ = insert_node(root_, key, value);
    if (root_)
    {
      root_->parent_ = nullptr;
    }
    ++size_;
  }

  template< class Key, class Value, class Compare >
  void AVLTree< Key, Value, Compare >::push(Key&& key, Value&& value)
  {
    if (has(key)) return;
    root_ = insert_node(root_, std::move(key), std::move(value));
    if (root_)
    {
      root_->parent_ = nullptr;
    }
    ++size_;
  }

  template< class Key, class Value, class Compare >
  bool AVLTree< Key, Value, Compare >::has(const Key& key) const
  {
    return find_node(key) != nullptr;
  }

  template< class Key, class Value, class Compare >
  Value AVLTree< Key, Value, Compare >::drop(const Key& key)
  {
    Node* node = find_node(key);
    if (!node)
    {
      throw std::out_of_range("Key not found");
    }
    Value result = std::move(node->data_.second);
    root_ = erase_node(root_, key);
    if (root_)
    {
      root_->parent_ = nullptr;
    }
    --size_;
    return result;
  }

  template< class Key, class Value, class Compare >
  typename AVLTree< Key, Value, Compare >::iterator AVLTree< Key, Value, Compare >::find(const Key& key)
  {
    Node* node = find_node(key);
    return iterator(node);
  }

  template< class Key, class Value, class Compare >
  typename AVLTree< Key, Value, Compare >::const_iterator AVLTree< Key, Value, Compare >::find(const Key& key) const
  {
    Node* node = find_node(key);
    return const_iterator(node);
  }

  template< class Key, class Value, class Compare >
  void AVLTree< Key, Value, Compare >::clear()
  {
    delete_tree(root_);
    root_ = nullptr;
    size_ = 0;
  }

  template< class Key, class Value, class Compare >
  void AVLTree< Key, Value, Compare >::swap(AVLTree& other) noexcept
  {
    std::swap(root_, other.root_);
    std::swap(size_, other.size_);
    std::swap(comp_, other.comp_);
  }

  template< class Key, class Value, class Compare >
  Compare AVLTree< Key, Value, Compare >::key_comp() const
  {
    return comp_;
  }

  template< class Key, class Value, class Compare >
  typename AVLTree< Key, Value, Compare >::Node* AVLTree< Key, Value, Compare >::find_node(const Key& key) const
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

  template< class Key, class Value, class Compare >
  typename AVLTree< Key, Value, Compare >::Node* AVLTree< Key, Value, Compare >::copy_tree(Node* node, Node* parent)
  {
    if (!node)
    {
      return nullptr;
    }
    Node* new_node = new Node(node->data_.first, node->data_.second, parent);
    new_node->height_ = node->height_;
    new_node->left_ = copy_tree(node->left_, new_node);
    new_node->right_ = copy_tree(node->right_, new_node);
    return new_node;
  }

  template< class Key, class Value, class Compare >
  void AVLTree< Key, Value, Compare >::delete_tree(Node* node)
  {
    if (!node)
    {
      return;
    }
    delete_tree(node->left_);
    delete_tree(node->right_);
    delete node;
  }

  template< class Key, class Value, class Compare >
  int AVLTree< Key, Value, Compare >::get_height(Node* node) const
  {
    return node ? node->height_ : 0;
  }

  template< class Key, class Value, class Compare >
  int AVLTree< Key, Value, Compare >::get_balance(Node* node) const
  {
    return node ? get_height(node->left_) - get_height(node->right_) : 0;
  }

  template< class Key, class Value, class Compare >
  void AVLTree< Key, Value, Compare >::update_height(Node* node)
  {
    if (node)
    {
      node->height_ = 1 + std::max(get_height(node->left_), get_height(node->right_));
    }
  }

  template< class Key, class Value, class Compare >
  typename AVLTree< Key, Value, Compare >::Node* AVLTree< Key, Value, Compare >::rotate_left(Node* node)
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

    update_height(node);
    update_height(right);

    return right;
  }

  template< class Key, class Value, class Compare >
  typename AVLTree< Key, Value, Compare >::Node* AVLTree< Key, Value, Compare >::rotate_right(Node* node)
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

    update_height(node);
    update_height(left);

    return left;
  }

  template< class Key, class Value, class Compare >
  typename AVLTree< Key, Value, Compare >::Node* AVLTree< Key, Value, Compare >::balance(Node* node)
  {
    if (!node)
    {
      return nullptr;
    }

    update_height(node);
    int balance = get_balance(node);

    if (balance > 1)
    {
      if (get_balance(node->left_) < 0)
      {
        node->left_ = rotate_left(node->left_);
        if (node->left_)
        {
          node->left_->parent_ = node;
        }
      }
      return rotate_right(node);
    }

    if (balance < -1)
    {
      if (get_balance(node->right_) > 0)
      {
        node->right_ = rotate_right(node->right_);
        if (node->right_)
        {
          node->right_->parent_ = node;
        }
      }
      return rotate_left(node);
    }

    return node;
  }

  template< class Key, class Value, class Compare >
  typename AVLTree< Key, Value, Compare >::Node* AVLTree< Key, Value, Compare >::insert_node(Node* node, const Key& key, const Value& value)
  {
    if (!node)
    {
      return new Node(key, value);
    }

    if (comp_(key, node->data_.first))
    {
      node->left_ = insert_node(node->left_, key, value);
      if (node->left_)
      {
        node->left_->parent_ = node;
      }
    }
    else if (comp_(node->data_.first, key))
    {
      node->right_ = insert_node(node->right_, key, value);
      if (node->right_)
      {
        node->right_->parent_ = node;
      }
    }
    else
    {
      return node;
    }

    return balance(node);
  }

  template< class Key, class Value, class Compare >
  typename AVLTree< Key, Value, Compare >::Node* AVLTree< Key, Value, Compare >::insert_node(Node* node, Key&& key, Value&& value)
  {
    if (!node)
    {
      return new Node(std::move(key), std::move(value));
    }

    if (comp_(key, node->data_.first))
    {
      node->left_ = insert_node(node->left_, std::move(key), std::move(value));
      if (node->left_)
      {
        node->left_->parent_ = node;
      }
    }
    else if (comp_(node->data_.first, key))
    {
      node->right_ = insert_node(node->right_, std::move(key), std::move(value));
      if (node->right_)
      {
        node->right_->parent_ = node;
      }
    }
    else
    {
      return node;
    }

    return balance(node);
  }

  template< class Key, class Value, class Compare >
  typename AVLTree< Key, Value, Compare >::Node* AVLTree< Key, Value, Compare >::erase_node(Node* node, const Key& key)
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
      if (!node->left_ || !node->right_)
      {
        Node* child = node->left_ ? node->left_ : node->right_;
        if (child)
        {
          child->parent_ = node->parent_;
        }
        delete node;
        return child;
      }
      else
      {
        Node* successor = min_node(node->right_);
        const_cast< Key& >(node->data_.first) = std::move(successor->data_.first);
        node->data_.second = std::move(successor->data_.second);
        node->right_ = erase_node(node->right_, successor->data_.first);
        if (node->right_)
        {
          node->right_->parent_ = node;
        }
      }
    }

    return balance(node);
  }

  template< class Key, class Value, class Compare >
  typename AVLTree< Key, Value, Compare >::Node* AVLTree< Key, Value, Compare >::min_node(Node* node) const
  {
    while (node && node->left_)
    {
      node = node->left_;
    }
    return node;
  }

  // ========== AVLIter implementation ==========

  template< class Key, class Value, class Compare >
  AVLIter< Key, Value, Compare >::AVLIter():
    node_(nullptr),
    stack_()
  {
  }

  template< class Key, class Value, class Compare >
  AVLIter< Key, Value, Compare >::AVLIter(Node* node, const Stack< Node* >& stack):
    node_(node),
    stack_(stack)
  {
  }

  template< class Key, class Value, class Compare >
  std::pair< const Key, Value >& AVLIter< Key, Value, Compare >::operator*()
  {
    return node_->data_;
  }

  template< class Key, class Value, class Compare >
  std::pair< const Key, Value >* AVLIter< Key, Value, Compare >::operator->()
  {
    return &node_->data_;
  }

  template< class Key, class Value, class Compare >
  AVLIter< Key, Value, Compare >& AVLIter< Key, Value, Compare >::operator++()
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

  template< class Key, class Value, class Compare >
  AVLIter< Key, Value, Compare > AVLIter< Key, Value, Compare >::operator++(int)
  {
    AVLIter tmp = *this;
    ++(*this);
    return tmp;
  }

  template< class Key, class Value, class Compare >
  bool AVLIter< Key, Value, Compare >::operator==(const AVLIter& other) const
  {
    return node_ == other.node_;
  }

  template< class Key, class Value, class Compare >
  bool AVLIter< Key, Value, Compare >::operator!=(const AVLIter& other) const
  {
    return !(*this == other);
  }

  // ========== AVLCIter implementation ==========

  template< class Key, class Value, class Compare >
  AVLCIter< Key, Value, Compare >::AVLCIter():
    node_(nullptr),
    stack_()
  {
  }

  template< class Key, class Value, class Compare >
  AVLCIter< Key, Value, Compare >::AVLCIter(const Node* node, const Stack< const Node* >& stack):
    node_(node),
    stack_(stack)
  {
  }

  template< class Key, class Value, class Compare >
  AVLCIter< Key, Value, Compare >::AVLCIter(const AVLIter< Key, Value, Compare >& other):
    node_(other.node_),
    stack_()
  {
  }

  template< class Key, class Value, class Compare >
  const std::pair< const Key, Value >& AVLCIter< Key, Value, Compare >::operator*() const
  {
    return node_->data_;
  }

  template< class Key, class Value, class Compare >
  const std::pair< const Key, Value >* AVLCIter< Key, Value, Compare >::operator->() const
  {
    return &node_->data_;
  }

  template< class Key, class Value, class Compare >
  AVLCIter< Key, Value, Compare >& AVLCIter< Key, Value, Compare >::operator++()
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

  template< class Key, class Value, class Compare >
  AVLCIter< Key, Value, Compare > AVLCIter< Key, Value, Compare >::operator++(int)
  {
    AVLCIter tmp = *this;
    ++(*this);
    return tmp;
  }

  template< class Key, class Value, class Compare >
  bool AVLCIter< Key, Value, Compare >::operator==(const AVLCIter& other) const
  {
    return node_ == other.node_;
  }

  template< class Key, class Value, class Compare >
  bool AVLCIter< Key, Value, Compare >::operator!=(const AVLCIter& other) const
  {
    return node_ != other.node_;
  }

  template< class Key, class Value, class Compare >
  void swap(AVLTree< Key, Value, Compare >& lhs, AVLTree< Key, Value, Compare >& rhs) noexcept
  {
    lhs.swap(rhs);
  }
}

template< class Key, class Value, class Compare >
std::ostream& operator<<(std::ostream& out, const vishnyakov::AVLIter< Key, Value, Compare >&)
{
  out << "AVLIter";
  return out;
}

template< class Key, class Value, class Compare >
std::ostream& operator<<(std::ostream& out, const vishnyakov::AVLCIter< Key, Value, Compare >&)
{
  out << "AVLCIter";
  return out;
}

#endif

