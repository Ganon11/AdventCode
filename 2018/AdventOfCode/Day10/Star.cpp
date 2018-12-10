#include "pch.h"
#include "Star.h"

#include <regex>

Star::Star(const std::wstring& line)
{
   std::wregex star_regex{ L"position=<\\s*(-?\\d+),\\s*(-?\\d+)> velocity=<\\s*(-?\\d+),\\s*(-?\\d+)>"};
   std::wsmatch matches;
   std::regex_search(line, matches, star_regex);
   if (matches.size() == 5) // Whole match, position X/Y, velocity X/Y
   {
      m_position.first = _wtoi(matches[1].str().c_str());
      m_position.second = _wtoi(matches[2].str().c_str());
      m_velocity.first = _wtoi(matches[3].str().c_str());
      m_velocity.second = _wtoi(matches[4].str().c_str());
   }
}

void Star::move()
{
   m_position.first += m_velocity.first;
   m_position.second += m_velocity.second;
}

Position Star::get_position() const
{
   return m_position;
}
