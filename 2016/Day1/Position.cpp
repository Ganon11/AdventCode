#include "stdafx.h"
#include "Position.h"
#include <cmath>

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

std::vector<Position> Position::MoveAndReturnIntermediatePositions(const Position& start, const int blocks, const Orientation orientation) {
   std::vector<Position> intermediate;
   switch (orientation) {
      case NORTH: {
         for (int i = 1; i <= blocks; ++i) {
            intermediate.push_back(Position{ start.getX(), start.getY() + i });
         }
         break;
      }
      case EAST: {
         for (int i = 1; i <= blocks; ++i) {
            intermediate.push_back(Position{ start.getX() + i, start.getY() });
         }
         break;
      }
      case WEST: {
         for (int i = 1; i <= blocks; ++i) {
            intermediate.push_back(Position{ start.getX() - i, start.getY() });
         }
         break;
      }
      case SOUTH: {
         for (int i = 1; i <= blocks; ++i) {
            intermediate.push_back(Position{ start.getX(), start.getY() - i });
         }
         break;
      }
   }

   return intermediate;
}

int Position::getX() const {
   return m_x;
}

int Position::getY() const {
   return m_y;
}

bool operator<(const Position& lhs, const Position& rhs) {
   int lx{ lhs.getX() }, ly{ lhs.getY() }, rx{ rhs.getX() }, ry{ rhs.getY() };
   
   if (lx < rx) {
      return true;
   }
   else if (lx == rx && ly < ry) {
      return true;
   }

   return false;
}

bool operator==(const Position& lhs, const Position& rhs) {
   return lhs.getX() == rhs.getX() && lhs.getY() == rhs.getY();
}

int Position::distanceFromOrigin() const {
   return abs(m_x) + abs(m_y);
}
