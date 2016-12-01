#pragma once

#include "Orientation.h"

#include <vector>

class Position {
public:
   Position(int x = 0, int y = 0);
   static Position Move(const Position& start, const int blocks, const Orientation orientation);
   static std::vector<Position> MoveAndReturnIntermediatePositions(const Position& start, const int blocks, const Orientation orientation);

   int getX() const;
   int getY() const;

   friend bool operator<(const Position& lhs, const Position& rhs);
   friend bool operator==(const Position& lhs, const Position& rhs);

   int distanceFromOrigin() const;

private:
   int m_x;
   int m_y;
};
