// Day17.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <regex>
#include <set>
#include <vector>

#include "../AoCHelpers/InputHandler.h"
#include "../AoCHelpers/Position.h"

#include <set>

int wmain(int argc, wchar_t* argv[]) {
  advent_of_code::InputHandler input{ argc, argv };
  std::vector<std::wstring> lines{ input.read_all_lines() };
  std::wregex vertical_line_regex{ L"x=(\\d+), y=(\\d+)..(\\d+)" };
  std::wregex horizontal_line_regex{ L"y=(\\d+), x=(\\d+)..(\\d+)" };
  std::wsmatch matches;
  std::set<advent_of_code::Position> walls;
  unsigned int min_x{ std::numeric_limits<unsigned int>::max() };
  unsigned int max_x{ std::numeric_limits<unsigned int>::min() };
  unsigned int max_y{ std::numeric_limits<unsigned int>::min() };

  for (const std::wstring& line : lines) {
    if (std::regex_search(line, matches, vertical_line_regex) && matches.size() == 4) {
      unsigned int x{ static_cast<unsigned int>(_wtoi(matches[1].str().c_str())) };
      if (x < min_x) {
        min_x = x;
      }

      if (x > max_x) {
        max_x = x;
      }

      unsigned int start_y{ static_cast<unsigned int>(_wtoi(matches[2].str().c_str())) };
      unsigned int end_y{ static_cast<unsigned int>(_wtoi(matches[3].str().c_str())) };
      for (unsigned int y = start_y; y <= end_y; ++y) {
        if (y > max_y) {
          max_y = y;
        }

        walls.insert(advent_of_code::Position{x, y});
      }
    } else if (std::regex_search(line, matches, horizontal_line_regex) && matches.size() == 4) {
      unsigned int y{ static_cast<unsigned int>(_wtoi(matches[1].str().c_str())) };
      if (y > max_y) {
        max_y = y;
      }

      unsigned int start_x{ static_cast<unsigned int>(_wtoi(matches[2].str().c_str())) };
      unsigned int end_x{ static_cast<unsigned int>(_wtoi(matches[3].str().c_str())) };
      for (unsigned int x = start_x; x <= end_x; ++x) {
        if (x < min_x) {
          min_x = x;
        }

        if (x > max_x) {
          max_x = x;
        }

        walls.insert(advent_of_code::Position{ x, y });
      }
    }
  }

  for (unsigned int y = 0; y <= max_y + 1; ++y) {
    for (unsigned int x = min_x - 1; x <= max_x + 1; ++x) {
      if (x == 500 && y == 0) {
        std::wcout << L'+';
        continue;
      }

      if (walls.end() == walls.find(advent_of_code::Position{ x, y })) {
        std::wcout << L' ';
      } else {
        std::wcout << L'#';
      }
    }
    std::wcout << std::endl;
  }

  return 0;
}
