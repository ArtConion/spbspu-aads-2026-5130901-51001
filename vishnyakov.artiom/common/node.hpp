#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>
#include <utility>

namespace vishnyakov
{
  template< class T >
  struct Node
  {
    T data_;
    Node* next_;

    explicit Node(const T& value, Node* next = nullptr):
      data_(value),
      next_(next)
    {}

    explicit Node(T&& value, Node* next = nullptr):
      data_(std::move(value)),
      next_(next)
    {}

    Node() = delete;
  };
}

#endif

