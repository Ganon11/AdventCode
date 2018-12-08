#include "stdafx.h"
#include <fstream>
#include <sstream>

#include "Filenames.h"
#include "InputHandler.h"

AdventOfCode::InputHandler::InputHandler() : InputHandler(Filenames::get_filename())
{ }

AdventOfCode::InputHandler::InputHandler(const wchar_t* filename) : m_filename(filename)
{ }

std::wstring AdventOfCode::InputHandler::read_single_line() const
{
   std::vector<std::wstring> lines;
   std::wifstream input{ m_filename };
   std::wstring line;
   getline(input, line);
   return line;
}

std::vector<std::wstring> AdventOfCode::InputHandler::read_all_lines() const
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
