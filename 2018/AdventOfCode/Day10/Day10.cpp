// Day10.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../AoCHelpers/InputHandler.h"
#include "Star.h"

bool star_map_is_small_enough(const std::vector<Star>& stars);
bool write_stars(const std::vector<Star>& stars);
__int64 move_stars(std::vector<Star>& stars);

int wmain(int argc, wchar_t* argv[]) {
  advent_of_code::InputHandler input{ argc, argv };
  std::vector<Star> all_stars; // Hey now
  for (const std::wstring& line : input.read_all_lines()) {
    all_stars.push_back(line);
  }

  __int64 size{ std::numeric_limits<__int64>::max() };
  unsigned int time{ 0 };
  while (true) {
    if (star_map_is_small_enough(all_stars)) {
      write_stars(all_stars);
    }

    __int64 new_size{ move_stars(all_stars) };
    if (new_size > size) {
      break;
    }

    size = new_size;
    ++time;
  }

  std::wcout << L"Message found after " << time << L" seconds." << std::endl;
  return 0;
}

bool star_map_is_small_enough(const std::vector<Star>& stars) {
  auto x_comparator{ [](const Star& s1, const Star& s2) {
      return s1.get_position().m_x < s2.get_position().m_x; } };
  auto y_comparator{ [](const Star& s1, const Star& s2) {
      return s1.get_position().m_y < s2.get_position().m_y; } };
  int min_x = std::min_element(stars.begin(), stars.end(), x_comparator)->get_position().m_x;
  int min_y = std::min_element(stars.begin(), stars.end(), y_comparator)->get_position().m_y;
  int max_x = std::max_element(stars.begin(), stars.end(), x_comparator)->get_position().m_x;
  int max_y = std::max_element(stars.begin(), stars.end(), y_comparator)->get_position().m_y;

  if ((max_x - min_x) < 400 && (max_y - min_y) < 300) {
    return true;
  }

  return false;
}

bool write_stars(const std::vector<Star>& stars) {
  auto x_comparator{ [](const Star& s1, const Star& s2) {
      return s1.get_position().m_x < s2.get_position().m_x; } };
  auto y_comparator{ [](const Star& s1, const Star& s2) {
      return s1.get_position().m_y < s2.get_position().m_y; } };
  int min_x = std::min_element(stars.begin(), stars.end(), x_comparator)->get_position().m_x;
  int min_y = std::min_element(stars.begin(), stars.end(), y_comparator)->get_position().m_y;
  int max_x = std::max_element(stars.begin(), stars.end(), x_comparator)->get_position().m_x;
  int max_y = std::max_element(stars.begin(), stars.end(), y_comparator)->get_position().m_y;

  std::wofstream output{ L"StarMap.txt" };
  if (!output.good()) {
    return false;
  }

  for (int y = min_y; y <= max_y; ++y) {
    for (int x = min_x; x <= max_x; ++x) {
      auto star_is_at_position{ [x, y](const Star& s) {
          auto position{ s.get_position() };
          return position.m_x == x && position.m_y == y;
      }};

      if (std::any_of(stars.begin(), stars.end(), star_is_at_position)) {
        output << L'#';
      } else {
        output << L' ';
      }
    }

    output << std::endl;
  }

  return true;
}

__int64 move_stars(std::vector<Star>& stars) {
  for (Star& star : stars) {
    star.move();
  }

  auto x_comparator{ [](const Star& s1, const Star& s2) {
      return s1.get_position().m_x < s2.get_position().m_x; } };
  __int64 min_x =
      std::min_element(stars.begin(), stars.end(), x_comparator)->get_position().m_x;
  __int64 max_x =
    std::max_element(stars.begin(), stars.end(), x_comparator)->get_position().m_x;

  auto y_comparator{ [](const Star& s1, const Star& s2) {
      return s1.get_position().m_y < s2.get_position().m_y; } };
  __int64 min_y =
      std::min_element(stars.begin(), stars.end(), y_comparator)->get_position().m_y;
  __int64 max_y =
      std::max_element(stars.begin(), stars.end(), y_comparator)->get_position().m_y;

  return (max_x - min_x) * (max_y - min_y);
}
