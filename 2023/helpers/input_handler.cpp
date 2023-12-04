#include <cstring>
#include <fstream>
#include <sstream>
#include <string>

#include <iostream>

#include "input_handler.h"

advent_of_code::InputHandler::InputHandler(const std::string& filename) : m_filename{ filename }
{}

std::string advent_of_code::InputHandler::read_single_line() const
{
  std::vector<std::string> lines;
  std::ifstream input{ m_filename };
  std::string line;
  getline(input, line);
  return line;
}

std::vector<std::string> advent_of_code::InputHandler::read_all_lines() const
{
  std::vector<std::string> lines;
  std::ifstream input{ m_filename };
  std::string line;
  while (input.good())
  {
    getline(input, line);
    if (!line.empty())
    {
      lines.push_back(line);
    }
  }

  return lines;
}

std::vector<std::string> advent_of_code::tokenize(const std::string& line, const char separator = ' ')
{
  std::vector<std::string> tokens;
  size_t index = 0;

  while (index < line.size())
  {
    std::string temp;
    size_t position = line.find(separator, index);
    if (position == std::string::npos)
    {
      temp = line.substr(index);
      index = line.size();
    }
    else
    {
      temp = line.substr(index, position - index);
      index = position + 1;
    }

    if (!temp.empty())
    {
      tokens.push_back(temp);
    }
  }

  return tokens;
}

std::vector<std::string> advent_of_code::tokenize(const std::string& line, const std::string& separator)
{
  std::vector<std::string> tokens;
  size_t index = 0;

  while (index < line.size())
  {
    std::string temp;
    size_t position = line.find(separator, index);
    if (position == std::string::npos)
    {
      temp = line.substr(index);
      index = line.size();
    }
    else
    {
      temp = line.substr(index, position - index);
      index = position + separator.size();
    }

    if (!temp.empty())
    {
      tokens.push_back(temp);
    }
  }

  return tokens;
}
