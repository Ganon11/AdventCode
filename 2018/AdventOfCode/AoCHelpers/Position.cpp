#include "stdafx.h"
#include "Position.h"

advent_of_code::Position::Position(const long long x, const long long y) : m_x{ x }, m_y{ y }
{}

bool advent_of_code::Position::operator<(const Position& other) const {
   if (m_y < other.m_y) {
      return true;
   }

   if (m_y == other.m_y && m_x < other.m_x) {
      return true;
   }

   return false;
}

bool advent_of_code::Position::operator>(const Position& other) const {
  if (m_y > other.m_y) {
    return true;
  }

  if (m_y == other.m_y && m_x > other.m_x) {
    return true;
  }

  return false;
}

bool advent_of_code::Position::operator==(const Position& other) const {
   return m_x == other.m_x && m_y == other.m_y;
}

bool advent_of_code::Position::operator!=(const Position& other) const {
   return !operator==(other);
}

bool advent_of_code::Position::is_adjacent_to(const Position& other) const {
   if (m_x == other.m_x && 1 == abs(m_y - other.m_y)) {
      return true;
   }

   if (m_y == other.m_y && 1 == abs(m_x - other.m_x)) {
      return true;
   }

   return false;
}

advent_of_code::Position advent_of_code::Position::north() const {
  return Position{ m_x, m_y - 1 };
}

advent_of_code::Position advent_of_code::Position::south() const {
  return Position{ m_x, m_y + 1 };
}

advent_of_code::Position advent_of_code::Position::east() const {
  return Position{ m_x + 1, m_y };
}

advent_of_code::Position advent_of_code::Position::west() const {
  return Position{ m_x - 1, m_y };
}

std::vector<advent_of_code::Position> advent_of_code::Position::get_adjacent_positions() const {
   return std::vector<Position>{
      north(),
      west(),
      east(),
      south()
   };
}

size_t advent_of_code::Position::distance_to(const Position& other) const {
   return static_cast<size_t>(abs(m_x - other.m_x) + abs(m_y - other.m_y));
}

std::wostream& advent_of_code::operator<<(std::wostream& out, const advent_of_code::Position& p) {
   out << p.m_x << L',' << p.m_y;
   return out;
}
