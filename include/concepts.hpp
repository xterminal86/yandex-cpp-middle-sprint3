#pragma once

#include <concepts>
#include <iterator>

#include "book.hpp"

namespace bookdb {

template <typename T>
concept BookContainerLike = requires (T t)
{
  std::begin(t);
  std::end(t);
};

// Я не знаю зачем это и что здесь писать, выглядит как какие-то велосипеды.
template <typename T>
concept BookIterator = true;

template <typename S, typename I>
concept BookSentinel = true;

template <typename P>
concept BookPredicate = true;

template <typename C>
concept BookComparator = true;

}  // namespace bookdb
