#include <cstring>
#include <fstream>
#include <sstream>
#include <string>

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
