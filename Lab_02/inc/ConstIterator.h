#pragma once

#include <iostream>
#include <iterator>

#include "Exceptions.h"

template <typename T> using SharedPtr = std::shared_ptr<T>;

template <typename T> using WeakPtr = std::weak_ptr<T>;

template <CopyNMoveable T> class Matrix;

template <CopyNMoveable T> class ConstIterator {
public:
  using iterator_category = std::random_access_iterator_tag;
  ConstIterator(const Matrix<T> &matrix, const size_t i = 0, const size_t j = 0)
      : index(i * matrix.getColumns() + j), data(matrix.getDataPtr()) {}

  ConstIterator(const ConstIterator &It) = default;

  const T *operator->() const;
  const T &operator*() const;

  operator bool() const;

  ConstIterator<T> &operator++();
  ConstIterator<T> operator++(int);

  ConstIterator<T> &operator--();
  ConstIterator<T> operator--(int);

  const T &operator[](int ind) const;
  ConstIterator<T> &operator=(const ConstIterator<T> &it);

  ConstIterator<T> &operator+=(int value);
  ConstIterator<T> &operator-=(int value);
  ConstIterator<T> operator+(int value) const;
  ConstIterator<T> operator-(int value) const;

  bool operator!=(const ConstIterator<T> &iterator) const;
  bool operator==(const ConstIterator<T> &iterator) const;
  bool operator<(const ConstIterator<T> &iterator) const;
  bool operator<=(const ConstIterator<T> &iterator) const;
  bool operator>(const ConstIterator<T> &iterator) const;
  bool operator>=(const ConstIterator<T> &iterator) const;

  const T &value() const;

  ConstIterator<T> &next();

  bool isEnd() const;
  bool isStart() const;

private:
  size_t index;

  WeakPtr<typename Matrix<T>::MatrixRow[]> data;

  bool _isIndexValid() const;
  bool _isExpired() const;
  void _checkIterValid(const char filename[], int line, const char inf[]) const;

  size_t _getRows() const;
  size_t _getCols() const;
};

#include "ConstIterator.hpp"
