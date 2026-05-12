#include <gtest/gtest.h>
#include <print>

#include "book_database.hpp"

using namespace bookdb;

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
