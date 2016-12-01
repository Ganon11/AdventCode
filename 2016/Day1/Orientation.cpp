#include "stdafx.h"
#include "Orientation.h"

Rotation GetRotation(wchar_t rotation) {
   switch (rotation) {
   case 'R':
   case 'r':
      return RIGHT;
   case 'L':
   case 'l':
      return LEFT;
   default:
      return RIGHT;
   }
}

Orientation Rotate(Orientation o, Rotation r) {
   switch (o) {
   case NORTH:
      switch (r) {
      case RIGHT:
         return EAST;
      case LEFT:
         return WEST;
      }
   case EAST:
      switch (r) {
      case RIGHT:
         return SOUTH;
      case LEFT:
         return NORTH;
      }
   case WEST:
      switch (r) {
      case RIGHT:
         return NORTH;
      case LEFT:
         return SOUTH;
      }
   case SOUTH:
      switch (r) {
      case RIGHT:
         return WEST;
      case LEFT:
         return EAST;
      }
   }

   // Literally can't happen because enums, but whatever.
   return NORTH;
}