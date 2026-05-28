#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>

namespace vishnyakov
{
  template< class T >
  class Vector
  {
  public:
    using iterator = T*;
    using const_iterator = const T*;

    Vector();
    explicit Vector(std::size_t count);
    Vector(std::size_t count, const T& value);
    Vector(const Vector& other);
    Vector(Vector&& other) noexcept;
    ~Vector();

    Vector& operator=(const Vector& other);
    Vector& operator=(Vector&& other) noexcept;

    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    bool empty() const noexcept;
    std::size_t size() const noexcept;
    std::size_t capacity() const noexcept;

    void reserve(std::size_t new_cap);
    void shrink_to_fit();

    T& operator[](std::size_t index) noexcept;
    const T& operator[](std::size_t index) const noexcept;
    T& at(std::size_t index);
    const T& at(std::size_t index) const;

    T& front() noexcept;
    const T& front() const noexcept;
    T& back() noexcept;
    const T& back() const noexcept;

    void push_back(const T& value);
    void push_back(T&& value);
    void pop_back();

    iterator insert(iterator pos, const T& value);
    iterator insert(iterator pos, T&& value);
    iterator insert(iterator pos, std::size_t count, const T& value);

    iterator erase(iterator pos);
    iterator erase(iterator first, iterator last);

    void clear() noexcept;
    void swap(Vector& other) noexcept;

  private:
    T* data_;
    std::size_t size_;
    std::size_t capacity_;

    void reallocate(std::size_t new_capacity);
  };

  template< class T >
  Vector< T >::Vector():
    data_(nullptr),
    size_(0),
    capacity_(0)
  {
  }

  template< class T >
  Vector< T >::Vector(std::size_t count):
    data_(nullptr),
    size_(0),
    capacity_(0)
  {
    if (count > 0)
    {
      reserve(count);
      size_ = count;
      for (std::size_t i = 0; i < size_; ++i)
      {
        new (data_ + i) T();
      }
    }
  }

  template< class T >
  Vector< T >::Vector(std::size_t count, const T& value):
    data_(nullptr),
    size_(0),
    capacity_(0)
  {
    if (count > 0)
    {
      reserve(count);
      size_ = count;
      for (std::size_t i = 0; i < size_; ++i)
      {
        new (data_ + i) T(value);
      }
    }
  }

  template< class T >
  Vector< T >::Vector(const Vector& other):
    data_(nullptr),
    size_(0),
    capacity_(0)
  {
    if (other.size_ > 0)
    {
      reserve(other.size_);
      size_ = other.size_;
      for (std::size_t i = 0; i < size_; ++i)
      {
        new (data_ + i) T(other.data_[i]);
      }
    }
  }

  template< class T >
  Vector< T >::Vector(Vector&& other) noexcept:
    data_(other.data_),
    size_(other.size_),
    capacity_(other.capacity_)
  {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }

  template< class T >
  Vector< T >::~Vector()
  {
    clear();
    ::operator delete(data_);
  }

  template< class T >
  Vector< T >& Vector< T >::operator=(const Vector& other)
  {
    if (this != &other)
    {
      Vector tmp(other);
      swap(tmp);
    }
    return *this;
  }

  template< class T >
  Vector< T >& Vector< T >::operator=(Vector&& other) noexcept
  {
    if (this != &other)
    {
      clear();
      ::operator delete(data_);
      data_ = other.data_;
      size_ = other.size_;
      capacity_ = other.capacity_;
      other.data_ = nullptr;
      other.size_ = 0;
      other.capacity_ = 0;
    }
    return *this;
  }

  template< class T >
  bool operator==(const Vector< T >& lhs, const Vector< T >& rhs)
  {
    if (lhs.size() != rhs.size())
    {
      return false;
    }
    for (size_t i = 0; i < lhs.size(); ++i)
    {
      if (lhs[i] != rhs[i])
      {
        return false;
      }
    }
    return true;
  }

  template< class T >
  bool operator!=(const Vector< T >& lhs, const Vector< T >& rhs)
  {
    return !(lhs == rhs);
  }

  template< class T >
  typename Vector< T >::iterator Vector< T >::begin() noexcept
  {
    return data_;
  }

  template< class T >
  typename Vector< T >::iterator Vector< T >::end() noexcept
  {
    return data_ + size_;
  }

  template< class T >
  typename Vector< T >::const_iterator Vector< T >::begin() const noexcept
  {
    return data_;
  }

  template< class T >
  typename Vector< T >::const_iterator Vector< T >::end() const noexcept
  {
    return data_ + size_;
  }

  template< class T >
  typename Vector< T >::const_iterator Vector< T >::cbegin() const noexcept
  {
    return data_;
  }

  template< class T >
  typename Vector< T >::const_iterator Vector< T >::cend() const noexcept
  {
    return data_ + size_;
  }

  template< class T >
  bool Vector< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class T >
  std::size_t Vector< T >::size() const noexcept
  {
    return size_;
  }

  template< class T >
  std::size_t Vector< T >::capacity() const noexcept
  {
    return capacity_;
  }

  template< class T >
  void Vector< T >::reserve(std::size_t new_cap)
  {
    if (new_cap <= capacity_)
    {
      return;
    }
    reallocate(new_cap);
  }

  template< class T >
  void Vector< T >::shrink_to_fit()
  {
    if (size_ == capacity_)
    {
      return;
    }
    if (size_ == 0)
    {
      clear();
      ::operator delete(data_);
      data_ = nullptr;
      capacity_ = 0;
      return;
    }
    reallocate(size_);
  }

  template< class T >
  T& Vector< T >::operator[](std::size_t index) noexcept
  {
    return data_[index];
  }

  template< class T >
  const T& Vector< T >::operator[](std::size_t index) const noexcept
  {
    return data_[index];
  }

  template< class T >
  T& Vector< T >::at(std::size_t index)
  {
    if (index >= size_)
    {
      throw std::out_of_range("Vector::at: index out of range");
    }
    return data_[index];
  }

  template< class T >
  const T& Vector< T >::at(std::size_t index) const
  {
    if (index >= size_)
    {
      throw std::out_of_range("Vector::at: index out of range");
    }
    return data_[index];
  }

  template< class T >
  T& Vector< T >::front() noexcept
  {
    return data_[0];
  }

  template< class T >
  const T& Vector< T >::front() const noexcept
  {
    return data_[0];
  }

  template< class T >
  T& Vector< T >::back() noexcept
  {
    return data_[size_ - 1];
  }

  template< class T >
  const T& Vector< T >::back() const noexcept
  {
    return data_[size_ - 1];
  }

  template< class T >
  void Vector< T >::push_back(const T& value)
  {
    if (size_ == capacity_)
    {
      std::size_t new_cap = (capacity_ == 0) ? 1 : capacity_ * 2;
      reallocate(new_cap);
    }
    new (data_ + size_) T(value);
    ++size_;
  }

  template< class T >
  void Vector< T >::push_back(T&& value)
  {
    if (size_ == capacity_)
    {
      std::size_t new_cap = (capacity_ == 0) ? 1 : capacity_ * 2;
      reallocate(new_cap);
    }
    new (data_ + size_) T(std::move(value));
    ++size_;
  }

  template< class T >
  void Vector< T >::pop_back()
  {
    if (size_ > 0)
    {
      --size_;
      data_[size_].~T();
    }
  }

  template< class T >
  typename Vector< T >::iterator Vector< T >::insert(iterator pos, const T& value)
  {
    std::size_t index = pos - begin();
    if (size_ == capacity_)
    {
      std::size_t new_cap = (capacity_ == 0) ? 1 : capacity_ * 2;
      reallocate(new_cap);
    }
    for (std::size_t i = size_; i > index; --i)
    {
      new (data_ + i) T(std::move(data_[i - 1]));
      data_[i - 1].~T();
    }
    new (data_ + index) T(value);
    ++size_;
    return begin() + index;
  }

  template< class T >
  typename Vector< T >::iterator Vector< T >::insert(iterator pos, T&& value)
  {
    std::size_t index = pos - begin();
    if (size_ == capacity_)
    {
      std::size_t new_cap = (capacity_ == 0) ? 1 : capacity_ * 2;
      reallocate(new_cap);
    }
    for (std::size_t i = size_; i > index; --i)
    {
      new (data_ + i) T(std::move(data_[i - 1]));
      data_[i - 1].~T();
    }
    new (data_ + index) T(std::move(value));
    ++size_;
    return begin() + index;
  }

  template< class T >
  typename Vector< T >::iterator Vector< T >::insert(iterator pos, std::size_t count, const T& value)
  {
    if (count == 0)
    {
      return pos;
    }
    std::size_t index = pos - begin();
    if (size_ + count > capacity_)
    {
      std::size_t new_cap = size_ + count;
      if (new_cap < capacity_ * 2)
      {
        new_cap = capacity_ * 2;
      }
      reallocate(new_cap);
    }
    for (std::size_t i = size_; i > index; --i)
    {
      new (data_ + i + count - 1) T(std::move(data_[i - 1]));
      data_[i - 1].~T();
    }
    for (std::size_t i = 0; i < count; ++i)
    {
      new (data_ + index + i) T(value);
    }
    size_ += count;
    return begin() + index;
  }

  template< class T >
  typename Vector< T >::iterator Vector< T >::erase(iterator pos)
  {
    std::size_t index = pos - begin();
    data_[index].~T();
    for (std::size_t i = index + 1; i < size_; ++i)
    {
      new (data_ + i - 1) T(std::move(data_[i]));
      data_[i].~T();
    }
    --size_;
    return begin() + index;
  }

  template< class T >
  typename Vector< T >::iterator Vector< T >::erase(iterator first, iterator last)
  {
    if (first == last)
    {
      return first;
    }
    std::size_t first_index = first - begin();
    std::size_t last_index = last - begin();
    std::size_t count = last_index - first_index;
    for (std::size_t i = first_index; i < last_index; ++i)
    {
      data_[i].~T();
    }
    for (std::size_t i = last_index; i < size_; ++i)
    {
      new (data_ + i - count) T(std::move(data_[i]));
      data_[i].~T();
    }
    size_ -= count;
    return begin() + first_index;
  }

  template< class T >
  void Vector< T >::clear() noexcept
  {
    for (std::size_t i = 0; i < size_; ++i)
    {
      data_[i].~T();
    }
    size_ = 0;
  }

  template< class T >
  void Vector< T >::swap(Vector& other) noexcept
  {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
  }

  template< class T >
  void Vector< T >::reallocate(std::size_t new_capacity)
  {
    T* new_data = static_cast< T* >(::operator new(new_capacity * sizeof(T)));
    for (std::size_t i = 0; i < size_; ++i)
    {
      new (new_data + i) T(std::move(data_[i]));
      data_[i].~T();
    }
    ::operator delete(data_);
    data_ = new_data;
    capacity_ = new_capacity;
  }

  template< class T >
  void swap(Vector< T >& lhs, Vector< T >& rhs) noexcept
  {
    lhs.swap(rhs);
  }
}

#endif

