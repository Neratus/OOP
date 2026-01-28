#pragma once

#include <iostream>
#include <iterator>

#include "Concepts.hpp"
#include "Exceptions.h"

template <CopyNMoveable T> using UniquePtr = std::unique_ptr<T>;

template <typename T> using SharedPtr = std::shared_ptr<T>;

template <typename T> using WeakPtr = std::weak_ptr<T>;

template <CopyNMoveable T> class Matrix;

template <CopyNMoveable T> class Iterator {
public:
  using iterator_category = std::random_access_iterator_tag;
  Iterator(Matrix<T> &matrix, const size_t i = 0, const size_t j = 0)
      : index(i * matrix.getColumns() + j), data(matrix.getDataPtr()) {}
  Iterator(const Iterator &It) = default;

  T *operator->();
  const T *operator->() const;

  T &operator*();
  const T &operator*() const;

  operator bool() const;

  Iterator<T> &operator++();
  Iterator<T> operator++(int);

  Iterator<T> &operator--();
  Iterator<T> operator--(int);

  T &operator[](int ind);
  const T &operator[](int ind) const;

  Iterator<T> &operator=(const Iterator<T> &it);

  Iterator<T> &operator+=(int value);
  Iterator<T> &operator-=(int value);
  Iterator<T> operator+(int value) const;
  Iterator<T> operator-(int value) const;

  bool operator!=(const Iterator<T> &iterator) const;
  bool operator==(const Iterator<T> &iterator) const;
  bool operator<(const Iterator<T> &iterator) const;
  bool operator<=(const Iterator<T> &iterator) const;
  bool operator>(const Iterator<T> &iterator) const;
  bool operator>=(const Iterator<T> &iterator) const;

  T &value();
  const T &value() const;

  Iterator<T> &next();

  bool isEnd();
  bool isStart();

private:
  size_t index;

  WeakPtr<typename Matrix<T>::MatrixRow[]> data;

  bool _isIndexValid() const;
  bool _isExpired() const;
  void _checkIterValid(const char filename[], int line, const char inf[]) const;

  size_t _getRows() const;
  size_t _getCols() const;
};

#include "Iterator.hpp"
