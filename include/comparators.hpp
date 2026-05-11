#pragma once

#include "book.hpp"

namespace bookdb::comp {

struct LessByAuthor
{
  bool operator()(const bookdb::Book& a, const bookdb::Book& b) const
  {
    return a.Author < b.Author;
  }
};

struct LessByTitle
{
  bool operator()(const bookdb::Book& a, const bookdb::Book& b) const
  {
    return a.Title < b.Title;
  }
};

struct LessByYear
{
  bool operator()(const bookdb::Book& a, const bookdb::Book& b) const
  {
    return a.Year < b.Year;
  }
};

struct LessByPopularity
{
  bool operator()(const bookdb::Book& a, const bookdb::Book& b) const
  {
    return a.Rating < b.Rating;
  }
};

struct LessByReadCount
{
  bool operator()(const bookdb::Book& a, const bookdb::Book& b) const
  {
    return a.ReadCount < b.ReadCount;
  }
};

}  // namespace bookdb::comp
