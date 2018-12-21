// Day20.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <unordered_set>
#include <string>
#include <queue>
#include <vector>

#include "../AoCHelpers/InputHandler.h"
#include "../AoCHelpers/Position.h"

enum PositionType
{
  ROOM,
  WALL,
  VERTICAL_DOOR,
  HORIZONTAL_DOOR,
  UNKNOWN
};

wchar_t position_type_to_string(const PositionType p);

typedef std::map<advent_of_code::Position, PositionType> ConstructionProject;

void print_facility(const ConstructionProject& facility,
    const advent_of_code::Position current_position = advent_of_code::NONE_POSITION);

size_t find_matching_paren_index(const std::wstring& str, const size_t start);
void process_regex(const advent_of_code::Position& start, const std::wstring& regex,
    ConstructionProject& facility);
void complete(ConstructionProject& facility);

unsigned long long max_path(const ConstructionProject& facility,
    const advent_of_code::Position& start, unsigned int& far_rooms);

int wmain(int argc, wchar_t* argv[]) {
  advent_of_code::InputHandler input{ argc, argv };
  std::wstring regex{ input.read_single_line() };
  ConstructionProject the_facility;

  // Add starting position
  advent_of_code::Position starting_position{ 0, 0 };
  the_facility[starting_position] = ROOM;

  // Add walls at corners
  the_facility[advent_of_code::Position{ -1, -1 }] = WALL;
  the_facility[advent_of_code::Position{ -1, 1 }] = WALL;
  the_facility[advent_of_code::Position{ 1, -1 }] = WALL;
  the_facility[advent_of_code::Position{ 1, 1 }] = WALL;

  // Add unknowns north, south, east, and west
  the_facility[advent_of_code::Position{ 0, -1 }] = UNKNOWN;
  the_facility[advent_of_code::Position{ 0, 1 }] = UNKNOWN;
  the_facility[advent_of_code::Position{ 1, 0 }] = UNKNOWN;
  the_facility[advent_of_code::Position{ -1, 0 }] = UNKNOWN;

  process_regex(starting_position, regex, the_facility);
  complete(the_facility);
  print_facility(the_facility);

  unsigned int far_rooms{ 0 };
  unsigned long long part1{ max_path(the_facility, starting_position, far_rooms) };

  std::wcout << L"Longest path: " << part1 << L" doors." << std::endl;
  std::wcout << L"There are " << far_rooms << L" rooms at least 1000 doors away." << std::endl;

  return 0;
}

wchar_t position_type_to_string(const PositionType p) {
  switch (p) {
  case ROOM:
    return L'.';
  case WALL:
    return L'#';
  case VERTICAL_DOOR:
    return L'|';
  case HORIZONTAL_DOOR:
    return L'-';
  case UNKNOWN:
  default:
    return L'?';
  }
}

void print_facility(const ConstructionProject& facility,
    const advent_of_code::Position current_position) {
  auto y_comparator{ [](const ConstructionProject::value_type& p1,
      const ConstructionProject::value_type& p2) { return p1.first.m_y < p2.first.m_y; } };
  long long min_y = std::min_element(facility.begin(), facility.end(), y_comparator)->first.m_y;
  long long max_y = std::max_element(facility.begin(), facility.end(), y_comparator)->first.m_y;

  auto x_comparator{ [](const ConstructionProject::value_type& p1,
      const ConstructionProject::value_type& p2) { return p1.first.m_x < p2.first.m_x; } };
  long long min_x = std::min_element(facility.begin(), facility.end(), x_comparator)->first.m_x;
  long long max_x = std::max_element(facility.begin(), facility.end(), x_comparator)->first.m_x;

  system("CLS");

  for (long long y = min_y; y <= max_y; ++y) {
    for (long long x = min_x; x <= max_x; ++x) {
      advent_of_code::Position current{ x, y };
      if (facility.end() == facility.find(current)) {
        std::wcout << L' ';
      } else if (current == current_position) {
        std::wcout << L'O';
      } else if (y == 0 && x == 0) {
        std::wcout << L'X';
      } else {
        std::wcout << position_type_to_string(facility.at(current));
      }
    }

    std::wcout << std::endl;
  }
}

size_t find_matching_paren_index(const std::wstring& str, const size_t start) {
  unsigned int parenCount{ 0 };
  size_t index{ start };
  do {
    if (L'(' == str[index]) {
      ++parenCount;
    } else if (L')' == str[index]) {
      --parenCount;
    }

    ++index;
  } while (parenCount > 0);

  return index;
}

void process_regex(const advent_of_code::Position& start, const std::wstring& regex,
    ConstructionProject& facility) {

  advent_of_code::Position current{ start };
  size_t index = 0;
  while (index < regex.size()) {
#ifdef _DEBUG
    print_facility(facility, current);
#endif
    wchar_t ch{ regex[index] };

    switch (ch) {
    case L'^':
      ++index;
      break;
    case L'$':
      return;
    case L'N':
      current = current.north().north();
      facility[current] = ROOM;
      facility[current.south()] = HORIZONTAL_DOOR;

      // Add new walls to new corners
      facility[current.north().east()] = WALL;
      facility[current.north().west()] = WALL;

      // Add new unknowns north, east, and west - but only if we haven't visited yet
      if (facility.end() == facility.find(current.north())) {
        facility[current.north()] = UNKNOWN;
      }

      if (facility.end() == facility.find(current.east())) {
        facility[current.east()] = UNKNOWN;
      }

      if (facility.end() == facility.find(current.west())) {
        facility[current.west()] = UNKNOWN;
      }
      
      ++index;
      break;
    case L'E':
      current = current.east().east();
      facility[current] = ROOM;
      facility[current.west()] = VERTICAL_DOOR;

      // Add new walls to new corners
      facility[current.east().north()] = WALL;
      facility[current.east().south()] = WALL;

      // Add new unknowns east, north, and south
      if (facility.end() == facility.find(current.east())) {
        facility[current.east()] = UNKNOWN;
      }

      if (facility.end() == facility.find(current.north())) {
        facility[current.north()] = UNKNOWN;
      }

      if (facility.end() == facility.find(current.south())) {
        facility[current.south()] = UNKNOWN;
      }

      ++index;
      break;
    case L'W':
      current = current.west().west();
      facility[current] = ROOM;
      facility[current.east()] = VERTICAL_DOOR;

      // Add new walls to new corners
      facility[current.west().north()] = WALL;
      facility[current.west().south()] = WALL;

      // Add new unknowns west, north, and south
      if (facility.end() == facility.find(current.west())) {
        facility[current.west()] = UNKNOWN;
      }

      if (facility.end() == facility.find(current.north())) {
        facility[current.north()] = UNKNOWN;
      }

      if (facility.end() == facility.find(current.south())) {
        facility[current.south()] = UNKNOWN;
      }

      ++index;
      break;
    case L'S':
      current = current.south().south();
      facility[current] = ROOM;
      facility[current.north()] = HORIZONTAL_DOOR;

      // Add new walls to new corners
      facility[current.south().east()] = WALL;
      facility[current.south().west()] = WALL;

      // Add new unknowns south, east, and west
      if (facility.end() == facility.find(current.south())) {
        facility[current.south()] = UNKNOWN;
      }

      if (facility.end() == facility.find(current.east())) {
        facility[current.east()] = UNKNOWN;
      }

      if (facility.end() == facility.find(current.west())) {
        facility[current.west()] = UNKNOWN;
      }

      ++index;
      break;
    case L'(': {
      size_t end_paren_index{ find_matching_paren_index(regex, index) };
      std::wstring branch{ regex.substr(index + 1, (end_paren_index - index - 2)) };
      std::wstring token;
      unsigned int parenCount{ 0 };
      for (size_t branch_index = 0; branch_index < branch.size(); ++branch_index) {
        switch (branch[branch_index]) {
        case L'|':
          if (parenCount == 0) {
            process_regex(current, token, facility);
            token.clear();
          } else {
            token.push_back(branch[branch_index]);
          }
          break;
        case L'(':
          token.push_back(branch[branch_index]);
          ++parenCount;
          break;
        case L')':
          token.push_back(branch[branch_index]);
          --parenCount;
          break;
        default:
          token.push_back(branch[branch_index]);
        }
      }

      if (!token.empty()) {
        process_regex(current, token, facility);
      }

      index = end_paren_index;
      break;
    }
    default:
      std::wcerr << L"Malformed input - or more likely, I screwed up" << std::endl;
      return;
    }
  }
}

void complete(ConstructionProject& facility) {
  auto y_comparator{ [](const ConstructionProject::value_type& p1,
      const ConstructionProject::value_type& p2) { return p1.first.m_y < p2.first.m_y; } };
  long long min_y = std::min_element(facility.begin(), facility.end(), y_comparator)->first.m_y;
  long long max_y = std::max_element(facility.begin(), facility.end(), y_comparator)->first.m_y;

  auto x_comparator{ [](const ConstructionProject::value_type& p1,
      const ConstructionProject::value_type& p2) { return p1.first.m_x < p2.first.m_x; } };
  long long min_x = std::min_element(facility.begin(), facility.end(), x_comparator)->first.m_x;
  long long max_x = std::max_element(facility.begin(), facility.end(), x_comparator)->first.m_x;

  for (long long y = min_y; y <= max_y; ++y) {
    for (long long x = min_x; x <= max_x; ++x) {
      advent_of_code::Position p{ x, y };
      if (facility.end() != facility.find(p) && UNKNOWN == facility.at(p)) {
        facility[p] = WALL;
      }
    }
  }
}

unsigned long long max_path(const ConstructionProject& facility,
    const advent_of_code::Position& start, unsigned int& far_rooms) {

  std::queue<advent_of_code::Position> frontier;
  frontier.push(start);

  std::map<advent_of_code::Position, unsigned long long> cost_so_far;
  cost_so_far[start] = 0;

  while (!frontier.empty()) {
    advent_of_code::Position current = frontier.front();
    frontier.pop();

    auto cost{ cost_so_far.find(current) };

    advent_of_code::Position n{ current.north() };
    advent_of_code::Position e{ current.east() };
    advent_of_code::Position w{ current.west() };
    advent_of_code::Position s{ current.south() };

    if (cost_so_far.end() == cost_so_far.find(n.north())
        && facility.end() != facility.find(n)
        && facility.at(n) == HORIZONTAL_DOOR) {

      frontier.push(n.north());
      cost_so_far[n.north()] = cost->second + 1;
    }

    if (cost_so_far.end() == cost_so_far.find(e.east())
      && facility.end() != facility.find(e)
      && facility.at(e) == VERTICAL_DOOR) {

      frontier.push(e.east());
      cost_so_far[e.east()] = cost->second + 1;
    }

    if (cost_so_far.end() == cost_so_far.find(w.west())
      && facility.end() != facility.find(w)
      && facility.at(w) == VERTICAL_DOOR) {

      frontier.push(w.west());
      cost_so_far[w.west()] = cost->second + 1;
    }

    if (cost_so_far.end() == cost_so_far.find(s.south())
      && facility.end() != facility.find(s)
      && facility.at(s) == HORIZONTAL_DOOR) {

      frontier.push(s.south());
      cost_so_far[s.south()] = cost->second + 1;
    }
  }

  far_rooms = std::count_if(cost_so_far.begin(), cost_so_far.end(),
      [](const auto& c){ return c.second >= 1000; });

  return std::max_element(cost_so_far.begin(), cost_so_far.end(),
      [](const auto& c1, const auto& c2){ return c1.second < c2.second; })->second;
}
