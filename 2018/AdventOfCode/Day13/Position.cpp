#include "pch.h"
#include "Position.h"

Position::Position(const unsigned int x, const unsigned int y) : m_x{ x }, m_y{ y }
{}

bool Position::operator<(const Position& other) const {
  if (m_x < other.m_x) {
    return true;
  }

  if (m_x == other.m_x && m_y < other.m_y) {
    return true;
  }

  return false;
}

bool Position::operator==(const Position& other) const {
  return m_x == other.m_x && m_y == other.m_y;
}

bool Position::operator!=(const Position& other) const {
  return !operator==(other);
}

std::wostream& operator<<(std::wostream& out, const Position& p) {
  out << p.m_x << L',' << p.m_y;
  return out;
}
