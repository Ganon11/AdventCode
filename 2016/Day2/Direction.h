#pragma once

#include <vector>

enum Direction {
   DIRECTION_NONE = 0,
   DIRECTION_UP = 1,
   DIRECTION_DOWN = 2,
   DIRECTION_LEFT = 3,
   DIRECTION_RIGHT = 4
};

typedef std::vector<Direction> DirectionList;
typedef std::vector<DirectionList> InstructionList;

Direction GetDirection(const wchar_t c);
