#pragma once

#include <iostream>
#include <limits>
#include <set>
#include <vector>

namespace advent_of_code {
class Position
{
public:
  Position() = default;
  Position(const unsigned short x, const unsigned short y, const unsigned short z = 0, const unsigned short a = 0);
  Position(const short x, const short y, const short z = 0, const short a = 0);
  Position(const unsigned int x, const unsigned int y, const unsigned int z = 0, const unsigned int a = 0);
  Position(const int x, const int y, const int z = 0, const int a = 0);
  Position(const unsigned long x, const unsigned long y, const unsigned long z = 0, const unsigned long a = 0);
  Position(const long x, const long y, const long z = 0, const long a = 0);
  Position(const unsigned long long x, const unsigned long long y, const unsigned long long z = 0, const unsigned long long a = 0);
  Position(const long long x, const long long y, const long long z = 0, const long long a = 0);
  Position(const std::string& line);

  ~Position() = default;
  Position(const Position& p) = default;
  Position& operator=(const Position& p) = default;

  long long x() const { return m_x; }
  long long y() const { return m_y; }
  long long z() const { return m_z; }
  long long a() const { return m_a; }

  bool operator<(const Position& other) const;
  bool operator>(const Position& other) const;
  bool operator==(const Position& other) const;
  bool operator!=(const Position& other) const;
  bool is_adjacent_to(const Position& other) const;

  Position north() const;
  Position south() const;
  Position east() const;
  Position west() const;
  Position above() const;
  Position below() const;
  Position past() const;
  Position future() const;

  std::vector<Position> get_adjacent_positions(const bool diagonal = false) const;
  std::vector<Position> get_adjacent_positions_3D() const;
  std::vector<Position> get_adjacent_positions_4D() const;

  size_t distance_to(const Position& other) const;

  friend std::ostream& operator<<(std::ostream& out, const Position& p);

private:
  long long m_x;
  long long m_y;
  long long m_z;
  long long m_a; // THE FOURTH DIMENSION WOOOOAH

public:
  static std::vector<Position> get_positions_in_line(const Position& p1, const Position& p2);
  static void get_boundaries(const std::set<Position>& positions, long long& min_row, long long& min_col, long long& max_row, long long& max_col);
  static bool in_bounds(const long long min_row, const long long min_col, const long long max_row, const long long max_col, const Position& p);
  static bool in_bounds(const std::set<Position>& positions, const Position& p);
};

const Position NONE_POSITION{
    std::numeric_limits<long long>::max(),
    std::numeric_limits<long long>::max(),
    std::numeric_limits<long long>::max(),
    std::numeric_limits<long long>::max() };

const Position ORIGIN{ 0ll, 0ll, 0ll, 0ll };
}


namespace std {
template <> struct hash<advent_of_code::Position>
{
  typedef advent_of_code::Position argument_type;
  typedef std::size_t result_type;
  std::size_t operator()(const advent_of_code::Position& p) const noexcept {
    return std::hash<long long>()(p.x() ^ (p.y() << 4) ^ (p.z() << 8) ^ (p.a() << 12));
  }
};
}
