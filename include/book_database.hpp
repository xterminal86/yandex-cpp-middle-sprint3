#pragma once

#include <print>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_set>

#include "book.hpp"
#include "concepts.hpp"
#include "heterogeneous_lookup.hpp"

namespace bookdb {

template <BookContainerLike BookContainer = std::vector<Book>>
class BookDatabase {
public:
    // Type aliases
    using AuthorContainer = std::unordered_set<std::string>;

    BookDatabase() = default;
    BookDatabase(std::initializer_list<Book> books)
      : books_(books) {}

    void clear()
    {
      books_.clear();
      authors_.clear();
    }

    // Standard container interface methods
    bool empty() const { return books_.empty(); }
    size_t size() const { return books_.size(); }

    Book& operator[](const size_t index)
    {
      return books_[index];
    }

    void EmplaceBack(const char* title,
                     const char* author,
                     const uint64_t year,
                     const Genre genre,
                     const double rating,
                     const uint64_t readCount)
    {
      books_.emplace_back(Book{ title, author, year, genre, rating, readCount });
    }

    void EmplaceBack(const char* title,
                     const char* author,
                     const uint64_t year,
                     const char* genre,
                     const double rating,
                     const uint64_t readCount)
    {
      books_.emplace_back(Book{ title, author, year, genre, rating, readCount });
    }

    const BookContainer& GetBooks() const { return books_; }
    const AuthorContainer& GetAuthors() const { return authors_; }

private:
    BookContainer books_;
    AuthorContainer authors_;
};

}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::BookDatabase<std::vector<bookdb::Book>>> {
    template <typename FormatContext>
    auto format(const bookdb::BookDatabase<std::vector<bookdb::Book>>& db,
                FormatContext& fc) const
    {
      format_to(fc.out(), "BookDatabase (size = {}): ", db.size());

      format_to(fc.out(), "Books:\n");
      for (const bookdb::Book& book : db.GetBooks())
      {
        format_to(fc.out(), "- {}\n", book);
      }

      format_to(fc.out(), "Authors:\n");
      for (const auto &author : db.GetAuthors())
      {
        format_to(fc.out(), "- {}\n", author);
      }

      return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx)
    {
      return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};
}  // namespace std
