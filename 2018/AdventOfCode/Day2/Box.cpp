#include "pch.h"
#include "Box.h"
#include <algorithm>

Box::Box(const wstring& name) : m_name{ name }
{
   CharacterMap charMap;
   for (wchar_t ch : name)
   {
      if (charMap.end() == charMap.find(ch))
      {
         charMap[ch] = 1u;
      }
      else
      {
         ++charMap[ch];
      }
   }

   m_has_exactly_two = (charMap.end() != find_if(charMap.begin(), charMap.end(), [](const CharacterMap::value_type& v) { return v.second == 2u; }));
   m_has_exactly_three = (charMap.end() != find_if(charMap.begin(), charMap.end(), [](const CharacterMap::value_type& v) { return v.second == 3u; }));
}

bool Box::has_exactly_two_of_a_character() const
{
   return m_has_exactly_two;
}

bool Box::has_exactly_three_of_a_character() const
{
   return m_has_exactly_three;
}

unsigned int Box::edit_distance(const Box& other) const
{
   wstring otherName{ other.get_name() };
   unsigned int diffCount{ 0u };
   for (unsigned int i = 0; i < m_name.length(); ++i)
   {
      if (m_name[i] != otherName[i])
      {
         ++diffCount;
      }
   }

   return diffCount;
}

wstring Box::get_name() const
{
   return m_name;
}
