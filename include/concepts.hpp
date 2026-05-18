#pragma once

#include <concepts>
#include <iterator>

#include "book.hpp"

namespace bookdb {

template <typename T>
concept BookContainerLike = requires (T t)
{
  std::is_same_v<typename T::value_type, Book>;

  { t.begin()  } -> std::convertible_to<typename T::iterator>;
  { t.cbegin() } -> std::convertible_to<typename T::const_iterator>;
  { t.end()    } -> std::convertible_to<typename T::const_iterator>;
  { t.end()    } -> std::convertible_to<typename T::iterator>;
  { t.size()   } -> std::convertible_to<typename T::size_type>;
  { t.empty()  } -> std::same_as<bool>;

  t.clear();
};

template <typename T>
concept BookIterator = requires (T it)
{
  { ++it } -> std::same_as<T&>;
  { it++ } -> std::same_as<T>;
  { *it  } -> std::same_as<Book&>;
  { it == it } -> std::convertible_to<bool>;
  { it != it } -> std::convertible_to<bool>;
};

template <typename S, typename I>
concept BookSentinel = requires (S sent, I it)
{
  { it == it } -> std::convertible_to<bool>;
  { it != it } -> std::convertible_to<bool>;
};

template <typename P>
concept BookPredicate = requires (P pred, const Book& b)
{
  { pred(b) } -> std::same_as<bool>;
};

template <typename C>
concept BookComparator = requires(C comp, const Book& b1, const Book& b2)
{
  { comp(b1, b2) } -> std::same_as<bool>;
};

}  // namespace bookdb
