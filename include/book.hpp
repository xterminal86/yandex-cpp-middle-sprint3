#pragma once

#include <format>
#include <stdexcept>
#include <string_view>

namespace bookdb {

enum class Genre
{
    Fiction
  , NonFiction
  , SciFi
  , Biography
  , Mystery
  , Unknown
};

// FYI: https://ru.wikipedia.org/wiki/FNV

// Simple FNV-1a hash function (must be constexpr)
constexpr uint32_t hash_str(std::string_view sv)
{
  // >>> hex(2166136261)
  // '0x811c9dc5'
  uint32_t hash = 0x811c9dc5;
  for (char c : sv)
  {
    // >>> hex(16777619)
    // '0x1000193'
    hash = (hash ^ static_cast<uint32_t>(c)) * 0x01000193;
  }
  return hash;
}

// User-defined literal for cleaner syntax.
constexpr uint32_t operator""_hash(const char* s, size_t n)
{
  return hash_str(std::string_view(s, n));
}

constexpr Genre GenreFromString(std::string_view s)
{
  using namespace std::literals;

  switch (hash_str(s))
  {
    case "Fiction"_hash:    return Genre::Fiction;    break;
    case "NonFiction"_hash: return Genre::NonFiction; break;
    case "SciFi"_hash:      return Genre::SciFi;      break;
    case "Biography"_hash:  return Genre::Biography;  break;
    case "Mystery"_hash:    return Genre::Mystery;    break;
    default:                return Genre::Unknown;    break;
  }

  return Genre::Unknown;
}

struct Book {
  // string_view для экономии памяти, чтобы ссылаться на оригинальную строку,
  // хранящуюся в другом контейнере.
  std::string_view Author;
  std::string_view Title;

  size_t Year = 1970;
  Genre Genre_ = Genre::Unknown;
  double Rating = 0.0;
  size_t ReadCount = 0;

  // NOTE: not actually a constexpr - falls back to runtime
  // because of std::string.
  constexpr Book(const std::string_view title,
                 const std::string_view author,
                 const size_t year,
                 const Genre genre,
                 const double rating,
                 const size_t readCount)
    : Author(author),
      Title(title),
      Year(year),
      Genre_(genre),
      Rating(rating),
      ReadCount(readCount) {}

  constexpr Book(const std::string_view title,
                 const std::string_view author,
                 const size_t year,
                 const std::string_view genre,
                 const double rating,
                 const size_t readCount)
    : Author(author),
      Title(title),
      Year(year),
      Genre_( GenreFromString(genre) ),
      Rating(rating),
      ReadCount(readCount) {}
};
}  // namespace bookdb

namespace std {

template <>
struct formatter<bookdb::Genre, char>
{
  template <typename FormatContext>
  auto format(const bookdb::Genre g, FormatContext& fc) const
  {
    std::string genre_str;

    // clang-format off
    using bookdb::Genre;
    switch (g)
    {
      case Genre::Fiction:    genre_str = "Fiction";    break;
      case Genre::Mystery:    genre_str = "Mystery";    break;
      case Genre::NonFiction: genre_str = "NonFiction"; break;
      case Genre::SciFi:      genre_str = "SciFi";      break;
      case Genre::Biography:  genre_str = "Biography";  break;
      case Genre::Unknown:    genre_str = "Unknown";    break;
      default:
          throw logic_error{"Unsupported bookdb::Genre"};
      }
    // clang-format on
    return format_to(fc.out(), "{}", genre_str);
  }

  constexpr auto parse(format_parse_context& ctx)
  {
    return ctx.begin();  // Просто игнорируем пользовательский формат
  }
};

template <>
struct formatter<bookdb::Book>
{
  template <typename FormatContext>
  auto format(const bookdb::Book& b, FormatContext& fc) const
  {
    return format_to(
      fc.out(),
R"({{
  "Author" : "{}",
  "Title" : "{}",
  "Year" : {},
  "Genre" : "{}",
  "Rating" : {},
  "ReadCount" : {}
}})",
      b.Author,
      b.Title,
      b.Year,
      b.Genre_,
      b.Rating,
      b.ReadCount);
  }

  constexpr auto parse(format_parse_context& ctx)
  {
    return ctx.begin();
  }
};

}  // namespace std
