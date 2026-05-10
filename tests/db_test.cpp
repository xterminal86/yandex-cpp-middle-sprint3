#include <gtest/gtest.h>

#include "book.hpp"

using namespace bookdb;

TEST(TestComponentName, SimpleCheck)
{
  {
    Book b("1984", "George Orwell", 1949, Genre::SciFi, 4., 190);
    EXPECT_STRCASEEQ("1984",          b.Title.data());
    EXPECT_STRCASEEQ("George Orwell", b.Author.data());
    EXPECT_EQ(1949, b.Year);
    EXPECT_EQ(Genre::SciFi, b.Genre_);
    EXPECT_DOUBLE_EQ(4.0, b.Rating);
    EXPECT_EQ(190, b.ReadCount);
  }
  // ---------------------------------------------------------------------------
  {
    Book b("1984", "George Orwell", 1949, "SciFi", 4., 190);
    EXPECT_STRCASEEQ("1984",          b.Title.data());
    EXPECT_STRCASEEQ("George Orwell", b.Author.data());
    EXPECT_EQ(1949, b.Year);
    EXPECT_EQ(Genre::SciFi, b.Genre_);
    EXPECT_DOUBLE_EQ(4.0, b.Rating);
    EXPECT_EQ(190, b.ReadCount);
  }
  // ---------------------------------------------------------------------------
  {
    Book b("Lolsus", "Don T. Beakunt", 100500, "Huj", 0.0001, 777);
    EXPECT_STRCASEEQ("Lolsus",         b.Title.data());
    EXPECT_STRCASEEQ("Don T. Beakunt", b.Author.data());
    EXPECT_EQ(100500, b.Year);
    EXPECT_EQ(Genre::Unknown, b.Genre_);
    EXPECT_DOUBLE_EQ(0.0001, b.Rating);
    EXPECT_EQ(777, b.ReadCount);
  }
}
