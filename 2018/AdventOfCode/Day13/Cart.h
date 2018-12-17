#pragma once

#include "../AoCHelpers/Position.h"

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
  Cart(const unsigned int x,
       const unsigned int y,
       const Direction direction);
  Cart(const advent_of_code::Position& p, const Direction direction);

  Direction get_current_direction() const;
  advent_of_code::Position get_current_position() const;
  unsigned int get_id() const;

  Direction turn();
  Direction turn_clockwise();
  Direction turn_counterclockwise();
  advent_of_code::Position move();

  bool operator<(const Cart& other) const;
  bool operator==(const Cart& other) const;
  bool operator!=(const Cart& other) const;

private:
  advent_of_code::Position m_position;
  Direction m_direction;

  enum Turn
  {
    LEFT,
    STRAIGHT,
    RIGHT
  };

  Turn m_next_turn;
  static unsigned int s_next_id;
  unsigned int m_id;
};
