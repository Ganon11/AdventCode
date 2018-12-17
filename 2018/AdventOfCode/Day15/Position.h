#pragma once

#include <iostream>
#include <limits>
#include <vector>

struct Position
{
  Position() = default;
  Position(const unsigned int x, const unsigned int y);
  Position(const Position& p) = default;
  bool operator<(const Position& other) const;
  bool operator==(const Position& other) const;
  bool operator!=(const Position& other) const;
  bool is_adjacent_to(const Position& other) const;

  std::vector<Position> get_adjacent_positions() const;
  size_t distance_to(const Position& other) const;

  friend std::wostream& operator<<(std::wostream& out, const Position& t);

  unsigned int m_x;
  unsigned int m_y;
};

const Position NONE_POSITION{
    std::numeric_limits<unsigned int>::max(), std::numeric_limits<unsigned int>::max() };
