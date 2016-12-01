#pragma once

enum Orientation {
   NORTH = 0,
   EAST = 1,
   WEST = 2,
   SOUTH = 3
};

enum Rotation {
   RIGHT = 0,
   LEFT = 1
};

Rotation GetRotation(wchar_t rotation);

Orientation Rotate(Orientation o, Rotation r);
