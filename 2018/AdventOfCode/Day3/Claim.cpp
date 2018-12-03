#include "pch.h"
#include "Claim.h"
#include <regex>

Claim::Claim(const wstring& line)
{
   wregex line_parsing_regex{ L"#(\\d+) @ (\\d+),(\\d+): (\\d+)x(\\d+)" };
   wsmatch matches;
   regex_search(line, matches, line_parsing_regex);
   if (matches.size() == 6) // Whole match, then 5 capture groups
   {
      m_id = _wtoi(matches[1].str().c_str());
      m_left = _wtoi(matches[2].str().c_str());
      m_top = _wtoi(matches[3].str().c_str());
      m_width = _wtoi(matches[4].str().c_str());
      m_height = _wtoi(matches[5].str().c_str());
   }
}

unsigned int Claim::get_id() const
{
   return m_id;
}

unsigned int Claim::get_left() const
{
   return m_left;
}

unsigned int Claim::get_top() const
{
   return m_top;
}

unsigned int Claim::get_width() const
{
   return m_width;
}

unsigned int Claim::get_height() const
{
   return m_height;
}

vector<Claim::Coordinate> Claim::generate_coordinates() const
{
   vector<Claim::Coordinate> areas_covered;
   for (unsigned int x = 0; x < m_width; ++x)
   {
      for (unsigned int y = 0; y < m_height; ++y)
      {
         areas_covered.push_back(make_pair(m_left + x, m_top + y));
      }
   }

   return areas_covered;
}
