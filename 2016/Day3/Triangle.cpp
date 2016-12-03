#include "stdafx.h"
#include "Triangle.h"
#include <vector>

Triangle::Triangle(int a, int b, int c) : m_a{ a }, m_b{ b }, m_c{ c }, m_circumference{ a + b + c } {
}

bool Triangle::IsValid() const {
   std::vector<int> sides{ m_a, m_b, m_c };
   for (const int side : sides) {
      int partial = m_circumference - side;
      if (partial <= side) {
         return false;
      }
   }

   return true;
}
