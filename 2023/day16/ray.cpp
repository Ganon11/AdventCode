#include "ray.h"

Ray::Ray()
: m_origin{ advent_of_code::ORIGIN },
  m_direction{ EAST }
{ }

Ray::Ray(const advent_of_code::Position& p, const Direction d)
: m_origin{ p },
  m_direction{ d }
{ }

bool Ray::operator==(const Ray& other) const
{
  return m_origin == other.m_origin && m_direction == other.m_direction;
}

advent_of_code::Position Ray::origin() const
{
  return m_origin;
}

Direction Ray::direction() const
{
  return m_direction;
}

char convert(const Direction direction)
{
  switch (direction)
  {
  case NORTH:
    return 'N';
  case WEST:
    return 'W';
  case EAST:
    return 'E';
  case SOUTH:
    return 'S';
  default:
    return '?';
  }
}
