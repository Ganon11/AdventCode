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

  const char* str = &line[0];
  do
  {
    const char* begin = str;

    while (*str != separator && *str)
    {
      ++str;
    }

    tokens.push_back(std::string{ begin, str });
  } while ('\0' != *str++);

  return tokens;
}

std::vector<std::string> advent_of_code::tokenize(const std::string& line, const std::string& separator)
{
  //std::cout << "Tokenizing string \"" << line << "\" by delimiter \"" << separator << "\"" << std::endl;
  std::vector<std::string> tokens;
  size_t index = 0;

  while (index < line.size())
  {
    std::string temp;
    size_t position = line.find(separator, index);
    if (position == std::string::npos)
    {
      //std::cout << "\tDelimiter not found, taking the rest of the string." << std::endl;
      temp = line.substr(index);
      index = line.size();
    }
    else
    {
      //std::cout << "\tDelimiter found at " << position << ", taking substring." << std::endl;
      temp = line.substr(index, position - index);
      index = position + separator.size();
      //std::cout << "\tIndex updated to " << index << std::endl;
    }

    if (!temp.empty())
    {
      //std::cout << "\tAdding token \"" << temp << "\"" << std::endl;
      tokens.push_back(temp);
    }
  }

  return tokens;
}
