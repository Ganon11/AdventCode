#pragma once

struct Point {
   unsigned int x, y;

   Point(const unsigned int a, const unsigned int b);

   bool IsOpen() const;
   bool operator<(const Point& rhs) const;
   bool operator==(const Point& rhs) const;
};
