#ifndef STACK_HPP
#define STACK_HPP

#include "../common/list.hpp"
#include <stdexcept>

namespace vishnyakov
{
  template<typename T>
  class Stack
  {
  public:
    Stack() = default;
    Stack(const Stack&) = default;
    Stack(Stack&&) noexcept = default;
    ~Stack() = default;

    Stack& operator=(const Stack&) = default;
    Stack& operator=(Stack&&) noexcept = default;

    void push(const T& value)
    {
      data_.push_front(value);
    }

    void push(T&& value)
    {
      data_.push_front(std::move(value));
    }

    T pop()
    {
      if (empty())
      {
        throw std::underflow_error("Stack is empty");
      }
      T value = std::move(data_.front());
      data_.pop_front();
      return value;
    }

    const T& top() const
    {
      if (empty())
      {
        throw std::underflow_error("Stack is empty");
      }
      return data_.front();
    }

    bool empty() const
    {
      return data_.empty();
    }

    size_t size() const
    {
      return data_.size();
    }

    void clear()
    {
      data_.clear();
    }

    void swap(Stack& other) noexcept
    {
      data_.swap(other.data_);
    }

  private:
    List<T> data_;
  };
}

#endif

