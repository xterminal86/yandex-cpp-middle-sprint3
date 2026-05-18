#include <gtest/gtest.h>
#include <print>

#include "book_database.hpp"
#include "statsistics.hpp"
#include "filters.hpp"

using namespace bookdb;
using namespace std::string_view_literals;

// =============================================================================

void FillDatabase(/*Fucking <> bullshit needed*/BookDatabase<>& db)
{
  db.PushBack(Book{"1984", "George Orwell", 1949, Genre::SciFi, 4., 190});
  db.PushBack(Book{"Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143});
  db.PushBack(Book{"The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120});
  db.PushBack(Book{"To Kill a Mockingbird", "Harper Lee", 1960, Genre::Fiction, 4.8, 156});
  db.PushBack(Book{"Pride and Prejudice", "Jane Austen", 1813, Genre::Fiction, 4.7, 178});
  db.PushBack(Book{"The Catcher in the Rye", "J.D. Salinger", 1951, Genre::Fiction, 4.3, 112});
  db.PushBack(Book{"Brave New World", "Aldous Huxley", 1932, Genre::SciFi, 4.5, 98});
  db.PushBack(Book{"Jane Eyre", "Charlotte Brontë", 1847, Genre::Fiction, 4.6, 110});
  db.PushBack(Book{"The Hobbit", "J.R.R. Tolkien", 1937, Genre::Fiction, 4.9, 203});
  db.PushBack(Book{"Lord of the Flies", "William Golding", 1954, Genre::Fiction, 4.2, 89});
}

// =============================================================================

TEST(BookConstructor, ConstexprCheck)
{
  BookDatabase db(
    {
        { "1984", "George Orwell", 1949, Genre::SciFi, 4., 190 }
      , { "Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143 }
      , { "The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120 }
      , {"To Kill a Mockingbird", "Harper Lee", 1960, Genre::Fiction, 4.8, 156 }
      , {"Pride and Prejudice", "Jane Austen", 1813, Genre::Fiction, 4.7, 178 }
      , {"The Catcher in the Rye", "J.D. Salinger", 1951, Genre::Fiction, 4.3, 112 }
      , {"Brave New World", "Aldous Huxley", 1932, Genre::SciFi, 4.5, 98 }
      , {"Jane Eyre", "Charlotte Brontë", 1847, Genre::Fiction, 4.6, 110 }
      , {"The Hobbit", "J.R.R. Tolkien", 1937, Genre::Fiction, 4.9, 203 }
      , {"Lord of the Flies", "William Golding", 1954, Genre::Fiction, 4.2, 89 }
    }
  );
  db.Dump();
  EXPECT_EQ(10, db.GetBooks().size());
  auto filtered = filterBooks(
    db.begin(),
    db.end(),
    all_of(
      GenreIs(Genre::Fiction),
      YearBetween(1925, 1950)
    )
  );
  EXPECT_EQ(3, filtered.size());
}

// =============================================================================

TEST(BookDatabase, EmplaceBack)
{
  BookDatabase db;
  db.EmplaceBack("1984", "George Orwell", 1949, Genre::SciFi, 4., 190);
  db.EmplaceBack("Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143);
  db.EmplaceBack("The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120);
  db.EmplaceBack("To Kill a Mockingbird", "Harper Lee", 1960, Genre::Fiction, 4.8, 156);
  db.EmplaceBack("Pride and Prejudice", "Jane Austen", 1813, Genre::Fiction, 4.7, 178);
  db.EmplaceBack("The Catcher in the Rye", "J.D. Salinger", 1951, Genre::Fiction, 4.3, 112);
  db.EmplaceBack("Brave New World", "Aldous Huxley", 1932, Genre::SciFi, 4.5, 98);
  db.EmplaceBack("Jane Eyre", "Charlotte Brontë", 1847, Genre::Fiction, 4.6, 110);
  db.EmplaceBack("The Hobbit", "J.R.R. Tolkien", 1937, Genre::Fiction, 4.9, 203);
  db.EmplaceBack("Lord of the Flies", "William Golding", 1954, Genre::Fiction, 4.2, 89);
  db.Dump();
  EXPECT_EQ(10, db.GetBooks().size());
  auto filtered = filterBooks(
    db.begin(),
    db.end(),
    all_of(
      GenreIs(Genre::Fiction),
      YearBetween(1925, 1950)
    )
  );
  EXPECT_EQ(3, filtered.size());
}

// =============================================================================

TEST(BookDatabase, MemoryEfficiency)
{
  {
    BookDatabase db;
    db.EmplaceBack("Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143);
    db.EmplaceBack("1984", "George Orwell", 1949, Genre::SciFi, 4., 190);

    auto& authors = db.GetAuthors();
    auto it1 = authors.find("George Orwell");
    ASSERT_TRUE(it1 != authors.end());

    // iterator -> holder object -> contents
    const void* authorPtr = (const void*)(*it1).data();

    std::span<const Book> s = db.GetBooks();
    ASSERT_EQ(2, s.size());

    const void* p1 = (const void*)s[0].Author.data();
    const void* p2 = (const void*)s[1].Author.data();

    // This would signify that Author in books_ container actually points to the
    // same string in authors_ unordered set and is not a separate string or
    // anything.
    EXPECT_EQ(authorPtr, p1);
    EXPECT_EQ(authorPtr, p2);

    // We could repeat the same thing for one of the titles, but since it's
    // implemented exactly the same as authors_ in code, and we don't have
    // GetTitles() method, and we're kinda don't wanna implement it since it
    // wasn't "part of the original deal", so just trust me bro. ;-)
  }
  //----------------------------------------------------------------------------
  {
    BookDatabase db;
    //
    // To prevent compiler optimizing strings and stuff.
    // If we fill database using this syntax:
    // db.PushBack({ "Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143 })"
    // then tests will pass on BookDatabase::books_.push_back(book).
    // But we need to make sure that Author and Title are created once and
    // elements in books_ reference them.
    //
    Book b = { "Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143 };
    db.PushBack(b);
    b = { "1984", "George Orwell", 1949, Genre::SciFi, 4., 190 };
    db.PushBack(b);

    auto& authors = db.GetAuthors();
    auto it1 = authors.find("George Orwell");
    ASSERT_TRUE(it1 != authors.end());

    // iterator -> holder object -> contents
    const void* authorPtr = (const void*)(*it1).data();

    std::span<const Book> s = db.GetBooks();
    ASSERT_EQ(2, s.size());

    const void* p1 = (const void*)s[0].Author.data();
    const void* p2 = (const void*)s[1].Author.data();

    // This would signify that Author in books_ container actually points to the
    // same string in authors_ unordered set and is not a separate string or
    // anything.
    EXPECT_EQ(authorPtr, p1);
    EXPECT_EQ(authorPtr, p2);

    // We could repeat the same thing for one of the titles, but since it's
    // implemented exactly the same as authors_ in code, and we don't have
    // GetTitles() method, and we're kinda don't wanna implement it since it
    // wasn't "part of the original deal", so just trust me bro. ;-)
  }
  // ---------------------------------------------------------------------------
#ifdef DEBUG_BUILD
  // Check that filterBooks() returns references to objects inside BookDatabase
  // container and not new ones.
  {
    BookDatabase db;
    FillDatabase(db);
    auto filtered = filterBooks(
      db.begin(),
      db.end(),
      any_of(
        YearBetween(0, 9999)
      )
    );
    ASSERT_EQ(10, filtered.size());

    std::unordered_map<void*, Book> expectedObjectsByAddr;
    auto& intlCont = db.GetInternalContainer();
    for (const Book& b : intlCont)
    {
      // Can't do this because Book doesn't have default ctor in our
      // implementation.
      //expectedObjectsByAddr[(void*)&b] = b;

      expectedObjectsByAddr.emplace((void*)&b, b);
    }

    for (auto& kvp : expectedObjectsByAddr)
    {
      bool found = false;
      void* addr = kvp.first;
      const Book& actualObj = kvp.second;
      Book* refObj = nullptr;
      for (auto& i : filtered)
      {
        if ((void*)&(i.get()) == addr)
        {
          found = true;
          refObj = &(i.get());
          break;
        }
      }
      if (not found)
      {
        std::println("Address {} not found in BookDatabase container", addr);
        db.Dump();
      }
      ASSERT_TRUE(found);
      ASSERT_TRUE(refObj != nullptr);
      EXPECT_EQ(actualObj.Author,    refObj->Author);
      EXPECT_EQ(actualObj.Title,     refObj->Title);
      EXPECT_EQ(actualObj.Year,      refObj->Year);
      EXPECT_EQ(actualObj.Genre_,    refObj->Genre_);
      EXPECT_EQ(actualObj.Rating,    refObj->Rating);
      EXPECT_EQ(actualObj.ReadCount, refObj->ReadCount);
    }
  }
#endif
}

// =============================================================================

TEST(BookFormatter, Test)
{
  {
    Book b("1984", "George Orwell", 1949, Genre::SciFi, 4., 190);
    std::println("{}", b);
  }
  {
    Book b("1984", "George Orwell", 1949, Genre::SciFi, 4., 190);
    std::string actual = std::format("{}", b);
    const std::string expected =
R"({
  "Author" : "George Orwell",
  "Title" : "1984",
  "Year" : 1949,
  "Genre" : "SciFi",
  "Rating" : 4,
  "ReadCount" : 190
})";
    EXPECT_EQ(expected, actual);
  }
}

// =============================================================================

TEST(BookDatabase, Empty)
{
  BookDatabase db;

  {
    auto filtered = filterBooks(
      db.begin(),
      db.end(),
      any_of(
        GenreIs(Genre::SciFi)
      )
    );
    EXPECT_TRUE(filtered.empty());
  }
  // ---------------------------------------------------------------------------
  {
    auto filtered = filterBooks(
      db.begin(),
      db.end(),
      any_of(
        AuthorIs("George Orwell")
      )
    );
    EXPECT_TRUE(filtered.empty());
  }
  // ---------------------------------------------------------------------------
  {
    auto filtered = filterBooks(
      db.begin(),
      db.end(),
      any_of(
        AuthorIs("Сергей Пахомов")
      )
    );
    EXPECT_TRUE(filtered.empty());
  }
}

// =============================================================================

TEST(BookDatabase, Filters)
{
  BookDatabase db;
  FillDatabase(db);

  {
    auto filtered = filterBooks(
      db.begin(),
      db.end(),
      any_of(
        GenreIs(Genre::SciFi)
      )
    );
    EXPECT_EQ(2, filtered.size());
  }
  // ---------------------------------------------------------------------------
  {
    auto filtered = filterBooks(
      db.begin(),
      db.end(),
      any_of(
        AuthorIs("George Orwell")
      )
    );
    EXPECT_EQ(2, filtered.size());
  }
  // ---------------------------------------------------------------------------
  {
    auto filtered = filterBooks(
      db.begin(),
      db.end(),
      any_of(
        AuthorIs("Сергей Пахомов")
      )
    );
    EXPECT_TRUE(filtered.empty());
  }
  // ---------------------------------------------------------------------------
  {
    auto filtered = filterBooks(
      db.begin(),
      db.end(),
      any_of(
        RatingAbove(5.0)
      )
    );
    EXPECT_TRUE(filtered.empty());
  }
}

// =============================================================================

TEST(Statistics, SampleRandom)
{
  {
    BookDatabase db;
    FillDatabase(db);
    auto out = sampleRandomBooks(db, 0);
    EXPECT_EQ(0, out.size());
  }
  // ---------------------------------------------------------------------------
  {
    BookDatabase db;
    FillDatabase(db);
    auto out = sampleRandomBooks(db, 1);
    EXPECT_EQ(1, out.size());
  }
  // ---------------------------------------------------------------------------
  {
    BookDatabase db;
    FillDatabase(db);
    auto out = sampleRandomBooks(db, 2);
    EXPECT_EQ(2, out.size());
  }
  // ---------------------------------------------------------------------------
  {
    BookDatabase db;
    FillDatabase(db);
    auto out = sampleRandomBooks(db, 10);
    EXPECT_EQ(10, out.size());
  }
  // ---------------------------------------------------------------------------
  {
    BookDatabase db;
    FillDatabase(db);
    auto out = sampleRandomBooks(db, 100);
    EXPECT_EQ(0, out.size());
  }
}

// =============================================================================

TEST(Statistics, GetTopN)
{
  {
    BookDatabase db;
    FillDatabase(db);
    auto out = getTopNBy(db, 0, comp::GreaterByRating{});
    ASSERT_EQ(0, out.size());
  }
  // ---------------------------------------------------------------------------
  {
    BookDatabase db;
    FillDatabase(db);
    auto out = getTopNBy(db, 1, comp::LessByRating{});
    ASSERT_EQ(1, out.size());
    EXPECT_EQ("1984"sv, out[0].Title);
  }
  // ---------------------------------------------------------------------------
  {
    BookDatabase db;
    FillDatabase(db);
    auto out = getTopNBy(db, 10, comp::GreaterByRating{});
    ASSERT_EQ(10, out.size());
  }
  // ---------------------------------------------------------------------------
  {
    BookDatabase db;
    FillDatabase(db);
    auto out = getTopNBy(db, 20, comp::GreaterByRating{});
    ASSERT_EQ(10, out.size());
  }
}