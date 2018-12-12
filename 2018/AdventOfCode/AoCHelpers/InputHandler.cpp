#include "stdafx.h"
#include <fstream>
#include <sstream>

#include "Filenames.h"
#include "InputHandler.h"
#include "Sample.h"

advent_of_code::InputHandler::InputHandler(const int argc, wchar_t** argv) {
  bool filename_set = false;
  for (int argv_index = 1; argv_index < argc; ++argv_index) {
    if (0 == _wcsicmp(L"-use-sample", argv[argv_index])) {
      sample::set_use_sample(true);
    } else if (0 == _wcsicmp(L"-filename", argv[argv_index]) && argv_index + 1 < argc) {
      m_filename = std::wstring(argv[argv_index + 1]);
      ++argv_index;
      filename_set = true;
    }
  }

  if (!filename_set) {
    m_filename = Filenames::get_filename();
  }
}

std::wstring advent_of_code::InputHandler::read_single_line() const
{
  std::vector<std::wstring> lines;
  std::wifstream input{ m_filename };
  std::wstring line;
  getline(input, line);
  return line;
}

std::vector<std::wstring> advent_of_code::InputHandler::read_all_lines() const
{
  std::vector<std::wstring> lines;
  std::wifstream input{ m_filename };
  std::wstring line;
  while (input.good())
  {
    getline(input, line);
    lines.push_back(line);
  }

  return lines;
}
