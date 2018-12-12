#pragma once

#include <string>
#include <vector>

namespace advent_of_code {
class InputHandler
{
public:
  InputHandler(const int argc, wchar_t** argv);

  std::wstring read_single_line() const;
  std::vector<std::wstring> read_all_lines() const;

private:
  std::wstring m_filename;
};
}
