#pragma once

#include "Position.h"

enum Direction
{
  NORTH,
  EAST,
  SOUTH,
  WEST
};

class Cart
{
public:
  Cart(const unsigned int x, const unsigned int y, const Direction direction);
  Cart(const Position& p, const Direction direction);

  Direction get_current_direction() const;
  Direction turn();
  Direction turn_clockwise();
  Direction turn_counterclockwise();

  Position get_current_position() const;
  Position move();

  bool operator<(const Cart& other) const;

private:
  Position m_position;
  Direction m_direction;

  enum Turn
  {
    LEFT,
    STRAIGHT,
    RIGHT
  };

  Turn m_next_turn;
};
