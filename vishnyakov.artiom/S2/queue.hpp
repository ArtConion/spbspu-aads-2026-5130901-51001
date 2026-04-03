#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "../common/list.hpp"
#include <stdexcept>

namespace vishnyakov
{
  template< class T >
  class Queue
  {
  public:
    Queue() = default;
    Queue(const Queue&) = default;
    Queue(Queue&&) noexcept = default;
    ~Queue() = default;

    Queue& operator=(const Queue&) = default;
    Queue& operator=(Queue&&) noexcept = default;

    void push(const T& value)
    {
      data_.push_back(value);
    }

    void push(T&& value)
    {
      data_.push_back(std::move(value));
    }

    T pop()
    {
      if (empty())
      {
        throw std::underflow_error("Queue is empty");
      }
      T value = std::move(data_.front());
      data_.pop_front();
      return value;
    }

    const T& front() const
    {
      if (empty())
      {
        throw std::underflow_error("Queue is empty");
      }
      return data_.front();
    }

    const T& back() const
    {
      if (empty())
      {
        throw std::underflow_error("Queue is empty");
      }
      return data_.back();
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

    void swap(Queue& other) noexcept
    {
      data_.swap(other.data_);
    }

  private:
    List< T > data_;
  };
}

#endif

