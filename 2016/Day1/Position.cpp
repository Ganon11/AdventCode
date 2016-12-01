#include "stdafx.h"
#include "Position.h"

Position::Position(int x, int y) : m_x{ x }, m_y{ y } {}

Position Position::Move(const Position& start, const int blocks, const Orientation orientation) {
   switch (orientation) {
   case NORTH:
      return Position{ start.getX(), start.getY() + blocks };
   case EAST:
      return Position{ start.getX() + blocks, start.getY() };
   case WEST:
      return Position{ start.getX() - blocks, start.getY() };
   case SOUTH:
      return Position{ start.getX(), start.getY() - blocks };
   default:
      return start;
   }
}

int Position::getX() const {
   return m_x;
}

int Position::getY() const {
   return m_y;
}
