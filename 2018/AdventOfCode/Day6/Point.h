#pragma once

#include <string>

struct Point
{
   int m_x;
   int m_y;

   Point(const std::wstring& line);
   Point(int x, int y);

   int DistanceTo(const Point& other) const;
};
