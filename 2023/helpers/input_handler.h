#pragma once

#include <filesystem>
#include <map>
#include <string>
#include <vector>

namespace advent_of_code {
class InputHandler
{
public:
  InputHandler(const std::filesystem::path& path);

  std::string read_single_line() const;
  std::vector<std::string> read_all_lines(const bool include_blanks = false) const;

private:
  std::filesystem::path m_path;
};

std::vector<std::string> tokenize(const std::string& line, const char separator);
std::vector<std::string> tokenize(const std::string& line, const std::string& separator);
}
