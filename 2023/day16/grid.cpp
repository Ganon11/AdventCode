#include "grid.h"

using advent_of_code::Position;

char convert(const GridType type)
{
  switch (type)
  {
  case FORWARD_MIRROR:
    return '/';
  case BACKWARD_MIRROR:
    return '\\';
  case VERTICAL_SPLITTER:
    return '|';
  case HORIZONTAL_SPLITTER:
    return '-';
  case SPACE:
  default:
    return '.';
  }
}

GridType convert(const char ch)
{
  switch (ch)
  {
  case '/':
    return FORWARD_MIRROR;
  case '\\':
    return BACKWARD_MIRROR;
  case '|':
    return VERTICAL_SPLITTER;
  case '-':
    return HORIZONTAL_SPLITTER;
  case '.':
  default:
    return SPACE;
  }
}

Grid parse_grid(const std::vector<std::string>& lines)
{
  Grid grid;
  for (unsigned long long row = 0; row < lines.size(); ++row)
  {
    for (unsigned long long col = 0; col < lines[row].size(); ++col)
    {
      GridType type{ convert(lines[row][col] )};
      if (type != SPACE)
      {
        grid.insert(Grid::value_type{ Position{ col, row }, type });
      }
    }
  }

  return grid;
}

Position find_terminating_position(const Grid& grid, const Ray& ray, bool& went_out_of_bounds)
{
  went_out_of_bounds = false;
  Position current{ ray.origin() };
  std::set<Position> all_positions;
  std::transform(grid.begin(), grid.end(), std::inserter(all_positions, all_positions.begin()), [](const auto& kvp){ return kvp.first; });
  long long min_row, min_col, max_row, max_col;
  Position::get_boundaries(all_positions, min_row, min_col, max_row, max_col);
  min_row = 0;
  min_col = 0;

  if (all_positions.end() != std::find(all_positions.begin(), all_positions.end(), current))
  {
    return current;
  }

  while (1)
  {
    Position next;
    switch (ray.direction())
    {
    case NORTH:
      next = current.north();
      break;
    case WEST:
      next = current.west();
      break;
    case EAST:
      next = current.east();
      break;
    case SOUTH:
      next = current.south();
      break;
    }

    if (!Position::in_bounds(min_row, min_col, max_row, max_col, next))
    {
      went_out_of_bounds = true;
      break;
    }

    current = next;
    auto itr{ std::find(all_positions.begin(), all_positions.end(), current) };
    if (all_positions.end() == itr)
    {
      continue;
    }

    GridType type{ grid.at(current) };
    bool keep_going = false;
    switch (type)
    {
    case HORIZONTAL_SPLITTER:
      if (ray.direction() == EAST || ray.direction() == WEST)
      {
        keep_going = true;
      }
      break;
    case VERTICAL_SPLITTER:
      if (ray.direction() == NORTH || ray.direction() == SOUTH)
      {
        keep_going = true;
      }
      break;
    default:
      break;
    }

    if (!keep_going)
    {
      break;
    }
  }

  return current;
}

std::vector<Ray> get_resultant_rays(const Ray& ray, const Position& position, const GridType type)
{
  std::vector<Ray> next_rays;
  switch (type)
  {
  case FORWARD_MIRROR:
    switch (ray.direction())
    {
    case NORTH:
      next_rays.push_back(Ray{ position.east(), EAST });
      break;
    case EAST:
      next_rays.push_back(Ray{ position.north(), NORTH });
      break;
    case WEST:
      next_rays.push_back(Ray{ position.south(), SOUTH });
      break;
    case SOUTH:
      next_rays.push_back(Ray{ position.west(), WEST });
      break;
    }
    break;
  case BACKWARD_MIRROR:
    switch (ray.direction())
    {
    case NORTH:
      next_rays.push_back(Ray{ position.west(), WEST });
      break;
    case EAST:
      next_rays.push_back(Ray{ position.south(), SOUTH });
      break;
    case WEST:
      next_rays.push_back(Ray{ position.north(), NORTH });
      break;
    case SOUTH:
      next_rays.push_back(Ray{ position.east(), EAST });
      break;
    }
    break;
  case VERTICAL_SPLITTER:
    switch (ray.direction())
    {
    case EAST:
    case WEST:
      next_rays.push_back(Ray{ position.south(), SOUTH });
      next_rays.push_back(Ray{ position.north(), NORTH });
      break;
    case SOUTH:
      next_rays.push_back(Ray{ position.south(), SOUTH });
      break;
    case NORTH:
      next_rays.push_back(Ray{ position.north(), NORTH });
      break;
    }
    break;
  case HORIZONTAL_SPLITTER:
    switch (ray.direction())
    {
    case EAST:
      next_rays.push_back(Ray{ position.east(), EAST });
      break;
    case WEST:
      next_rays.push_back(Ray{ position.west(), WEST });
      break;
    case SOUTH:
    case NORTH:
      next_rays.push_back(Ray{ position.east(), EAST });
      next_rays.push_back(Ray{ position.west(), WEST });
      break;
    }
    break;
  case SPACE:
  default:
    break;
  }

  return next_rays;
}
