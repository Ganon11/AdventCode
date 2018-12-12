// Day6.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "Point.h"

#include "../AoCHelpers/InputHandler.h"

typedef std::vector<std::vector<size_t>> TwoDimensionalMap;

bool get_points(std::vector<Point>& points, const AdventOfCode::InputHandler& input);
size_t closest_point(const Point& p, const std::vector<Point>& all_points);

TwoDimensionalMap create_map(const std::vector<Point>& points);
unsigned int find_size_of_area(const TwoDimensionalMap& map, const size_t index);
size_t find_index_of_largest_area(const TwoDimensionalMap& map, const size_t num_points,
    size_t& largest_area);

size_t find_size_of_safe_area(const std::vector<Point>& points);

int wmain(int argc, wchar_t* argv[]) {
  AdventOfCode::InputHandler input{ argc, argv };
  std::vector<Point> points;
  if (!get_points(points, input)) {
    std::wcout << L"Couldn't parse input." << std::endl;
    return -1;
  }

  /////////////////////////////////////////////////////////////////////////////
  // Part 1
  /////////////////////////////////////////////////////////////////////////////
  const TwoDimensionalMap map{ create_map(points) };
  size_t largest_area;
  const size_t index_with_largest_area{ find_index_of_largest_area(map, points.size(),
      largest_area) };
  std::wcout << L"Largest Area - " << largest_area << std::endl;

  /////////////////////////////////////////////////////////////////////////////
  // Part 2
  /////////////////////////////////////////////////////////////////////////////
  const size_t safe_area = find_size_of_safe_area(points);
  std::wcout << L"Safe Area - " << safe_area << std::endl;

  return 0;
}

bool get_points(std::vector<Point>& points, const AdventOfCode::InputHandler& input) {
  for (const std::wstring& line : input.read_all_lines()) {
    points.push_back(line);
  }

  return true;
}

size_t closest_point(const Point& p, const std::vector<Point>& all_points) {
  int min_distance{ std::numeric_limits<int>::max() };
  size_t min_distance_index;
  bool is_tied{ false };

  for (size_t index = 0; index < all_points.size(); ++index) {
    int distance{ p.DistanceTo(all_points[index]) };
    if (distance < min_distance) {
      min_distance = distance;
      min_distance_index = index;
      is_tied = false;
    } else if (distance == min_distance) {
      is_tied = true;
    }
  }

  if (is_tied) {
    return -1;
  } else {
    return min_distance_index;
  }
}

TwoDimensionalMap create_map(const std::vector<Point>& points) {
  auto x_comparator{ [](const Point& p1, const Point& p2) { return p1.m_x < p2.m_x; } };
  auto y_comparator{ [](const Point& p1, const Point& p2) { return p1.m_y < p2.m_y; } };
  unsigned int largest_x{ std::max_element(points.begin(), points.end(), x_comparator)->m_x };
  unsigned int largest_y{ std::max_element(points.begin(), points.end(), y_comparator)->m_y };
  TwoDimensionalMap map;
  map.reserve(largest_y + 1);

  for (size_t row = 0; row <= largest_y; ++row) {
    std::vector<size_t> current_row;
    current_row.reserve(largest_x + 1);
    for (size_t col = 0; col <= largest_x; ++col) {
      Point p{ col, row };
      size_t closest{ closest_point(p, points) };
      current_row.push_back(closest);
    }

    map.push_back(current_row);
  }

  return map;
}

unsigned int find_size_of_area(const TwoDimensionalMap& map, const size_t index) {
  unsigned int sum = 0;
  for (size_t row = 0; row < map.size(); ++row) {
    sum += static_cast<int>(count(map[row].begin(), map[row].end(), index));
  }

  return sum;
}

size_t find_index_of_largest_area(const TwoDimensionalMap& map, const size_t num_points,
    size_t& largest_area) {
  std::set<size_t> infinite_areas;
  // Check top row (row = 0) and bottom row (row = map.size() - 1)
  for (size_t col = 0; col < map[0].size(); ++col) {
    infinite_areas.insert(map[0][col]);
    infinite_areas.insert(map[map.size() - 1][col]);
  }

  // Check left column (col = 0) and right column (col = map[0].size() - 1)
  for (size_t row = 0; row < map.size(); ++row) {
    infinite_areas.insert(map[row][0]);
    infinite_areas.insert(map[row][map[row].size() - 1]);
  }

  unsigned int max_area{ std::numeric_limits<unsigned int>::min() };
  size_t largest_index;

  for (size_t index = 0; index < num_points; ++index) {
    if (infinite_areas.end() != infinite_areas.find(index)) {
      // The point at this index has infinite area, and does not count.
      continue;
    }

    unsigned int area{ find_size_of_area(map, index) };
    if (area > max_area) {
      max_area = area;
      largest_index = index;
    }
  }

  largest_area = max_area;
  return largest_index;
}

namespace {
  const int MAX_ALLOWED_DISTANCE{ 10000 };
}

size_t find_size_of_safe_area(const std::vector<Point>& points) {
  auto x_comparator{ [](const Point& p1, const Point& p2) { return p1.m_x < p2.m_x; } };
  auto y_comparator{ [](const Point& p1, const Point& p2) { return p1.m_y < p2.m_y; } };
  unsigned int largest_x{ std::max_element(points.begin(), points.end(), x_comparator)->m_x };
  unsigned int largest_y{ std::max_element(points.begin(), points.end(), y_comparator)->m_y };

  int points_in_safe_area = 0;
  for (size_t row = 0; row <= largest_y; ++row) {
    for (size_t col = 0; col <= largest_x; ++col) {
      Point p{ col, row };
      int sum_distance{ 0 };
      for (size_t index = 0; index < points.size(); ++index) {
        sum_distance += p.DistanceTo(points[index]);
      }

      if (sum_distance < MAX_ALLOWED_DISTANCE) {
        ++points_in_safe_area;
      }
    }
  }

  return points_in_safe_area;
}
