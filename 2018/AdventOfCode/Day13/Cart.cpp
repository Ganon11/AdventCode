#include "pch.h"
#include "Cart.h"

Cart::Cart(const unsigned int x,
           const unsigned int y,
           const Direction direction)
    : m_position{ x, y }, m_direction{ direction }, m_next_turn{ LEFT }, m_id { s_next_id++ }
{}

Cart::Cart(const Position& p, const Direction direction)
    : m_position{ p }, m_direction{ direction }, m_next_turn{ LEFT }, m_id{ s_next_id++ }
{}

Direction Cart::get_current_direction() const {
  return m_direction;
}

Position Cart::get_current_position() const {
  return m_position;
}

unsigned int Cart::get_id() const {
  return m_id;
}

Direction Cart::turn() {
  switch (m_next_turn) {
  case LEFT:
    turn_counterclockwise();
    m_next_turn = STRAIGHT;
    break;
  case STRAIGHT:
    m_next_turn = RIGHT;
    break;
  case RIGHT:
    turn_clockwise();
    m_next_turn = LEFT;
    break;
  }

  return m_direction;
}

Direction Cart::turn_clockwise() {
  switch (m_direction) {
  case NORTH:
    m_direction = EAST;
    break;
  case EAST:
    m_direction = SOUTH;
    break;
  case SOUTH:
    m_direction = WEST;
    break;
  case WEST:
  default:
    m_direction = NORTH;
    break;
  }

  return m_direction;
}

Direction Cart::turn_counterclockwise() {
  switch (m_direction) {
  case NORTH:
    m_direction = WEST;
    break;
  case EAST:
    m_direction = NORTH;
    break;
  case SOUTH:
    m_direction = EAST;
    break;
  case WEST:
  default:
    m_direction = SOUTH;
    break;
  }

  return m_direction;
}

Position Cart::move() {
  switch (m_direction) {
  case NORTH:
    m_position = Position(m_position.m_x, m_position.m_y - 1);
    break;
  case EAST:
    m_position = Position(m_position.m_x + 1, m_position.m_y);
    break;
  case SOUTH:
    m_position = Position(m_position.m_x, m_position.m_y + 1);
    break;
  case WEST:
  default:
    m_position = Position(m_position.m_x - 1, m_position.m_y);
    break;
  }

  return m_position;
}

bool Cart::operator<(const Cart& other) const {
  return m_position < other.get_current_position();
}

bool Cart::operator==(const Cart& other) const {
  return m_id == other.m_id;
}

bool Cart::operator!=(const Cart& other) const {
  return !operator==(other);
}

unsigned int Cart::s_next_id = 1;
