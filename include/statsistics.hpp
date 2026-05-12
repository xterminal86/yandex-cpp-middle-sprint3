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

using Histogram = std::flat_map<std::string, size_t>;

template <BookContainerLike T, typename Comparator = TransparentStringLess>
Histogram buildAuthorHistogramFlat(const BookDatabase<T>& cont, Comparator comp = {})
{
  Histogram stats;

  std::span<const Book> s = cont.GetBooks();

  for (const Book& i : s)
  {
    stats[std::string(i.Author)]++;
  }

  return stats;
}

template <BookIterator It, BookSentinel<It> Sent>
auto calculateGenreRatings(It first, Sent last)
{
  Histogram ratingsSumByGenre;

  for (const Book& i : s)
  {
    ratingsSumByGenre[std::string(i.Genre)] += i.Rating;
  }

  Histogram out;

  for (const auto& kvp : ratingsSumByGenre)
  {
    size_t cnt = std::count_if(
      ratingsSumByGenre.begin(),
      ratingsSumByGenre.end(),
      [&kvp](const auto& p)
      {
        return kvp.first == p.first;
      }
    );

    out[kvp.first] = (double)kvp.second / (double)cnt;
  }

  return out;
}


/*
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
      format_to(fc.out(), "{{ ");

      bool first = true;
      for (const auto& kvp : h)
      {
        if (not first)
        {
          format_to(fc.out(), ", ");
        }

        format_to(fc.out(), "\"{}\" : {}", kvp.first, kvp.second);
        first = false;
      }

      format_to(fc.out(), " }}");

      return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx)
    {
      return ctx.begin();
    }
};
} // namespace std