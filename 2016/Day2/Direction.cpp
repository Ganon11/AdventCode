#include "stdafx.h"
#include "Direction.h"

Direction GetDirection(const wchar_t c) {
   switch (c) {
   case L'U':
   case L'u':
      return DIRECTION_UP;
   case L'D':
   case L'd':
      return DIRECTION_DOWN;
   case L'L':
   case L'l':
      return DIRECTION_LEFT;
   case L'R':
   case L'r':
      return DIRECTION_RIGHT;
   default:
      return DIRECTION_NONE;
   }
}