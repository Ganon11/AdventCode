#include "position.h"

#include <vector>

#include "input_handler.h"

namespace advent_of_code
{
Position::Position(const unsigned short x, const unsigned short y, const unsigned short z, const unsigned short a)
  : m_x{ static_cast<long long>(x) },
    m_y{ static_cast<long long>(y) },
    m_z{ static_cast<long long>(z) },
    m_a{ static_cast<long long>(a) }
{}

Position::Position(const short x, const short y, const short z, const short a)
  : m_x{ static_cast<long long>(x) },
    m_y{ static_cast<long long>(y) },
    m_z{ static_cast<long long>(z) },
    m_a{ static_cast<long long>(a) }
{}

Position::Position(const unsigned int x, const unsigned int y, const unsigned int z, const unsigned int a)
  : m_x{ static_cast<long long>(x) },
    m_y{ static_cast<long long>(y) },
    m_z{ static_cast<long long>(z) },
    m_a{ static_cast<long long>(a) }
{}

Position::Position(const int x, const int y, const int z, const int a)
  : m_x{ static_cast<long long>(x) },
    m_y{ static_cast<long long>(y) },
    m_z{ static_cast<long long>(z) },
    m_a{ static_cast<long long>(a) }
{}

Position::Position(const unsigned long x, const unsigned long y, const unsigned long z, const unsigned long a)
  : m_x{ static_cast<long long>(x) },
    m_y{ static_cast<long long>(y) },
    m_z{ static_cast<long long>(z) },
    m_a{ static_cast<long long>(a) }
{}

Position::Position(const long x, const long y, const long z, const long a)
  : m_x{ static_cast<long long>(x) },
    m_y{ static_cast<long long>(y) },
    m_z{ static_cast<long long>(z) },
    m_a{ static_cast<long long>(a) }
{}

Position::Position(const unsigned long long x, const unsigned long long y, const unsigned long long z, const unsigned long long a)
  : m_x{ static_cast<long long>(x) },
    m_y{ static_cast<long long>(y) },
    m_z{ static_cast<long long>(z) },
    m_a{ static_cast<long long>(a) }
{}

Position::Position(const long long x, const long long y, const long long z, const long long a)
  : m_x{ x },
    m_y{ y },
    m_z{ z },
    m_a{ a }
{}

Position::Position(const std::string& line)
{
  std::vector<std::string> tokens{ tokenize(line, ',') };

  size_t token_count = tokens.size();
  if (token_count >= 1)
  {
    m_x = std::stoll(tokens[0]);
  }

  if (token_count >= 2)
  {
    m_y = std::stoll(tokens[1]);
  }

  if (token_count >= 3)
  {
    m_z = std::stoll(tokens[2]);
  }

  if (token_count >= 4)
  {
    m_a = std::stoll(tokens[3]);
  }
}

bool Position::operator<(const Position& other) const
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

bool Position::operator>(const Position& other) const
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

bool Position::operator==(const Position& other) const
{
  return m_x == other.m_x && m_y == other.m_y && m_z == other.m_z && m_a == other.m_a;
}

bool Position::operator!=(const Position& other) const
{
  return !operator==(other);
}

bool Position::is_adjacent_to(const Position& other) const
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

Position Position::north() const
{
  return Position{ m_x, m_y - 1, m_z, m_a };
}

Position Position::south() const
{
  return Position{ m_x, m_y + 1, m_z, m_a };
}

Position Position::east() const
{
  return Position{ m_x + 1, m_y, m_z, m_a };
}

Position Position::west() const
{
  return Position{ m_x - 1, m_y, m_z, m_a };
}

Position Position::above() const
{
  return Position{ m_x, m_y, m_z - 1, m_a };
}

Position Position::below() const
{
  return Position{ m_x, m_y, m_z + 1, m_a };
}

Position Position::past() const
{
  return Position{ m_x, m_y, m_z, m_a - 1 };
}

Position Position::future() const
{
  return Position{ m_x, m_y, m_z, m_a + 1 };
}

std::vector<Position> Position::get_adjacent_positions(const bool diagonal) const
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

std::vector<Position> Position::get_adjacent_positions_3D() const
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

std::vector<Position> Position::get_adjacent_positions_4D() const
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

size_t Position::distance_to(const Position& other) const
{
  return static_cast<size_t>(llabs(m_x - other.m_x)
                           + llabs(m_y - other.m_y)
                           + llabs(m_z - other.m_z)
                           + llabs(m_a - other.m_a));
}

std::ostream& operator<<(std::ostream& out, const Position& p)
{
  out << p.x() << ", " << p.y() << ", " << p.z() << ", " << p.a();
  return out;
}

std::vector<Position> Position::getPositionsInLine(const Position& p1, const Position& p2)
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

void Position::get_boundaries(const std::set<Position>& positions, long long& min_row, long long& min_col, long long& max_row, long long& max_col)
{
  min_row = std::min_element(positions.begin(), positions.end())->y();
  min_col = std::min_element(positions.begin(), positions.end())->x();
  max_row = std::max_element(positions.begin(), positions.end())->y();
  max_col = std::max_element(positions.begin(), positions.end())->x();
}
} // namespace advent_of_code
