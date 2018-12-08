#include "pch.h"
#include "Point.h"
#include <regex>

Point::Point(const std::wstring& line)
{
   std::wregex point_regex{ L"(\\d+), (\\d+)" };
   std::wsmatch matches;
   std::regex_search(line, matches, point_regex);
   if (matches.size() == 3) // Whole match, x and y
   {
      m_x = _wtoi(matches[1].str().c_str());
      m_y = _wtoi(matches[2].str().c_str());
   }
}

Point::Point(unsigned int x, unsigned int y) : m_x{ x }, m_y{ y }
{ }

#ifdef _WIN64
Point::Point(size_t x, size_t y) : m_x{ static_cast<unsigned int>(x) }, m_y{ static_cast<unsigned int>(y) }
{ }
#endif

int Point::DistanceTo(const Point& other) const
{
   return (abs(static_cast<int>(m_x) - static_cast<int>(other.m_x)) + abs(static_cast<int>(m_y) - static_cast<int>(other.m_y)));
}
