#include <cstring>
#include <fstream>
#include <sstream>
#include <string>

#include "filenames.h"
#include "input_handler.h"
#include "sample.h"

advent_of_code::InputHandler::InputHandler(const int argc, char** argv)
{
  bool filename_set = false;
  for (int argv_index = 1; argv_index < argc; ++argv_index)
  {
    if (0 == strcmp("-use-sample", argv[argv_index]))
    {
      sample::set_use_sample(true);
    }
    else if (0 == strcmp("-filename", argv[argv_index]) && argv_index + 1 < argc)
    {
      m_filename = std::string(argv[argv_index + 1]);
      ++argv_index;
      filename_set = true;
    }
    else if (argv_index + 1 < argc)
    {
      m_arguments[argv[argv_index]] = argv[argv_index + 1];
    }
  }

  if (!filename_set)
  {
    m_filename = filenames::get_filename();
  }
}

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
    lines.push_back(line);
  }

  return lines;
}

bool advent_of_code::InputHandler::get_argument(const std::string& flag, std::string& value) const
{
  auto arg{ m_arguments.find(flag) };
  if (m_arguments.end() == arg)
  {
    return false;
  }

  value = arg->second;
  return true;
}
