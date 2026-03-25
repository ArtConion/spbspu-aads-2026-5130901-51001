#ifndef LIST_HPP
#define LIST_HPP

#include "node.hpp"
#include <utility>

namespace vishnyakov
{
  template< class T > class List;

  template< class T >
  class LIter
  {
    friend class List< T >;

    using Node = vishnyakov::Node< T >;

    Node* node_;

  public:
    LIter(Node* node = nullptr):
      node_(node)
    {}

    T& operator*() const
    {
      return node_->data_;
    }

    T* operator->() const
    {
      return &node_->data_;
    }

    LIter< T >& operator++()
    {
      node_ = node_->next_;
      return *this;
    }

    LIter< T > operator++(int)
    {
      LIter< T > tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const LIter< T >& other) const
    {
      return node_ == other.node_;
    }

    bool operator!=(const LIter< T >& other) const
    {
      return !(node_ == other.node_);
    }

    operator LIter< const T >() const
    {
      return LIter< const T >(const_cast< const Node* >(node_));
    }
  };

  template< class T >
  class LCIter
  {
    friend class List< T >;

    using Node = vishnyakov::Node< T >;

    const Node* node_;

  public:
    LCIter(const Node* node = nullptr):
      node_(node)
    {}

    LCIter(const LIter< T >& it):
      node_(it.node_)
    {}

    const T& operator*() const
    {
      return node_->data_;
    }

    const T* operator->() const
    {
      return &node_->data_;
    }

    const LCIter< T >& operator++()
    {
      node_ = node_->next_;
      return *this;
    }

    LCIter< T > operator++(int)
    {
      LCIter< T > tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const LCIter< T >& other) const
    {
      return node_ == other.node_;
    }

    bool operator!=(const LCIter< T >& other) const
    {
      return !(node_ == other.node_);
    }
  };

  template< class T >
  class List
  {
    using Node = vishnyakov::Node< T >;

    Node* pseudoknot_;
    size_t size_;

  public:
    List():
      pseudoknot_(nullptr),
      size_(0)
    {
      pseudoknot_ = static_cast< Node* >(::operator new(sizeof(Node)));
      pseudoknot_->next_ = pseudoknot_;
    }

    List(const List& other):
      List()
    {
      for (const T& value : other)
      {
        push_back(value);
      }
    }

    List(List&& other) noexcept:
      pseudoknot_(other.pseudoknot_),
      size_(other.size_)
    {
      other.pseudoknot_ = nullptr;
      other.size_ = 0;
    }

    ~List()
    {
      clear();
      if (pseudoknot_)
      {
        ::operator delete(pseudoknot_);
      }
    }

    List& operator=(const List& other)
    {
      if (this != &other)
      {
        List< T > tmp(other);
        swap(tmp);
      }
      return *this;
    }

    List& operator=(List&& other) noexcept
    {
      if (this != &other)
      {
        clear();
        if (pseudoknot_)
        {
          ::operator delete(pseudoknot_);
        }

        pseudoknot_ = other.pseudoknot_;
        size_ = other.size_;

        other.pseudoknot_ = nullptr;
        other.size_ = 0;
      }
      return *this;
    }

    LIter< T > begin() noexcept
    {
      return pseudoknot_ ? LIter< T >(pseudoknot_->next_) : LIter< T >(nullptr);
    }

    LIter< T > end() noexcept
    {
      return LIter< T >(pseudoknot_);
    }

    LCIter< T > begin() const noexcept
    {
      return pseudoknot_ ? LCIter< T >(pseudoknot_->next_) : LCIter< T >(nullptr);
    }

    LCIter< T > end() const noexcept
    {
      return LCIter< T >(pseudoknot_);
    }

    LCIter< T > cbegin() const noexcept
    {
      return pseudoknot_ ? LCIter< T >(pseudoknot_->next_) : LCIter< T >(nullptr);
    }

    LCIter< T > cend() const noexcept
    {
      return LCIter< T >(pseudoknot_);
    }

    T& front()
    {
      return pseudoknot_->next_->data_;
    }

    const T& front() const
    {
      return pseudoknot_->next_->data_;
    }

    T& back()
    {
      Node* current = pseudoknot_->next_;
      while (current->next_ != pseudoknot_)
      {
        current = current->next_;
      }
      return current->data_;
    }

    const T& back() const
    {
      Node* current = pseudoknot_->next_;
      while (current->next_ != pseudoknot_)
      {
        current = current->next_;
      }
      return current->data_;
    }

    void push_front(const T& value)
    {
      insert_after(LIter< T >(pseudoknot_), value);
    }

    void push_front(T&& value)
    {
      insert_after(LIter< T >(pseudoknot_), std::move(value));
    }

    void push_back(const T& value)
    {
      Node* last = pseudoknot_;
      while (last->next_ != pseudoknot_)
      {
        last = last->next_;
      }
      insert_after(LIter< T >(last), value);
    }

    void push_back(T&& value)
    {
      Node* last = pseudoknot_;
      while (last->next_ != pseudoknot_)
      {
        last = last->next_;
      }
      insert_after(LIter< T >(last), std::move(value));
    }

    LIter< T > insert_after(LIter< T > pos, const T& value)
    {
      Node* new_node = new Node(value, pos.node_->next_);
      pos.node_->next_ = new_node;
      ++size_;
      return LIter< T >(new_node);
    }

    LIter< T > insert_after(LIter< T > pos, T&& value)
    {
      Node* new_node = new Node(std::move(value), pos.node_->next_);
      pos.node_->next_ = new_node;
      ++size_;
      return LIter< T >(new_node);
    }

    void pop_front()
    {
      if (empty())
      {
        return;
      }
      erase_after(LIter< T >(pseudoknot_));
    }

    void pop_back()
    {
      if (empty())
      {
        return;
      }

      Node* prev = pseudoknot_;
      Node* current = pseudoknot_->next_;

      while (current->next_ != pseudoknot_)
      {
        prev = current;
        current = current->next_;
      }

      erase_after(LIter< T >(prev));
    }

    LIter< T > erase_after(LIter< T > pos)
    {
      if (pos.node_->next_ == pseudoknot_)
      {
        return end();
      }

      Node* to_delete = pos.node_->next_;
      pos.node_->next_ = to_delete->next_;
      delete to_delete;
      --size_;

      return LIter< T >(pos.node_->next_);
    }

    void clear()
    {
      while (!empty())
      {
        pop_front();
      }
    }

    bool empty() const
    {
      return size_ == 0;
    }

    size_t size() const
    {
      return size_;
    }

    void swap(List< T >& other) noexcept
    {
      std::swap(pseudoknot_, other.pseudoknot_);
      std::swap(size_, other.size_);
    }
  };
}

#endif

