#pragma once

#include <map>
#include <string>

using namespace std;

typedef map<wchar_t, unsigned int> CharacterMap;

class Box
{
public:
   Box(const wstring& name);

   bool has_exactly_two_of_a_character() const;
   bool has_exactly_three_of_a_character() const;
   unsigned int edit_distance(const Box& other) const;

   wstring get_name() const;

private:
   wstring m_name;
   bool m_has_exactly_two;
   bool m_has_exactly_three;
};