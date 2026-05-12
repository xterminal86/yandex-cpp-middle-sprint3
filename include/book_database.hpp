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

    // Опционально.
    // BookDatabase(std::initializer_list<Book> books);

    void clear()
    {
      books_.clear();
      authors_.clear();
    }

    // Standard container interface methods
    bool empty() const { return books_.empty(); }
    size_t size() const { return books_.size(); }

    Book& operator[](const size_t index) { return books_[index]; }

    auto begin() { return books_.begin(); }
    auto end() { return books_.end(); }

    // Опционально.
    //template <typename... Args>
    //  requires std::constructible_from<Book, Args...>
    //  void EmplaceBack(Args &&...args);

    void PushBack(const Book& book)
    {
      // What's the point if author should already be set in Book object?
      authors_.insert(std::string(book.Author.data(), book.Author.size()));
      books_.push_back(book);
    }

    void PushBack(Book&& book)
    {
      authors_.insert(std::string(book.Author.data(), book.Author.size()));
      books_.push_back(std::move(book));
    }

    std::span<const Book> GetBooks() const
    {
      return books_;
    }

    std::span<Book> GetBooks()
    {
      return books_;
    }

    const AuthorContainer& GetAuthors() const
    {
      return authors_;
    }

    const AuthorContainer& GetAuthors()
    {
      return authors_;
    }

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

      bool first = true;
      for (const bookdb::Book& book : db.GetBooks())
      {
        if (not first)
        {
          format_to(fc.out(), ",\n");
        }

        format_to(fc.out(), "{}", book);
        first = false;
      }

      format_to(fc.out(), "\nAuthors:\n");
      for (const auto& author : db.GetAuthors())
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
