#pragma once

#include "position.h"

enum Direction
{
  NORTH,
  WEST,
  EAST,
  SOUTH
};

class Ray
{
public:
  Ray();
  Ray(const advent_of_code::Position& p, const Direction d);

  bool operator==(const Ray& other) const;
  advent_of_code::Position origin() const;
  Direction direction() const;

private:
  advent_of_code::Position m_origin;
  Direction m_direction;
};

char convert(const Direction direction);
