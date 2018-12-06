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

Point::Point(int x, int y) : m_x{ x }, m_y{ y }
{

}

int Point::DistanceTo(const Point& other) const
{
   return (abs(m_x - other.m_x) + abs(m_y - other.m_y));
}
