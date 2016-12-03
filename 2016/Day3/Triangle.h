#pragma once

class Triangle {
public:
   Triangle(int a, int b, int c);

   bool IsValid() const;

private:
   int m_a;
   int m_b;
   int m_c;

   int m_circumference;
};
