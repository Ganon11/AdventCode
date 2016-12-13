#include "stdafx.h"
#include "Point.h"

#ifdef _DEBUG
#define FAVORITE 10
#else
#define FAVORITE 1362
#endif

Point::Point(const unsigned int a, const unsigned int b) : x{ a }, y{ b } {}

bool Point::IsOpen() const {
   unsigned int num{ (x * x) + (3 * x) + (2 * x * y) + y + (y * y) + FAVORITE };

   unsigned int total{ 0 };
   while (num) {
      if (num & 1) {
         ++total;
      }
      num >>= 1;
   }

   return (0 == total % 2);
}

bool Point::operator<(const Point& rhs) const {
   if (x < rhs.x) {
      return true;
   }

   if (x == rhs.x && y < rhs.y) {
      return true;
   }

   return false;
}

bool Point::operator==(const Point& rhs) const {
   return (x == rhs.x && y == rhs.y);
}
