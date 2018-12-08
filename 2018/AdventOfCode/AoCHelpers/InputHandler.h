#pragma once

#include <string>
#include <vector>

namespace AdventOfCode
{
   class InputHandler
   {
   public:
      InputHandler();
      InputHandler(const wchar_t* filename);

      std::wstring read_single_line() const;
      std::vector<std::wstring> read_all_lines() const;

   private:
      std::wstring m_filename;
   };
}
