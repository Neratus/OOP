#pragma once

#include <concepts>
#include <iterator>

template <typename T1, typename T2>
concept Convertable =
    requires(T1 t1) { T2(t1); } && requires(T2 t2) { T1(t2); };

template <typename T>
concept CopyNMoveable = std::copyable<T> && std::movable<T>;

template <typename T, typename U = T>
concept ElementArithmetic = CopyNMoveable<T> && requires(const T &a, U b) {
  { a + b } -> std::convertible_to<T>;
  { a - b } -> std::convertible_to<T>;
  { a * b } -> std::convertible_to<T>;
  { a / b } -> std::convertible_to<T>;
  { -a } -> std::convertible_to<T>;
};

template <typename U, typename T>
concept MatrixArithmetic =
    CopyNMoveable<U> && CopyNMoveable<T> && requires(const U &u, const T &t) {
      { u + t } -> std::convertible_to<T>;
      { u - t } -> std::convertible_to<T>;
      { u * t } -> std::convertible_to<T>;
      { u / t } -> std::convertible_to<T>;
    };