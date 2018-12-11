#include "pch.h"
#include "Grid.h"

#include <sstream>

Coordinate::Coordinate(const unsigned int x, const unsigned int y, const unsigned int square_size)
  : m_x{ x }, m_y{ y }, m_square_size{ square_size }
{ }

Grid::Grid(const unsigned int grid_serial_number) {
  m_power_grid = TwoDimensionalIntVector(GRID_SIZE);
  m_partial_sums = TwoDimensionalIntVector(GRID_SIZE);
  for (size_t index = 0; index < m_power_grid.size(); ++index) {
    m_power_grid[index] = std::vector<int>(GRID_SIZE);
    m_partial_sums[index] = std::vector<int>(GRID_SIZE);
  }

  for (unsigned int y = 0; y < m_power_grid.size(); ++y) {
    for (unsigned int x = 0; x < m_power_grid[y].size(); ++x) {
      Coordinate c{ x, y };
      int power_level{ calculate_power(c, grid_serial_number) };

      m_power_grid[y][x] = power_level;
      int partial_sum = power_level;
      if (y > 0) {
        partial_sum += m_partial_sums[x][y - 1];
      }

      if (x > 0) {
        partial_sum += m_partial_sums[x - 1][y];
      }

      if (x > 0 && y > 0) {
        partial_sum -= m_partial_sums[x - 1][y - 1];
      }

      m_partial_sums[x][y] = partial_sum;
    }
  }
}

Coordinate Grid::get_max_three_by_three() const {
  Coordinate c{ 0, 0, 3 };
  get_max_square(c);
  return c;
}

Coordinate Grid::get_max_square() const {
  Coordinate max_coordinate{ 0, 0, 1 };
  int max_power_level{ std::numeric_limits<int>::min() };

  for (unsigned int square_size = 1; square_size <= 300; ++square_size) {
    Coordinate c{ 0, 0, square_size };
    int power_level{ get_max_square(c) };
    if (power_level > max_power_level) {
      max_coordinate = c;
      max_power_level = power_level;
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

  // Keep only the hundreds digit of the power level (so 12345 becomes 3; numbers with no hundreds
  // digit become 0).
  power_level = (power_level / 100) % 10;

  // Subtract 5 from the power level.
  power_level -= 5;

  return power_level;
}

int Grid::get_max_square(Coordinate& max_coordinate) const {
  unsigned int square_size{ max_coordinate.m_square_size };
  int max_power_level{ std::numeric_limits<int>::min() };
  const unsigned int num_squares = (GRID_SIZE - square_size + 1);

  for (unsigned int y = square_size; y < GRID_SIZE; ++y) {
    for (unsigned int x = square_size; x < GRID_SIZE; ++x) {
      int partial_sum_a{ m_partial_sums[x][y] };
      int partial_sum_b{ m_partial_sums[x - square_size][y] };
      int partial_sum_c{ m_partial_sums[x][y - square_size] };
      int partial_sum_d{ m_partial_sums[x - square_size][y - square_size] };

      int power_level{ partial_sum_d + partial_sum_a - partial_sum_b - partial_sum_c };
      if (power_level > max_power_level) {
        max_coordinate = Coordinate{ x - square_size + 1, y - square_size + 1, square_size };
        max_power_level = power_level;
      }
    }
  }

  return max_power_level;
}
