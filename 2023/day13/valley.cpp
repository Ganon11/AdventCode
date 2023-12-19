#include "valley.h"

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

Valley::Valley(const std::vector<std::string>& lines)
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
}

unsigned int Valley::rows() const
{
  return find_reflection(m_rows);
}

unsigned int Valley::columns() const
{
  return find_reflection(m_columns);
}

unsigned int Valley::find_reflection(const std::vector<unsigned int>& nums)
{
  for (int index = 0; index < nums.size() - 1; ++index)
  {
    if (reflection_at(nums, index))
    {
      return index + 1;
    }
  }

  return -1;
}

bool Valley::reflection_at(const std::vector<unsigned int>& nums, const int index)
{
  int index1 = index;
  int index2 = index + 1;
  while (index1 >= 0 && index2 < nums.size())
  {
    if (nums[index1] != nums[index2])
    {
      return false;
    }

    --index1;
    ++index2;
  }

  return true;
}
