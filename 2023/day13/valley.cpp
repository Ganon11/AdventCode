#include "valley.h"

#include <iostream>
#include <string>
#include <vector>

unsigned int to_number(const std::string& str)
{
  unsigned int result{0};
  for (const char ch : str)
  {
    result <<= 1;
    if (ch == '#')
    {
      result ^= 1;
    }
  }

  return result;
}

bool is_power_of_two(const unsigned int n)
{
  return n != 0 && ((n & (n - 1)) == 0);
}

bool different_by_one_bit(const unsigned int a, const unsigned int b)
{
  return is_power_of_two(a ^ b);
}

Valley::Valley(const std::vector<std::string>& lines)
  : m_row_score{ -1 },
    m_col_score{ -1 }
{
  m_rows.reserve(lines.size());
  m_columns.reserve(lines.size());

  for (const std::string& row : lines)
  {
    unsigned int number{ to_number(row) };
    m_rows.push_back(to_number(row));
  }

  for (size_t col = 0; col < lines[0].size(); ++col)
  {
    std::string temp;
    for (const std::string& row : lines)
    {
      temp += row[col];
    }

    unsigned int number{ to_number(temp) };
    m_columns.push_back(to_number(temp));
  }

  m_row_score = rows(false);
  m_col_score = columns(false);

  //std::cout << "Precomputed row score " << m_row_score << ", col score " << m_col_score << std::endl;
}

int Valley::rows(const bool smudge) const
{
  if (!smudge && m_row_score != -1)
  {
    return m_row_score;
  }

  return find_reflection(m_rows, smudge, m_row_score);
}

int Valley::columns(const bool smudge) const
{
  if (!smudge && m_col_score != -1)
  {
    return m_col_score;
  }

  return find_reflection(m_columns, smudge, m_col_score);
}

int Valley::find_reflection(const std::vector<unsigned int>& nums, const bool smudge, const int previous_score)
{
  for (int index = 0; index < nums.size() - 1; ++index)
  {
    if (reflection_at(nums, index, smudge))
    {
      int reflection{ index + 1 };
      if (smudge && reflection == previous_score)
      {
        continue;
      }

      return index + 1;
    }
  }

  return -1;
}

bool Valley::reflection_at(const std::vector<unsigned int>& nums, const int index, const bool smudge)
{
  int index1 = index;
  int index2 = index + 1;
  bool smudge_found = false;
  while (index1 >= 0 && index2 < nums.size())
  {
    if (nums[index1] != nums[index2])
    {
      if (!smudge)
      {
        return false;
      }

      if (smudge_found)
      {
        return false;
      }

      if (!different_by_one_bit(nums[index1], nums[index2]))
      {
        return false;
      }

      smudge_found = true;
    }

    --index1;
    ++index2;
  }

  if (smudge)
  {
    return smudge_found;
  }

  return true;
}
