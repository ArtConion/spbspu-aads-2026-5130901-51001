#ifndef AVLTREE_HPP
#define AVLTREE_HPP

#include <cstddef>
#include <utility>
#include <stdexcept>
#include <algorithm>

namespace vishnyakov
{
  template< class Key, class Value, class Compare = std::less< Key > >
  class AVLTree
  {
  public:
    using value_type = std::pair< const Key, Value >;

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

    AVLTree():
      root_(nullptr),
      size_(0),
      comp_()
    {
    }

    AVLTree(const AVLTree& other);
    AVLTree(AVLTree&& other) noexcept;
    ~AVLTree();

    AVLTree& operator=(const AVLTree& other);
    AVLTree& operator=(AVLTree&& other) noexcept;

    bool empty() const noexcept;
    std::size_t size() const noexcept;

    void push(const Key& key, const Value& value);
    void push(Key&& key, Value&& value);
    bool has(const Key& key) const;
    Value drop(const Key& key);

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
    Node* max_node(Node* node) const;
  };

  template< class Key, class Value, class Compare >
  void swap(AVLTree< Key, Value, Compare >& lhs, AVLTree< Key, Value, Compare >& rhs) noexcept;
}

#endif

