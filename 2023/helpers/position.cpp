#include "position.h"

advent_of_code::Position::Position(const long long x, const long long y, const long long z, const long long a)
  : m_x{ x }, m_y{ y }, m_z{ z }, m_a{ a }
{}

bool advent_of_code::Position::operator<(const Position& other) const
{
  if (m_a < other.m_a)
  {
    return true;
  }

  if (m_a == other.m_a && m_z < other.m_z)
  {
    return true;
  }

  if (m_a == other.m_a && m_z == other.m_z && m_y < other.m_y)
  {
    return true;
  }

  if (m_a == other.m_a && m_z == other.m_z && m_y == other.m_y && m_x < other.m_x)
  {
    return true;
  }

  return false;
}

bool advent_of_code::Position::operator>(const Position& other) const
{
  if (m_a > other.m_a)
  {
    return true;
  }

  if (m_a == other.m_a && m_z > other.m_z)
  {
    return true;
  }

  if (m_a == other.m_a && m_z == other.m_z && m_y > other.m_y)
  {
    return true;
  }

  if (m_a == other.m_a && m_z == other.m_z && m_y == other.m_y && m_x > other.m_x)
  {
    return true;
  }

  return false;
}

bool advent_of_code::Position::operator==(const Position& other) const
{
  return m_x == other.m_x && m_y == other.m_y && m_z == other.m_z && m_a == other.m_a;
}

bool advent_of_code::Position::operator!=(const Position& other) const
{
  return !operator==(other);
}

bool advent_of_code::Position::is_adjacent_to(const Position& other) const
{
  if (m_x == other.m_x && m_y == other.m_y && m_z == other.m_z && 1 == llabs(m_a - other.m_a))
  {
    return true;
  }

  if (m_x == other.m_x && m_y == other.m_y && 1 == llabs(m_z - other.m_z) && m_a == other.m_a)
  {
    return true;
  }

  if (m_x == other.m_x && 1 == llabs(m_y - other.m_y) && m_z == other.m_z && m_a == other.m_a)
  {
    return true;
  }

  if (1 == llabs(m_x - other.m_x) && m_y == other.m_y && m_z == other.m_z && m_a == other.m_a)
  {
    return true;
  }

  return false;
}

advent_of_code::Position advent_of_code::Position::north() const
{
  return Position{ m_x, m_y - 1, m_z };
}

advent_of_code::Position advent_of_code::Position::south() const
{
  return Position{ m_x, m_y + 1, m_z };
}

advent_of_code::Position advent_of_code::Position::east() const
{
  return Position{ m_x + 1, m_y, m_z };
}

advent_of_code::Position advent_of_code::Position::west() const
{
  return Position{ m_x - 1, m_y, m_z };
}

std::vector<advent_of_code::Position> advent_of_code::Position::get_adjacent_positions() const
{
  return std::vector<Position>{
    north(),
    west(),
    east(),
    south()
  };
}

advent_of_code::Position advent_of_code::Position::above() const
{
  return Position{ m_x, m_y, m_z - 1 };
}

advent_of_code::Position advent_of_code::Position::below() const
{
  return Position{ m_x, m_y, m_z + 1 };
}

std::vector<advent_of_code::Position> advent_of_code::Position::get_adjacent_positions_3D() const
{
  return std::vector<Position>{
    above(),
    north(),
    west(),
    east(),
    south(),
    below()
  };
}

size_t advent_of_code::Position::distance_to(const Position& other) const
{
  return static_cast<size_t>(llabs(m_x - other.m_x)
                           + llabs(m_y - other.m_y)
                           + llabs(m_z - other.m_z)
                           + llabs(m_a - other.m_a));
}
