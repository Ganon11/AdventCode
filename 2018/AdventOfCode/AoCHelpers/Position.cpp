#include "stdafx.h"
#include "Position.h"

advent_of_code::Position::Position(const unsigned int x, const unsigned int y) : m_x{ x }, m_y{ y }
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

bool advent_of_code::Position::operator==(const Position& other) const {
   return m_x == other.m_x && m_y == other.m_y;
}

bool advent_of_code::Position::operator!=(const Position& other) const {
   return !operator==(other);
}

bool advent_of_code::Position::is_adjacent_to(const Position& other) const {
   if (m_x == other.m_x && 1 == abs(static_cast<int>(m_y) - static_cast<int>(other.m_y))) {
      return true;
   }

   if (m_y == other.m_y && 1 == abs(static_cast<int>(m_x) - static_cast<int>(other.m_x))) {
      return true;
   }

   return false;
}

std::vector<advent_of_code::Position> advent_of_code::Position::get_adjacent_positions() const {
   return std::vector<Position>{
      Position(m_x, m_y - 1),
      Position(m_x - 1, m_y),
      Position(m_x + 1, m_y),
      Position(m_x, m_y + 1)
   };
}

size_t advent_of_code::Position::distance_to(const Position& other) const {
   return (abs(static_cast<int>(m_x) - static_cast<int>(other.m_x))
         + abs(static_cast<int>(m_y) - static_cast<int>(other.m_y)));
}

std::wostream& advent_of_code::operator<<(std::wostream& out, const advent_of_code::Position& p) {
   out << p.m_x << L',' << p.m_y;
   return out;
}
