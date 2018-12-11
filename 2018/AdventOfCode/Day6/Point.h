#pragma once

#include <string>

struct Point
{
  unsigned int m_x;
  unsigned int m_y;

  Point(const std::wstring& line);
  Point(unsigned int x, unsigned int y);

#ifdef _WIN64
  Point(size_t x, size_t y);
#endif

  int DistanceTo(const Point& other) const;
};
