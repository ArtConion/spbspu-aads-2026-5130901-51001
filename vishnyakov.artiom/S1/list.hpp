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

    void splice(LIter< T > pos, List< T >& other) noexcept
    {
      if (other.empty() || &other == this)
      {
        return;
      }

      Node* last = other.pseudoknot_;
      while (last->next_ != other.pseudoknot_)
      {
        last = last->next_;
      }

      Node* first = other.pseudoknot_->next_;

      other.pseudoknot_->next_ = other.pseudoknot_;

      Node* next_after_pos = pos.node_->next_;
      pos.node_->next_ = first;
      last->next_ = next_after_pos;

      size_ += other.size_;
      other.size_ = 0;
    }

    void splice(LIter< T > pos, List< T >& other, LIter< T > it) noexcept
    {
      if (&other == this || other.empty() || it == other.end())
      {
        return;
      }

      Node* node_to_move = it.node_;

      Node* prev = other.pseudoknot_;
      while (prev->next_ != node_to_move)
      {
        prev = prev->next_;
      }

      prev->next_ = node_to_move->next_;
      --other.size_;

      node_to_move->next_ = pos.node_->next_;
      pos.node_->next_ = node_to_move;
      ++size_;
    }

    void splice(LIter< T > pos, List< T >& other, LIter< T > first, LIter< T > last) noexcept
    {
      if (other.empty() || first == last || &other == this)
      {
        return;
      }

      Node* first_node = first.node_;
      Node* last_node = last.node_;

      Node* prev_first = other.pseudoknot_;
      while (prev_first->next_ != first_node)
      {
        prev_first = prev_first->next_;
      }

      prev_first->next_ = last_node;

      size_t count = 0;
      Node* current = first_node;
      while (current != last_node)
      {
        ++count;
        current = current->next_;
      }

      other.size_ -= count;

      Node* next_after_pos = pos.node_->next_;
      pos.node_->next_ = first_node;

      current = first_node;
      while (current->next_ != last_node)
      {
        current = current->next_;
      }
      current->next_ = next_after_pos;

      size_ += count;
    }

    void sort() noexcept
    {
      sort(std::less< T >());
    }

    template< class Compare >
    void sort(Compare comp) noexcept
    {
      if (size_ < 2)
      {
        return;
      }

      List< T > sorted;

      while (!empty())
      {
        Node* node = pseudoknot_->next_;
        pseudoknot_->next_ = node->next_;
        --size_;

        Node* prev = sorted.pseudoknot_;
        Node* current = sorted.pseudoknot_->next_;

        while (current != sorted.pseudoknot_ && comp(current->data_, node->data_))
        {
          prev = current;
          current = current->next_;
        }

        node->next_ = current;
        prev->next_ = node;
        ++sorted.size_;
      }

      swap(sorted);
    }

    void merge(List< T >& other) noexcept
    {
      merge(other, std::less< T >());
    }

    template< class Compare >
    void merge(List< T >& other, Compare comp) noexcept
    {
      if (&other == this || other.empty())
      {
        return;
      }

      List< T > result;

      while (!empty() && !other.empty())
      {
        if (comp(front(), other.front()))
        {
          Node* node = pseudoknot_->next_;
          pseudoknot_->next_ = node->next_;
          --size_;

          Node* last = result.pseudoknot_;
          while (last->next_ != result.pseudoknot_)
          {
            last = last->next_;
          }
          node->next_ = result.pseudoknot_;
          last->next_ = node;
          ++result.size_;
        }
        else
        {
          Node* node = other.pseudoknot_->next_;
          other.pseudoknot_->next_ = node->next_;
          --other.size_;

          Node* last = result.pseudoknot_;
          while (last->next_ != result.pseudoknot_)
          {
            last = last->next_;
          }
          node->next_ = result.pseudoknot_;
          last->next_ = node;
          ++result.size_;
        }
      }

      if (!empty())
      {
        Node* last = result.pseudoknot_;
        while (last->next_ != result.pseudoknot_)
        {
          last = last->next_;
        }
        last->next_ = pseudoknot_->next_;
        while (last->next_ != pseudoknot_)
        {
          last = last->next_;
        }
        last->next_ = result.pseudoknot_;
        result.size_ += size_;
        size_ = 0;
        pseudoknot_->next_ = pseudoknot_;
      }

      if (!other.empty())
      {
        Node* last = result.pseudoknot_;
        while (last->next_ != result.pseudoknot_)
        {
          last = last->next_;
        }
        last->next_ = other.pseudoknot_->next_;
        while (last->next_ != other.pseudoknot_)
        {
          last = last->next_;
        }
        last->next_ = result.pseudoknot_;
        result.size_ += other.size_;
        other.size_ = 0;
        other.pseudoknot_->next_ = other.pseudoknot_;
      }

      swap(result);
    }

    template< class Predicate >
    List< T > partition(Predicate pred) noexcept
    {
      List< T > false_list;

      LIter< T > it = begin();
      while (it != end())
      {
        if (!pred(*it))
        {
          LIter< T > next_it = it;
          ++next_it;

          Node* node = it.node_;
          Node* prev = pseudoknot_;
          while (prev->next_ != node)
          {
            prev = prev->next_;
          }
          prev->next_ = node->next_;
          --size_;

          Node* last = false_list.pseudoknot_;
          while (last->next_ != false_list.pseudoknot_)
          {
            last = last->next_;
          }
          node->next_ = false_list.pseudoknot_;
          last->next_ = node;
          ++false_list.size_;

          it = next_it;
        }
        else
        {
          ++it;
        }
      }

      return false_list;
    }
  };
}

#endif

