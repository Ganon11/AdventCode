#pragma once

#include <iostream>
#include <limits>

struct Position
{
  Position() = default;
  Position(const unsigned int x, const unsigned int y);
  Position(const Position& p) = default;
  bool operator<(const Position& other) const;
  bool operator==(const Position& other) const;
  bool operator!=(const Position& other) const;

  friend std::wostream& operator<<(std::wostream& out, const Position& t);

  unsigned int m_x;
  unsigned int m_y;
};

const Position NONE_POSITION{
    std::numeric_limits<unsigned int>::max(), std::numeric_limits<unsigned int>::max() };
