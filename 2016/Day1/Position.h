#pragma once

#include "Orientation.h"

class Position {
public:
   Position(int x = 0, int y = 0);
   static Position Move(const Position& start, const int blocks, const Orientation orientation);

   int getX() const;
   int getY() const;

private:
   int m_x;
   int m_y;
};
