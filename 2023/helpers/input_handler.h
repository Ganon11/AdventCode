#pragma once

#include <map>
#include <string>
#include <vector>

namespace advent_of_code {
class InputHandler
{
public:
  InputHandler(const std::string& filename);

  std::string read_single_line() const;
  std::vector<std::string> read_all_lines() const;

private:
  std::string m_filename;
};

std::vector<std::string> tokenize(const std::string& line, const char separator);
std::vector<std::string> tokenize(const std::string& line, const std::string& separator);
}
