#pragma once

#include <iostream>
#include <limits>
#include <vector>

namespace advent_of_code {
class Position
{
public:
  Position() = default;
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

  std::vector<Position> get_adjacent_positions() const;
  std::vector<Position> get_adjacent_positions_3D() const;
  std::vector<Position> get_adjacent_positions_4D() const;

  size_t distance_to(const Position& other) const;

  friend std::ostream& operator<<(std::ostream& out, const Position& p)
  {
    out << p.m_x << L',' << p.m_y << L',' << p.m_z << L',' << p.m_a;
    return out;
  }

private:
  long long m_x;
  long long m_y;
  long long m_z;
  long long m_a; // THE FOURTH DIMENSION WOOOOAH

public:
  static std::vector<Position> getPositionsInLine(const Position& p1, const Position& p2);
};

#ifdef max
#undef max
#endif

const Position NONE_POSITION{
    std::numeric_limits<long long>::max(),
    std::numeric_limits<long long>::max(),
    std::numeric_limits<long long>::max(),
    std::numeric_limits<long long>::max() };

const Position ORIGIN{ 0, 0, 0, 0 };
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
