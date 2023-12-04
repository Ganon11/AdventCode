#pragma once

#include "position.h"

enum POSITION_TYPE {
  SPACE = 0,
  NUMBER = 1,
  SYMBOL = 2
};

struct SchematicSquare
{
public:
  SchematicSquare() = default;
  SchematicSquare(advent_of_code::Position p, char ch);

  advent_of_code::Position position;
  char character;
  POSITION_TYPE type;
};
