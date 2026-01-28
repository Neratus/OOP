#pragma once

#include "list/ListConcepts.hpp"
#include <memory>

template <MoveAndCopy T> class ListNode {
public:
  ListNode(const T &value, std::shared_ptr<ListNode<T>> next);

  std::shared_ptr<ListNode<T>> getNext() const;
  void setNext(std::shared_ptr<ListNode<T>> next); // добавлено

  T getValue() const;

  T &getValueRef();
  const T &getValueRef() const;

private:
  T value;
  std::shared_ptr<ListNode<T>> next;
};

#include <list/ListNodeImpl.hpp>
