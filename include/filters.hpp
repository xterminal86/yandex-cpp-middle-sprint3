#pragma once

#include <algorithm>
#include <functional>

#include "book.hpp"
#include "concepts.hpp"

namespace bookdb {

// Опционально.
/*
constexpr auto YearBetween(int start, int end);
constexpr auto RatingAbove(double min_rating);
constexpr auto GenreIs(Genre genre);
*/

/*
template <BookIterator It, BookSentinel<It> Sent, BookPredicate Pred>
auto filterBooks(It first, Sent last, Pred pred);

template <BookPredicate... Preds>
constexpr auto all_of(Preds... preds);

template <BookPredicate... Preds>
constexpr auto any_of(Preds... preds);
*/

}  // namespace bookdb