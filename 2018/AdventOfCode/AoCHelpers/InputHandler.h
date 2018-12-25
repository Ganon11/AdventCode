#pragma once

#include <map>
#include <string>
#include <vector>

namespace advent_of_code {
class InputHandler
{
public:
  InputHandler(const int argc, wchar_t** argv);

  std::wstring read_single_line() const;
  std::vector<std::wstring> read_all_lines() const;

  bool get_argument(const std::wstring& flag, std::wstring& value) const;

private:
  std::wstring m_filename;
  std::map<std::wstring, std::wstring> m_arguments;
};
}
