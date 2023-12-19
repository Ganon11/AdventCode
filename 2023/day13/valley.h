#pragma once

#include <string>
#include <vector>

class Valley
{
public:
  Valley(const std::vector<std::string>& lines);

  unsigned int rows() const;
  unsigned int columns() const;

private:
  static unsigned int find_reflection(const std::vector<unsigned int>& nums);
  static bool reflection_at(const std::vector<unsigned int>& nums, const int index);

private:
  std::vector<unsigned int> m_rows;
  std::vector<unsigned int> m_columns;
};
