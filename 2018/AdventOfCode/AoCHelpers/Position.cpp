#include "stdafx.h"
#include "Position.h"

advent_of_code::Position::Position(const long long x, const long long y, const long long z)
  : m_x{ x }, m_y{ y }, m_z{ z }
{}

bool advent_of_code::Position::operator<(const Position& other) const {
  if (m_z < other.m_z) {
    return true;
  }

  if (m_z == other.m_z && m_y < other.m_y) {
    return true;
  }

  if (m_z == other.m_z && m_y == other.m_y && m_x < other.m_x) {
    return true;
  }

  return false;
}

bool advent_of_code::Position::operator>(const Position& other) const {
  if (m_z > other.m_z) {
    return true;
  }

  if (m_z == other.m_z && m_y > other.m_y) {
    return true;
  }

  if (m_z == other.m_z && m_y == other.m_y && m_x > other.m_x) {
    return true;
  }

  return false;
}

bool advent_of_code::Position::operator==(const Position& other) const {
  return m_x == other.m_x && m_y == other.m_y && m_z == other.m_z;
}

bool advent_of_code::Position::operator!=(const Position& other) const {
  return !operator==(other);
}

bool advent_of_code::Position::is_adjacent_to(const Position& other) const {
  if (m_x == other.m_x && m_y == other.m_y && 1 == abs(m_z - other.m_z)) {
    return true;
  }

  if (m_x == other.m_x && 1 == abs(m_y - other.m_y) && m_z == other.m_z) {
    return true;
  }

  if (1 == abs(m_x - other.m_x) && m_y == other.m_y && m_z == other.m_z) {
    return true;
  }

  return false;
}

advent_of_code::Position advent_of_code::Position::north() const {
  return Position{ m_x, m_y - 1, m_z };
}

advent_of_code::Position advent_of_code::Position::south() const {
  return Position{ m_x, m_y + 1, m_z };
}

advent_of_code::Position advent_of_code::Position::east() const {
  return Position{ m_x + 1, m_y, m_z };
}

advent_of_code::Position advent_of_code::Position::west() const {
  return Position{ m_x - 1, m_y, m_z };
}

std::vector<advent_of_code::Position> advent_of_code::Position::get_adjacent_positions() const {
  return std::vector<Position>{
    north(),
    west(),
    east(),
    south()
  };
}

advent_of_code::Position advent_of_code::Position::above() const {
  return Position{ m_x, m_y, m_z - 1 };
}

advent_of_code::Position advent_of_code::Position::below() const {
  return Position{ m_x, m_y, m_z + 1 };
}

std::vector<advent_of_code::Position> advent_of_code::Position::get_adjacent_positions_3D() const {
  return std::vector<Position>{
    above(),
      north(),
      west(),
      east(),
      south(),
      below()
  };
}

size_t advent_of_code::Position::distance_to(const Position& other) const {
  return static_cast<size_t>(abs(m_x - other.m_x) + abs(m_y - other.m_y) + abs(m_z - other.m_z));
}

std::wostream& advent_of_code::operator<<(std::wostream& out, const advent_of_code::Position& p) {
  out << p.m_x << L',' << p.m_y;
  return out;
}
