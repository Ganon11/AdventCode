#include <iostream>
#include <vector>

#include "cxxopts.hpp"
#include "input_handler.h"

namespace
{
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

class Valley
{
public:
  Valley(const std::vector<std::string>& lines)
  {
    //std::cout << "New Valley" << std::endl;
    m_rows.reserve(lines.size());
    m_columns.reserve(lines.size());

    for (const std::string& row : lines)
    {
      unsigned int number{ to_number(row) };
      //std::cout << "\tParsed row " << row << " to :" << number << std::endl;
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
      //std::cout << "\tParsed column " << temp << " to :" << number << std::endl;
      m_columns.push_back(to_number(temp));
    }
  }

  int rows() const
  {
    for (int index = 0; index < m_rows.size() - 1; ++index)
    {
      if (vertical_reflection(index))
      {
        return index + 1;
      }
    }

    return -1;
  }

  int columns() const
  {
    for (int index = 0; index < m_columns.size() - 1; ++index)
    {
      if (horizontal_reflection(index))
      {
        return index + 1;
      }
    }

    return -1;
  }

private:
  bool horizontal_reflection(const int index) const
  {
    int left_index = index;
    int right_index = index + 1;
    while (left_index >= 0 && right_index < m_columns.size())
    {
      if (m_columns[left_index] != m_columns[right_index])
      {
        return false;
      }
      --left_index;
      ++right_index;
    }

    return true;
  }

  bool vertical_reflection(const int index) const
  {
    int top_index = index;
    int bottom_index = index + 1;
    while (top_index >= 0 && bottom_index < m_rows.size())
    {
      if (m_rows[top_index] != m_rows[bottom_index])
      {
        return false;
      }

      --top_index;
      ++bottom_index;
    }

    return true;
  }

private:
  std::vector<unsigned int> m_rows;
  std::vector<unsigned int> m_columns;
};

std::vector<Valley> parse_valleys(const advent_of_code::InputHandler& input)
{
  std::vector<std::string> buffer;
  std::vector<Valley> valleys;
  for (const std::string& line : input.read_all_lines(true))
  {
    if (line.empty())
    {
      valleys.push_back(Valley{ buffer });
      buffer.clear();
    }
    else
    {
      buffer.push_back(line);
    }
  }

  if (!buffer.empty())
  {
    valleys.push_back(Valley{ buffer });
  }

  return valleys;
}
}

int main(int argc, char* argv[])
{
  cxxopts::Options options("d13", "Day 13 of Advent of Code");
  options.add_options()
    ("f,filename", "Input Filename", cxxopts::value<std::string>())
    ("h,help", "Print usage")
  ;

  auto result = options.parse(argc, argv);
  if (result.count("help"))
  {
    std::cout << options.help() << std::endl;
    return 0;
  }

  if (!result.count("filename"))
  {
    std::cout << "ERROR: Missing required argument: <filename>" << std::endl;
    std::cout << options.help() << std::endl;
    return -1;
  }

  advent_of_code::InputHandler input{ result["filename"].as<std::string>() };
  std::vector<Valley> valleys{ parse_valleys(input) };

  unsigned long long rows{0}, cols{0};
  for (const Valley& v : valleys)
  {
    int r{ v.rows() };
    if (r != -1)
    {
      rows += r;
    }

    int c{ v.columns() };
    if (c != -1)
    {
      cols += c;
    }
  }

  std::cout << (100 * rows) + cols << std::endl;

  return 0;
}
