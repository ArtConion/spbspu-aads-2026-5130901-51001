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

  private:
    Node* find_prev(LIter< T > it) const noexcept
    {
      if (it.node_ == pseudoknot_ || it.node_ == pseudoknot_->next_)
      {
        return pseudoknot_;
      }

      Node* current = pseudoknot_->next_;
      while (current != pseudoknot_ && current->next_ != it.node_)
      {
        current = current->next_;
      }
      return current == pseudoknot_ ? nullptr : current;
    }

  public:
    void splice(LIter< T > pos, List< T >& other) noexcept
    {
      if (other.empty() || &other == this)
      {
        return;
      }

      Node* first_node = other.pseudoknot_->next_;
      Node* last_node = other.pseudoknot_;

      other.pseudoknot_->next_ = other.pseudoknot_;
      size_ += other.size_;
      other.size_ = 0;

      Node* last_current = pos.node_->next_;
      pos.node_->next_ = first_node;

      Node* current = first_node;
      while (current->next_ != last_node)
      {
        current = current->next_;
      }
      current->next_ = last_current;
    }

    void splice(LIter< T > pos, List< T >& other, LIter< T > it) noexcept
    {
      if (&other == this || other.empty() || it == other.end())
      {
        return;
      }

      Node* node_to_move = it.node_;
      Node* prev = other.find_prev(it);

      if (prev)
      {
        prev->next_ = node_to_move->next_;
        --other.size_;
      }

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

      Node* prev_first = other.find_prev(first);
      if (prev_first)
      {
        prev_first->next_ = last_node;
      }
      else
      {
        other.pseudoknot_->next_ = last_node;
      }

      size_t count = 0;
      Node* current = first_node;
      while (current != last_node)
      {
        ++count;
        current = current->next_;
      }
      other.size_ -= count;
      size_ += count;

      Node* last_current = pos.node_->next_;
      pos.node_->next_ = first_node;

      current = first_node;
      while (current->next_ != last_node)
      {
        current = current->next_;
      }
      current->next_ = last_current;
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

      bool swapped;
      Node* end = pseudoknot_;

      do
      {
        swapped = false;
        Node* prev = pseudoknot_;
        Node* current = pseudoknot_->next_;

        while (current != end && current->next_ != pseudoknot_)
        {
          Node* next_node = current->next_;
          if (comp(next_node->data_, current->data_))
          {
            prev->next_ = next_node;
            current->next_ = next_node->next_;
            next_node->next_ = current;

            swapped = true;
          }
          else
          {
            prev = current;
            current = current->next_;
          }
        }
        end = current;
      } while (swapped);
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

      LIter< T > this_it = begin();
      LIter< T > other_it = other.begin();

      while (this_it != end() && other_it != other.end())
      {
        if (comp(*other_it, *this_it))
        {
          LIter< T > next_other = other_it;
          ++next_other;
          splice(this_it, other, other_it);
          other_it = next_other;
        }
        else
        {
          ++this_it;
        }
      }

      if (!other.empty())
      {
        splice(end(), other);
      }
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
          false_list.splice(false_list.end(), *this, it);
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