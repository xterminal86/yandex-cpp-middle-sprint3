#pragma once

#include <algorithm>
#include <functional>

#include "book.hpp"
#include "concepts.hpp"

namespace bookdb {

constexpr auto YearBetween(int start, int end)
{
  return [start, end](const Book& b)
  {
    return (b.Year >= start and b.Year <= end);
  };
}

constexpr auto RatingAbove(double min_rating)
{
  return [min_rating](const Book& b)
  {
    return b.Rating >= min_rating;
  };
}

constexpr auto GenreIs(Genre genre)
{
  return [genre](const Book& b)
  {
    return b.Genre_ == genre;
  };
}

constexpr auto AuthorIs(const std::string& name)
{
  return [&name](const Book& b)
  {
    return b.Author == name;
  };
}

using FilteredBooks = std::vector<std::reference_wrapper<Book>>;

template <BookIterator It, BookSentinel<It> Sent, BookPredicate Pred>
FilteredBooks filterBooks(It first, Sent last, Pred pred)
{
  FilteredBooks res;

  std::copy_if(first, last, std::back_inserter(res), pred);

  return res;
}

template <BookPredicate... Preds>
constexpr auto all_of(Preds... preds)
{
  return [preds...](const Book& b)
  {
    return (preds(b) and ...);
  };
}

template <BookPredicate... Preds>
constexpr auto any_of(Preds... preds)
{
  return [preds...](const Book& b)
  {
    return (preds(b) or ...);
  };
}

}  // namespace bookdb