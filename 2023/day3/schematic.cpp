#include "schematic.h"

SchematicSquare::SchematicSquare(advent_of_code::Position p, char ch)
{
  position = p;
  character = ch;
  if (std::isdigit(ch))
  {
    type = POSITION_TYPE::NUMBER;
  }
  else if (ch == '.')
  {
    type = POSITION_TYPE::SPACE;
  }
  else
  {
    type = POSITION_TYPE::SYMBOL;
  }
}
