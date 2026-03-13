#ifndef NODE_HPP
#define NODE_HPP

namespace vishnaykov
{
  template< class T >
  struct Node
  {
    T data_;
    Node * next_;

    explicit Node(const T & value = T(), Node * next = nullptr):
      data_(value),
      next_(next)
    {}

    explicit Node(T&& value, Node* next = nullptr):
      data_(std::move(val)),
      next_(next)
    {}
  };
}

#endif