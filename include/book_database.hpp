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

    //
    // For Container requirement compliance.
    // (https://en.cppreference.com/cpp/named_req/Container)
    // These are mostly needed for template black magic (e.g. see concepts.hpp).
    //
    using value_type      = Book;
    using reference       = Book&;
    using const_reference = const Book&;
    using iterator        = BookContainer::iterator;
    using const_iterator  = BookContainer::const_iterator;
    using difference_type = BookContainer::difference_type;
    using size_type       = BookContainer::size_type;

    // Custom.
    using UOSS = std::unordered_set<std::string>;

    BookDatabase() = default;

    BookDatabase(std::initializer_list<Book> books)
      : books_(books)
    {
      for (Book& b : books_)
      {
        // Leverage std::string(const char* data, size_t size) ctor.
        auto it1 = authors_.emplace(b.Author.data(), b.Author.size());
        b.Author = *it1.first;
        auto it2 = titles_.emplace(b.Title.data(), b.Title.size());
        b.Title = *it2.first;
      }
    }

    void clear()
    {
      books_.clear();
      authors_.clear();
    }

    // Standard container interface methods
    bool empty()     const { return books_.empty(); }
    size_type size() const { return books_.size();  }

    Book& operator[](const size_t index) { return books_[index]; }

    iterator begin()              { return books_.begin();  }
    iterator end()                { return books_.end();    }
    const_iterator cbegin() const { return books_.cbegin(); }
    const_iterator cend()   const { return books_.cend();   }

    template <typename... Args>
    requires std::constructible_from<Book, Args...>
    void EmplaceBack(Args&&... args)
    {
      Book& inserted = books_.emplace_back(std::forward<Args>(args)...);
      auto it1 = authors_.insert(
        std::string(inserted.Author.data(), inserted.Author.size())
      );
      auto it2 = titles_.insert(
        std::string(inserted.Title.data(), inserted.Title.size())
      );
      inserted.Author = *it1.first;
      inserted.Title  = *it2.first;
    }

    void PushBack(const Book& book)
    {
      auto it1 = titles_.insert(
        std::string(book.Title.data(), book.Title.size())
      );
      auto it2 = authors_.insert(
        std::string(book.Author.data(), book.Author.size())
      );
      books_.push_back(
        Book(
          *it1.first,
          *it2.first,
          book.Year,
          book.Genre_,
          book.Rating,
          book.ReadCount
        )
      );
    }

    void PushBack(Book&& book)
    {
      auto p1 = authors_.insert(
        std::string(book.Author.data(), book.Author.size())
      );
      auto p2 = titles_.insert(
        std::string(book.Title.data(), book.Title.size())
      );
      book.Author = *p1.first;
      book.Title  = *p2.first;
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

    const UOSS& GetAuthors() const
    {
      return authors_;
    }

    const UOSS& GetAuthors()
    {
      return authors_;
    }

    void Dump()
    {
      static const std::string ruler(80, '=');

      // -----------------------------------------------------------------------
      auto _Printer =
      [](const std::string& header, const UOSS& container)
      {
        std::println("{}:", header);

        for (const std::string& data : container)
        {
          std::println("@{} -> '{}'", (void*)data.data(), data);
        }
      };
      // -----------------------------------------------------------------------

      std::println("");
      std::println("---------- BookDatabase dump start ----------");
      std::println("");

      _Printer("Authors", authors_);
      std::println("");
      _Printer("Titles", titles_);

      std::println("");

      std::println("Contents:");
      for (const Book& b : books_)
      {
        std::println("@{} -> {{ @{} -> '{}', @{} -> '{}', ... }}",
                     (void*)&b,
                     (void*)b.Author.data(),
                     b.Author,
                     (void*)b.Title.data(),
                     b.Title);
      }

      std::println("");
      std::println("---------- BookDatabase dump end ------------");
      std::println("");
    }

#ifdef DEBUG_BUILD
    BookContainer& GetInternalContainer()
    {
      return books_;
    }
#endif

private:
    BookContainer books_;
    UOSS authors_;
    UOSS titles_;
};

}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::BookDatabase<std::vector<bookdb::Book>>> {
    template <typename FormatContext>
    auto format(const bookdb::BookDatabase<std::vector<bookdb::Book>>& db,
                FormatContext& fc) const
    {
      format_to(fc.out(), "BookDatabase (size = {}), ", db.size());

      format_to(fc.out(), "books:\n");

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
