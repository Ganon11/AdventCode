#pragma once

#include <string>
#include <vector>

unsigned int to_number(const std::string& str);
bool is_power_of_two(const unsigned int n);
bool different_by_one_bit(const unsigned int a, const unsigned int b);

class Valley
{
public:
  Valley(const std::vector<std::string>& lines);

  int rows(const bool smudge = false) const;
  int columns(const bool smudge = false) const;

private:
  static int find_reflection(const std::vector<unsigned int>& nums, const bool smudge);
  static bool reflection_at(const std::vector<unsigned int>& nums, const int index, const bool smudge);

private:
  std::vector<unsigned int> m_rows;
  std::vector<unsigned int> m_columns;
};
