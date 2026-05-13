#include <gtest/gtest.h>
#include <print>

#include "book_database.hpp"
#include "statsistics.hpp"

using namespace bookdb;

// =============================================================================

void FillDatabase(/*Fucking bullshit*/BookDatabase<>& db)
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
  {
    Book b("1984", "George Orwell", 1949, Genre::SciFi, 4., 190);
    EXPECT_EQ("1984",          b.Title);
    EXPECT_EQ("George Orwell", b.Author);
    EXPECT_EQ(1949, b.Year);
    EXPECT_EQ(Genre::SciFi, b.Genre_);
    EXPECT_DOUBLE_EQ(4.0, b.Rating);
    EXPECT_EQ(190, b.ReadCount);
  }
  // ---------------------------------------------------------------------------
  {
    Book b("1984", "George Orwell", 1949, "SciFi", 4., 190);
    EXPECT_EQ("1984",          b.Title);
    EXPECT_EQ("George Orwell", b.Author);
    EXPECT_EQ(1949, b.Year);
    EXPECT_EQ(Genre::SciFi, b.Genre_);
    EXPECT_DOUBLE_EQ(4.0, b.Rating);
    EXPECT_EQ(190, b.ReadCount);
  }
  // ---------------------------------------------------------------------------
  {
    Book b("Lolsus", "Don T. Beakunt", 100500, "Huj", 0.0001, 777);
    EXPECT_EQ("Lolsus",         b.Title);
    EXPECT_EQ("Don T. Beakunt", b.Author);
    EXPECT_EQ(100500, b.Year);
    EXPECT_EQ(Genre::Unknown, b.Genre_);
    EXPECT_DOUBLE_EQ(0.0001, b.Rating);
    EXPECT_EQ(777, b.ReadCount);
  }
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
