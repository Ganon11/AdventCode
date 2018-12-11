#include "pch.h"
#include "Grid.h"

#include <sstream>

Coordinate::Coordinate(const unsigned int x, const unsigned int y, const unsigned int square_size)
   : m_x{ x }, m_y{ y }, m_square_size{ square_size }
{ }

Grid::Grid(const unsigned int grid_serial_number) {
   m_power_grid = std::vector<std::vector<int>>(GRID_SIZE);
   for (size_t index = 0; index < m_power_grid.size(); ++index) {
      m_power_grid[index] = std::vector<int>(GRID_SIZE, std::numeric_limits<int>::min());
   }

   for (unsigned int y = 0; y < m_power_grid.size(); ++y) {
      for (unsigned int x = 0; x < m_power_grid[y].size(); ++x) {
         Coordinate c{ x, y };
         m_power_grid[y][x] = calculate_power(c, grid_serial_number);
      }
   }
}

Coordinate Grid::get_max_three_by_three() const {
   Coordinate max_coordinate{ 0, 0 };
   int max_power_level{ std::numeric_limits<int>::min() };

   for (unsigned int y = 0; y < m_power_grid.size() - 2; ++y) {
      for (unsigned int x = 0; x < m_power_grid[y].size() - 2; ++x) {
         Coordinate c{ x, y };
         int power_level{ calculate_sum(c) };

         if (power_level > max_power_level)
         {
            max_coordinate = c;
            max_power_level = power_level;
         }
      }
   }

   return max_coordinate;
}

Coordinate Grid::get_max_square() const {
   Coordinate max_coordinate{ 0, 0, 1 };
   int max_power_level{ std::numeric_limits<int>::min() };

   for (unsigned int square_size = 1; square_size <= 300; ++square_size) {
      for (unsigned int y = 0; y < (m_power_grid.size() - square_size + 1); ++y) {
         for (unsigned int x = 0; x < (m_power_grid[y].size() - square_size + 1); ++x) {
            Coordinate c{ x, y, square_size };
            int power_level{ calculate_sum(c) };

            if (power_level > max_power_level)
            {
               max_coordinate = c;
               max_power_level = power_level;
            }
         }
      }
   }

   return max_coordinate;
}

int Grid::calculate_power(const Coordinate& c, const unsigned int grid_serial_number) {
   // Find the fuel cell's rack ID, which is its X coordinate plus 10.
   const int rack_id{ static_cast<int>(c.m_x) + 10 };

   // Begin with a power level of the rack ID times the Y coordinate.
   int power_level{ static_cast<int>(rack_id * c.m_y) };

   // Increase the power level by the value of the grid serial number
   power_level += grid_serial_number;

   // Set the power level to itself multiplied by the rack ID.
   power_level *= rack_id;

   // Keep only the hundreds digit of the power level (so 12345 becomes 3; numbers with no hundreds digit become 0).
   if (power_level > 100 || power_level < -100) {
      std::wstringstream ss;
      ss << power_level;
      const std::wstring power_level_string{ ss.str() };
      const std::wstring hundreds_digit{ power_level_string[power_level_string.length() - 3] };
      power_level = _wtoi(hundreds_digit.c_str());
   } else {
      power_level = 0;
   }
   
   power_level -= 5;

   return power_level;
}

int Grid::calculate_sum(const Coordinate& c) const {
   if ((c.m_x + c.m_square_size - 1) >= GRID_SIZE || (c.m_y + c.m_square_size - 1) >= GRID_SIZE) {
      return std::numeric_limits<int>::min();
   }

   int sum{ 0 };
   for (unsigned int y = c.m_y; y < (c.m_y + c.m_square_size); ++y) {
      for (unsigned int x = c.m_x; x < (c.m_x + c.m_square_size); ++x) {
         sum += m_power_grid[y][x];
      }
   }

   return sum;
}
