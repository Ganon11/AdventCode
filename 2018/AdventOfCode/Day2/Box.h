#pragma once

#include <map>
#include <string>

typedef std::map<wchar_t, unsigned int> CharacterMap;

class Box
{
public:
  Box(const std::wstring& name);

  bool has_exactly_two_of_a_character() const;
  bool has_exactly_three_of_a_character() const;
  unsigned int edit_distance(const Box& other) const;

  std::wstring get_name() const;

private:
  std::wstring m_name;
  bool m_has_exactly_two;
  bool m_has_exactly_three;
};