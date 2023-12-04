#include "position.h"

#include <vector>

advent_of_code::Position::Position(const long long x, const long long y, const long long z, const long long a)
  : m_x{ x },
    m_y{ y },
    m_z{ z },
    m_a{ a }
{}

advent_of_code::Position::Position(const unsigned long long x, const unsigned long long y, const unsigned long long z, const unsigned long long a)
  : m_x{ static_cast<long long>(x) },
    m_y{ static_cast<long long>(y) },
    m_z{ static_cast<long long>(z) },
    m_a{ static_cast<long long>(a) }
{}

advent_of_code::Position::Position(const std::string& line)
{
  std::vector<std::string> tokens;

  const char* str = &line[0];
  do
  {
    const char* begin = str;

    while (*str != ',' && *str)
    {
      ++str;
    }

    tokens.push_back(std::string{ begin, str });
  } while ('\0' != *str++);

  size_t token_count = tokens.size();
  if (token_count >= 1)
  {
    m_x = stoi(tokens[0]);
  }

  if (token_count >= 2)
  {
    m_y = stoi(tokens[1]);
  }

  if (token_count >= 3)
  {
    m_z = stoi(tokens[2]);
  }

  if (token_count >= 4)
  {
    m_a = stoi(tokens[3]);
  }
}

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
  return Position{ m_x, m_y - 1, m_z, m_a };
}

advent_of_code::Position advent_of_code::Position::south() const
{
  return Position{ m_x, m_y + 1, m_z, m_a };
}

advent_of_code::Position advent_of_code::Position::east() const
{
  return Position{ m_x + 1, m_y, m_z, m_a };
}

advent_of_code::Position advent_of_code::Position::west() const
{
  return Position{ m_x - 1, m_y, m_z, m_a };
}

advent_of_code::Position advent_of_code::Position::above() const
{
  return Position{ m_x, m_y, m_z - 1, m_a };
}

advent_of_code::Position advent_of_code::Position::below() const
{
  return Position{ m_x, m_y, m_z + 1, m_a };
}

advent_of_code::Position advent_of_code::Position::past() const
{
  return Position{ m_x, m_y, m_z, m_a - 1 };
}

advent_of_code::Position advent_of_code::Position::future() const
{
  return Position{ m_x, m_y, m_z, m_a + 1 };
}

std::vector<advent_of_code::Position> advent_of_code::Position::get_adjacent_positions(const bool diagonal) const
{
  std::vector<Position> positions{
    north(),
    west(),
    east(),
    south()
  };

  if (diagonal)
  {
    positions.push_back(north().west());
    positions.push_back(north().east());
    positions.push_back(south().west());
    positions.push_back(south().east());
  }

  return positions;
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

std::vector<advent_of_code::Position> advent_of_code::Position::get_adjacent_positions_4D() const
{
  return std::vector<Position>{
    above(),
    north(),
    west(),
    east(),
    south(),
    below(),
    past(),
    future()
  };
}

size_t advent_of_code::Position::distance_to(const Position& other) const
{
  return static_cast<size_t>(llabs(m_x - other.m_x)
                           + llabs(m_y - other.m_y)
                           + llabs(m_z - other.m_z)
                           + llabs(m_a - other.m_a));
}

std::ostream& advent_of_code::operator<<(std::ostream& out, const advent_of_code::Position& p)
{
  out << p.x() << ", " << p.y() << ", " << p.z() << ", " << p.a();
  return out;
}

std::vector<advent_of_code::Position> advent_of_code::Position::getPositionsInLine(const advent_of_code::Position& p1, const advent_of_code::Position& p2)
{
  std::vector<Position> positions;
  bool good = false;
  long long num_points = 0;

  if (p1 == p2)
  {
    good = true;
    num_points = 1;
  }
  // Horizontal lines
  else if (p1.x() != p2.x() && p1.y() == p2.y() && p1.z() == p2.z() && p1.a() == p2.a())
  {
    good = true;
    num_points = abs(p1.x() - p2.x()) + 1;
  }
  // Vertical lines
  else if (p1.x() == p2.x() && p1.y() != p2.y() && p1.z() == p2.z() && p1.a() == p2.a())
  {
    good = true;
    num_points = abs(p1.y() - p2.y()) + 1;
  }
  // Stacked lines
  else if (p1.x() == p2.x() && p1.y() == p2.y() && p1.z() != p2.z() && p1.a() == p2.a())
  {
    good = true;
    num_points = abs(p1.z() - p2.z()) + 1;
  }
  // Time lines
  else if (p1.x() == p2.x() && p1.y() == p2.y() && p1.z() == p2.z() && p1.a() != p2.a())
  {
    good = true;
    num_points = abs(p1.a() - p2.a()) + 1;
  }
  // X/Y Diagonals
  else if (p1.z() == p2.z() && p1.a() == p2.a() && abs(p1.x() - p2.x()) == abs(p1.y() - p2.y()))
  {
    good = true;
    num_points = abs(p1.y() - p2.y()) + 1;
  }
  // X/Z Diagonals
  else if (p1.y() == p2.y() && p1.a() == p2.a() && abs(p1.x() - p2.x()) == abs(p1.z() - p2.z()))
  {
    good = true;
    num_points = abs(p1.z() - p2.z()) + 1;
  }
  // X/A Diagonals
  else if (p1.y() == p2.y() && p1.z() == p2.z() && abs(p1.x() - p2.x()) == abs(p1.a() - p2.a()))
  {
    good = true;
    num_points = abs(p1.a() - p2.a()) + 1;
  }
  // Y/Z Diagonals
  else if (p1.x() == p2.x() && p1.a() == p2.a() && abs(p1.y() - p2.y()) == abs(p1.z() - p2.z()))
  {
    good = true;
    num_points = abs(p1.z() - p2.z()) + 1;
  }
  // Y/A Diagonals
  else if (p1.x() == p2.x() && p1.z() == p2.z() && abs(p1.y() - p2.y()) == abs(p1.a() - p2.a()))
  {
    good = true;
    num_points = abs(p1.a() - p2.a()) + 1;
  }
  // Z/A Diagonals
  else if (p1.x() == p2.x() && p1.y() == p2.y() && abs(p1.z() - p2.z()) == abs(p1.a() - p2.a()))
  {
    good = true;
    num_points = abs(p1.a() - p2.a()) + 1;
  }
  // X/Y/Z Diagonals
  else if (p1.a() == p2.a() && abs(p1.x() - p2.x()) == abs(p1.y() - p2.y()) && abs(p1.x() - p2.x()) == abs(p1.z() - p2.z()))
  {
    good = true;
    num_points = abs(p1.z() - p2.z()) + 1;
  }
  // X/Y/A Diagonals
  else if (p1.z() == p2.z() && abs(p1.x() - p2.x()) == abs(p1.y() - p2.y()) && abs(p1.x() - p2.x()) == abs(p1.a() - p2.a()))
  {
    good = true;
    num_points = abs(p1.a() - p2.a()) + 1;
  }
  // X/Z/A Diagonals
  else if (p1.y() == p2.y() && abs(p1.x() - p2.x()) == abs(p1.z() - p2.z()) && abs(p1.x() - p2.x()) == abs(p1.a() - p2.a()))
  {
    good = true;
    num_points = abs(p1.a() - p2.a()) + 1;
  }
  // Y/Z/A Diagonals
  else if (p1.x() == p2.x() && abs(p1.y() - p2.y()) == abs(p1.z() - p2.z()) && abs(p1.y() - p2.y()) == abs(p1.a() - p2.a()))
  {
    good = true;
    num_points = abs(p1.a() - p2.a()) + 1;
  }
  // All Diagonals
  else if (abs(p1.x() - p2.x()) == abs(p1.y() - p2.y()) && abs(p1.x() - p2.x()) == abs(p1.z() - p2.z()) && abs(p1.x() - p2.x()) == abs(p1.a() - p2.a()))
  {
    good = true;
    num_points = abs(p1.a() - p2.a()) + 1;
  }

  if (!good) return positions;

  positions.resize(num_points);

  long long x = p1.x();
  long long y = p1.y();
  long long z = p1.z();
  long long a = p1.a();

  for (long long index = 0; index < num_points; ++index)
  {
    positions[index] = Position(x, y, z, a);

    if (x < p2.x())
    {
      ++x;
    }
    else if (x > p2.x())
    {
      --x;
    }

    if (y < p2.y())
    {
      ++y;
    }
    else if (y > p2.y())
    {
      --y;
    }

    if (z < p2.z())
    {
      ++z;
    }
    else if (z > p2.z())
    {
      --z;
    }

    if (a < p2.a())
    {
      ++a;
    }
    else if (a > p2.a())
    {
      --a;
    }
  }

  return positions;
}
