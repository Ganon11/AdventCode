#pragma once

#include <map>
#include <string>
#include <vector>

namespace advent_of_code {
class InputHandler
{
public:
  InputHandler(const int argc, char** argv);
  InputHandler(const std::string& filename);

  std::string read_single_line() const;
  std::vector<std::string> read_all_lines() const;

  bool get_argument(const std::string& flag, std::string& value) const;

private:
  std::string m_filename;
  std::map<std::string, std::string> m_arguments;
};
}
