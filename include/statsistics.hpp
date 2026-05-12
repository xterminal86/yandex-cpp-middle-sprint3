#pragma once

#include <algorithm>
#include <iterator>
#include <random>
#include <stdexcept>
#include <string_view>

#include "book_database.hpp"

#include <print>
#include <flat_map>

namespace bookdb {

using Histogram = std::flat_map<std::string_view, size_t>;

template <BookContainerLike T, typename Comparator = TransparentStringLess>
Histogram buildAuthorHistogramFlat(const BookDatabase<T>& cont, Comparator comp = {})
{
  Histogram stats;

  for (const auto& i : cont.GetBooks())
  {
    stats[i.Author]++;
  }

  return stats;
}
/*
template <BookIterator It, BookSentinel<It> Sent>
auto calculateGenreRatings(It first, Sent last);

template <BookContainerLike T>
double calculateAverageRating(const BookDatabase<T> &books);

template <BookContainerLike T>
auto sampleRandomBooks(const BookDatabase<T> &cont, size_t num);

template <BookContainerLike T, typename Comparator>
auto getTopNBy(BookDatabase<T> &cont, size_t n, Comparator comp);
*/

}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::Histogram> {
    template <typename FormatContext>
    auto format(const bookdb::Histogram& h, FormatContext& fc) const
    {
      format_to(fc.out(), "{{");

      bool first = true;
      for (const auto& kvp : h)
      {
        if (not first)
        {
          format_to(fc.out(), ",");
        }

        format_to(fc.out(), "{} : {}", kvp.first, kvp.second);
        first = false;
      }

      format_to(fc.out(), "}}");

      return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx)
    {
      return ctx.begin();
    }
};
} // namespace std