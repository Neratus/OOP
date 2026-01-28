#pragma once

#include "List.hpp"
#include <iostream>
#include <list/ListExceptions.hpp>

template <MoveAndCopy T> List<T>::List() {}

template <MoveAndCopy T>
template <ConvertibleIterator<T> Iter>
List<T>::List(Iter &&begin, Iter &&end) {
  for (auto it = begin; it != end; ++it)
    pushBack(*it);
}

template <MoveAndCopy T>
List<T>::List(std::initializer_list<T> list) : List(list.begin(), list.end()) {}

template <MoveAndCopy T>
List<T>::List(size_type size)
  requires DefaultConstructible<T>
{
  resize(size);
}

template <MoveAndCopy T>
List<T>::List(size_type size, const value_type &instance)
  requires CopyConstructible<T>
{
  resize(size, instance);
}

template <MoveAndCopy T> auto List<T>::pushFront(const T &value) -> iterator {
  try {
    head = std::make_shared<ListNode<T>>(value, head);
  } catch (std::bad_alloc e) {
    throw NodeAllocationException(__FILE_NAME__, typeid(*this).name(),
                                  __FUNCTION__, __LINE__);
  }
  return iterator(head);
}

template <MoveAndCopy T> void List<T>::popFront() noexcept {
  if (head != nullptr)
    head = head->getNext();
}

template <MoveAndCopy T> void List<T>::popBack() noexcept {
  if (head->getNext() == nullptr) {
    head = nullptr;
    return;
  }

  auto node = head;
  while (node != nullptr && node->getNext() != nullptr &&
         node->getNext()->getNext() != nullptr)
    node = node->getNext();

  node->setNext(nullptr);
}

template <MoveAndCopy T>
void List<T>::erase(const iterator &iterator) noexcept {
  if (iterator)
    eraseNode(iterator.getPtr());
}

template <MoveAndCopy T>
void List<T>::eraseNode(std::shared_ptr<ListNode<T>> remove) noexcept {
  if (remove == nullptr)
    return;

  auto node = head;
  while (node != nullptr) {
    if (node->getNext() == remove) {
      node->setNext(remove->getNext());
      return;
    }
    node = node->getNext();
    ;
  }
}

template <MoveAndCopy T> auto List<T>::pushBack(const T &value) -> iterator {
  std::shared_ptr<ListNode<T>> node = nullptr;
  try {
    node = std::make_shared<ListNode<T>>(value, nullptr);
  } catch (std::bad_alloc e) {
    throw NodeAllocationException(__FILE_NAME__, typeid(*this).name(),
                                  __FUNCTION__, __LINE__);
  }

  auto tail = head;
  if (tail == nullptr) {
    head = node;
    return iterator(node);
  }

  while (tail->getNext() != nullptr)
    tail = tail->getNext();

  tail->setNext(node);
  return iterator(node);
}

template <MoveAndCopy T> auto List<T>::getSize() const noexcept -> size_type {
  auto node = head;
  size_type size = 0;
  while (node != nullptr) {
    size++;
    node = node->getNext();
  }
  return size;
}

template <MoveAndCopy T> void List<T>::clear() noexcept { head = nullptr; }

template <MoveAndCopy T> auto List<T>::at(size_type index) -> reference {
  validateGet(__LINE__);
  validateIndex(__LINE__);

  auto node = head;
  while (index-- != 0 && node != nullptr)
    node = node->getNext();

  return node->getValueRef();
}

template <MoveAndCopy T>
auto List<T>::at(size_type index) const -> const_reference {
  return at(index);
}

template <MoveAndCopy T>
auto List<T>::operator[](size_type index) -> reference {
  return at(index);
}

template <MoveAndCopy T>
auto List<T>::operator[](size_type index) const -> const_reference {
  return at(index);
}

template <MoveAndCopy T> auto List<T>::getFront() -> reference {
  validateGet(__LINE__);
  return head->getValueRef();
}

template <MoveAndCopy T> auto List<T>::getFront() const -> const_reference {
  return getFront();
}

template <MoveAndCopy T> auto List<T>::getBack() -> reference {
  validateGet(__LINE__);
  auto node = head;
  while (node != nullptr && node->getNext() != nullptr)
    node = node->getNext();

  return node->getValueRef();
}

template <MoveAndCopy T> auto List<T>::getBack() const -> const_reference {
  return getBack();
}

template <MoveAndCopy T>
void List<T>::resize(size_type size)
  requires DefaultConstructible<T>
{
  head = nullptr;
  for (int i = 0; i < size; ++i)
    pushFront(T());
}

template <MoveAndCopy T>
void List<T>::resize(size_type size, const value_type &instance)
  requires CopyConstructible<T>
{
  head = nullptr;
  for (int i = 0; i < size; ++i)
    pushFront(T(instance));
}

template <MoveAndCopy T> List<T> &List<T>::operator=(const List<T> &list) {
  head = nullptr;
  for (auto &it : list)
    pushBack(it);
  return *this;
}

template <MoveAndCopy T> auto List<T>::begin() -> iterator {
  return iterator(head);
}

template <MoveAndCopy T> auto List<T>::end() -> iterator {
  return iterator(nullptr);
}

template <MoveAndCopy T> auto List<T>::begin() const -> const_iterator {
  return const_iterator(head);
}

template <MoveAndCopy T> auto List<T>::end() const -> const_iterator {
  return const_iterator(nullptr);
}

template <MoveAndCopy T> auto List<T>::cbegin() const -> const_iterator {
  return const_iterator(head);
}

template <MoveAndCopy T> auto List<T>::cend() const -> const_iterator {
  return const_iterator(nullptr);
}

template <MoveAndCopy T> void List<T>::validateGet(int line) const {
  if (head == nullptr)
    throw EmptyListException(__FILE_NAME__, typeid(*this).name(), __FUNCTION__,
                             line);
}

template <MoveAndCopy T> void List<T>::validateIndex(int line) const {
  if (head == nullptr)
    throw InvalidIndexAccessException(__FILE_NAME__, typeid(*this).name(),
                                      __FUNCTION__, line);
}
