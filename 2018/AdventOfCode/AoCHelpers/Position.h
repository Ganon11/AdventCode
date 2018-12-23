#pragma once

#include <iostream>
#include <limits>
#include <vector>

namespace advent_of_code {
struct Position
{
  Position() = default;
  Position(const long long x, const long long y, const long long z = 0);
  Position(const Position& p) = default;
  bool operator<(const Position& other) const;
  bool operator>(const Position& other) const;
  bool operator==(const Position& other) const;
  bool operator!=(const Position& other) const;
  bool is_adjacent_to(const Position& other) const;

  Position north() const;
  Position south() const;
  Position east() const;
  Position west() const;
  std::vector<Position> get_adjacent_positions() const;
  Position above() const;
  Position below() const;
  std::vector<Position> get_adjacent_positions_3D() const;

  size_t distance_to(const Position& other) const;

  friend std::wostream& operator<<(std::wostream& out, const Position& t);

  long long m_x;
  long long m_y;
  long long m_z;
};

#ifdef max
#undef max
#endif

const Position NONE_POSITION{
    std::numeric_limits<long long>::max(),
    std::numeric_limits<long long>::max(),
    std::numeric_limits<long long>::max() };

const Position ORIGIN{ 0, 0, 0 };
}


namespace std {
template <> struct hash<advent_of_code::Position>
{
  typedef advent_of_code::Position argument_type;
  typedef std::size_t result_type;
  std::size_t operator()(const advent_of_code::Position& p) const noexcept {
    return std::hash<long long>()(p.m_x ^ (p.m_y << 4) ^ (p.m_z << 8));
  }
};
}
