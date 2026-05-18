#pragma once

#include <iostream>
#include <algorithm>
#include <iterator>
#include <random>
#include <stdexcept>
#include <string_view>
#include <random>

#include "book_database.hpp"
#include "comparators.hpp"

#include <print>
#include <flat_map>

namespace bookdb {

// =============================================================================

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

// =============================================================================

using GenreRatings = std::flat_map<Genre, double>;

template <BookIterator It, BookSentinel<It> Sent>
GenreRatings calculateGenreRatings(It first, Sent last)
{
  GenreRatings out;

  std::unordered_map<Genre, std::pair<double, size_t>> ratingsSumByGenre;

  while (first != last)
  {
    // Nice shortcut.
    auto& [sum, count] = ratingsSumByGenre[first->Genre_];
    sum += first->Rating;
    count++;
    first = std::next(first);
  }

  for (const auto& kvp : ratingsSumByGenre)
  {
    const double ratingSum  = kvp.second.first;
    const double totalCount = (double)kvp.second.second;

    out.insert_or_assign(kvp.first, ratingSum / totalCount);
  }

  return out;
}

// =============================================================================

template <BookContainerLike T>
double calculateAverageRating(const BookDatabase<T>& books)
{
  return std::accumulate(
    books.cbegin(),
    books.cend(),
    0.0,
    [](double sum, const Book& b)
    {
      return sum + b.Rating;
    }
  ) / (double)books.size();
}

// =============================================================================

template <BookContainerLike T>
auto sampleRandomBooks(const BookDatabase<T>& cont, size_t num)
{
  std::vector<Book> res;

  if (num > cont.size())
  {
    std::cerr << "Requested range (" << num << ") "
              << "is greater than the amount of books in database "
              << "(" << cont.size() << "). "
              << "Returning empty result."
              << "\n";
    return res;
  }

  std::sample(
    cont.cbegin(),
    cont.cend(),
    std::back_inserter(res),
    num,
    std::mt19937_64(std::random_device{}())
  );

  return res;
}

// =============================================================================

template <BookContainerLike T, typename Comparator>
auto getTopNBy(BookDatabase<T> &cont, size_t n, Comparator comp)
{
  std::vector<Book> out;

  if (n == 0)
  {
    return out;
  }

  // Since we're modifying container in-place we can't use partial sort because
  // it will produce UB if you try to partial sort full range.
  std::sort(cont.begin(), cont.end(), comp);

  size_t counter = 0;
  for (auto& i : cont)
  {
    if (counter >= n)
    {
      break;
    }
    out.push_back(i);
    counter++;
  }

  return out;
}

}  // namespace bookdb

// =============================================================================

namespace std {

template <>
struct formatter<bookdb::Histogram> {
    template <typename FormatContext>
    auto format(const bookdb::Histogram& h, FormatContext& fc) const
    {
      format_to(fc.out(), "{{\n");

      bool first = true;
      for (const auto& kvp : h)
      {
        if (not first)
        {
          format_to(fc.out(), ",\n");
        }
        else
        {
          format_to(fc.out(), "\n");
        }

        format_to(fc.out(), "  \"{}\" : {}", kvp.first, kvp.second);
        first = false;
      }

      format_to(fc.out(), "\n}}");

      return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx)
    {
      return ctx.begin();
    }
};

// -----------------------------------------------------------------------------

template <>
struct formatter<bookdb::GenreRatings> {
    template <typename FormatContext>
    auto format(const bookdb::GenreRatings& gr, FormatContext& fc) const
    {
      format_to(fc.out(), "{{\n");

      bool first = true;
      for (const auto& kvp : gr)
      {
        if (not first)
        {
          format_to(fc.out(), ",\n");
        }
        else
        {
          format_to(fc.out(), "\n");
        }

        format_to(fc.out(), "  \"{}\" : {}", kvp.first, kvp.second);
        first = false;
      }

      format_to(fc.out(), "\n}}");

      return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx)
    {
      return ctx.begin();
    }
};

} // namespace std

// =============================================================================
