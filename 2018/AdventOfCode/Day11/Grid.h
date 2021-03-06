#pragma once

#include <algorithm>
#include <vector>
#include <utility>

typedef std::vector<std::vector<int>> TwoDimensionalIntVector;

struct Coordinate
{
  Coordinate(const unsigned int x, const unsigned int y, const unsigned int square_size = 3);

  unsigned int m_x;
  unsigned int m_y;
  unsigned int m_square_size;
};

class Grid
{
public:
  Grid(const unsigned int grid_serial_number);

  Coordinate get_max_three_by_three() const;
  Coordinate get_max_square() const;

private:
  TwoDimensionalIntVector m_power_grid;
  TwoDimensionalIntVector m_partial_sums;

  static constexpr unsigned int GRID_SIZE{ 300 };
  static int calculate_power(const Coordinate& c, const unsigned int grid_serial_number);
  int get_max_square(Coordinate& max_coordinate) const;
};
