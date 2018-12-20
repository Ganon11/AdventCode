// Day18.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

#include "../AoCHelpers/InputHandler.h"

enum AcreType
{
  OPEN,
  TREES,
  LUMBERYARD
};

typedef std::vector<std::vector<AcreType>> LumberCollectionArea;
typedef std::map<LumberCollectionArea, unsigned long long> MemoizationMap;

LumberCollectionArea read_lumber_area(const advent_of_code::InputHandler& input);
wchar_t acretype_to_string(const AcreType acre);
AcreType string_to_acretype(const wchar_t ch);
void print_area(const LumberCollectionArea& area);
AcreType get_new_acretype(const LumberCollectionArea& area, const size_t x, const size_t y);
LumberCollectionArea process_minute(const LumberCollectionArea& area);
void count_acre_types(const LumberCollectionArea& area, unsigned int& sum_trees,
    unsigned int& sum_lumberyards);

int wmain(int argc, wchar_t* argv[]) {
  advent_of_code::InputHandler input{ argc, argv };
  LumberCollectionArea area{ read_lumber_area(input) };

  MemoizationMap memory;
  memory[area] = 0;

  unsigned long long minute{ 1 };
  unsigned int sum_trees{ 0 };
  unsigned int sum_lumberyards{ 0 };
  const unsigned long long MAX_GENERATIONS{ 1000000000 };
  for (; minute <= MAX_GENERATIONS; ++minute) {
    area = process_minute(area);

    // Part 1
    if (minute == 10) {
      count_acre_types(area, sum_trees, sum_lumberyards);
      std::wcout << L"At minute 10, resource value: " << sum_trees << L" trees * "
          << sum_lumberyards << L" lumberyards = " << sum_trees * sum_lumberyards << std::endl;
    }

    auto dejavu{ memory.find(area) };
    if (memory.end() == dejavu) {
      memory[area] = minute;
    } else {
      unsigned long long cycle_length{ minute - dejavu->second };
      std::wcout << L"At minute " << minute << L", cycle of length " << cycle_length
          << L" detected." << std::endl;
      unsigned long long skip = (MAX_GENERATIONS - minute) / cycle_length;
      minute += (skip * cycle_length);
    }
  }

  // Part 2
  count_acre_types(area, sum_trees, sum_lumberyards);
  std::wcout << L"After minute " << minute - 1 << L", resource value: " << sum_trees
      << L" trees * " << sum_lumberyards << L" lumberyards = " << sum_trees * sum_lumberyards
      << std::endl;

  return 0;
}

LumberCollectionArea read_lumber_area(const advent_of_code::InputHandler& input) {
  LumberCollectionArea area;
  std::vector<std::wstring> lines{ input.read_all_lines() };
  area.reserve(lines.size());

  for (const std::wstring& line : lines) {
    std::vector<AcreType> new_row;
    new_row.reserve(line.size());
    for (const wchar_t ch : line) {
      new_row.push_back(string_to_acretype(ch));
    }

    area.push_back(new_row);
  }

  return area;
}

wchar_t acretype_to_string(const AcreType acre) {
  switch (acre) {
  case OPEN:
    return L'.';
  case TREES:
    return L'|';
  case LUMBERYARD:
  default:
    return L'#';
  }
}

AcreType string_to_acretype(const wchar_t ch) {
  switch (ch) {
  case L'|':
    return TREES;
  case L'#':
    return LUMBERYARD;
  case L'.':
  default:
    return OPEN;
  }
}

void print_area(const LumberCollectionArea& area) {
  for (size_t y = 0; y < area.size(); ++y) {
    for (size_t x = 0; x < area[y].size(); ++x) {
      std::wcout << acretype_to_string(area[y][x]);
    }

    std::wcout << std::endl;
  }

  std::wcout << std::endl;
}

AcreType get_new_acretype(const LumberCollectionArea& area, const size_t x, const size_t y) {
  const size_t min_y = (y == 0 ? y : y - 1);
  const size_t max_y = (y == (area.size() - 1) ? y : y + 1);
  const size_t min_x = (x == 0 ? x : x - 1);
  const size_t max_x = (x == (area[y].size() - 1) ? x : x + 1);

  switch (area[y][x]) {
  case OPEN: {
    unsigned int sum_trees = 0;
    for (size_t y_index = min_y; y_index <= max_y; ++y_index) {
      for (size_t x_index = min_x; x_index <= max_x; ++x_index) {
        if (x_index == x && y_index == y) {
          continue;
        }

        if (area[y_index][x_index] == TREES) {
          ++sum_trees;
        }
      }
    }

    return sum_trees >= 3 ? TREES : OPEN;
  }
  case TREES: {
    unsigned int sum_lumberyards = 0;
    for (size_t y_index = min_y; y_index <= max_y; ++y_index) {
      for (size_t x_index = min_x; x_index <= max_x; ++x_index) {
        if (x_index == x && y_index == y) {
          continue;
        }

        if (area[y_index][x_index] == LUMBERYARD) {
          ++sum_lumberyards;
        }
      }
    }

    return sum_lumberyards >= 3 ? LUMBERYARD : TREES;
  }
  case LUMBERYARD: {
    bool adjacent_to_lumberyard{ false };
    bool adjacent_to_trees{ false };
    for (size_t y_index = min_y; y_index <= max_y; ++y_index) {
      for (size_t x_index = min_x; x_index <= max_x; ++x_index) {
        if (x_index == x && y_index == y) {
          continue;
        }

        if (area[y_index][x_index] == LUMBERYARD) {
          adjacent_to_lumberyard = true;
        } else if (area[y_index][x_index] == TREES) {
          adjacent_to_trees = true;
        }
      }
    }

    return (adjacent_to_lumberyard && adjacent_to_trees) ? LUMBERYARD : OPEN;
  }
  default:
    return OPEN;
  }
}

LumberCollectionArea process_minute(const LumberCollectionArea& area) {
  LumberCollectionArea new_area;
  new_area.reserve(area.size());

  for (size_t y = 0; y < area.size(); ++y) {
    std::vector<AcreType> new_row;
    new_row.reserve(area[y].size());
    for (size_t x = 0; x < area[y].size(); ++x) {
      new_row.push_back(get_new_acretype(area, x, y));
    }

    new_area.push_back(new_row);
  }

  return new_area;
}

void count_acre_types(const LumberCollectionArea& area, unsigned int& sum_trees,
  unsigned int& sum_lumberyards) {

  sum_trees = 0;
  sum_lumberyards = 0;
  for (size_t y = 0; y < area.size(); ++y) {
    for (size_t x = 0; x < area[y].size(); ++x) {
      if (area[y][x] == TREES) {
        ++sum_trees;
      } else if (area[y][x] == LUMBERYARD) {
        ++sum_lumberyards;
      }
    }
  }
}
