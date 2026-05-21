#pragma once

#include <string>
#include <string_view>

namespace bookdb {

struct TransparentStringLess
{
  // 1. This is a tag and it must be present for a comparator to be transparent.
  using is_transparent = void;

  // 2. Need operator()
  template <typename T, typename U>
  bool operator()(const T& lhs, const U& rhs) const
  {
    return lhs < rhs;
  }
};

// =============================================================================

struct TransparentStringEqual
{
  using is_transparent = void;

  template <typename T, typename U>
  bool operator()(const T& lhs, const U& rhs) const
  {
    return lhs == rhs;
  }
};

// =============================================================================

struct TransparentStringHash
{
  using is_transparent = void;

  size_t operator()(const char* str) const
  {
    return std::hash<const char*>{}(str);
  }

  size_t operator()(std::string_view str) const
  {
    return std::hash<std::string_view>{}(str);
  }

  size_t operator()(const std::string& str) const
  {
    return std::hash<std::string>{}(str);
  }
};

}  // namespace bookdb
