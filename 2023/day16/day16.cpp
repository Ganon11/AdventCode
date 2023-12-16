#include <iostream>
#include <iterator>
#include <map>
#include <queue>
#include <set>
#include <unordered_set>
#include <utility>
#include <vector>

#include "cxxopts.hpp"
#include "input_handler.h"
#include "position.h"

using advent_of_code::Position;

namespace
{
enum Direction
{
  NORTH,
  WEST,
  EAST,
  SOUTH
};

char convert(const Direction direction)
{
  switch (direction)
  {
  case NORTH:
    return 'N';
  case WEST:
    return 'W';
  case EAST:
    return 'E';
  case SOUTH:
    return 'S';
  }
}

struct Ray
{
  Ray()
  : origin{ advent_of_code::ORIGIN },
    direction { EAST }
  { }

  Ray(const Position& p, const Direction d)
  : origin{ p },
    direction{ d }
    { }

  bool operator==(const Ray& other) const
  {
    return origin == other.origin && direction == other.direction;
  }

  struct HashFunction
  {
    size_t operator()(const Ray& r) const
    {
      size_t positionHash = std::hash<Position>{}(r.origin);
      size_t directionHash = std::hash<Direction>{}(r.direction);
      return positionHash ^ directionHash;
    }
  };

  Position origin;
  Direction direction;
};

enum GridType
{
  SPACE,
  FORWARD_MIRROR,
  BACKWARD_MIRROR,
  VERTICAL_SPLITTER,
  HORIZONTAL_SPLITTER
};

char convert(const GridType type)
{
  switch (type)
  {
  case SPACE:
    return '.';
  case FORWARD_MIRROR:
    return '/';
  case BACKWARD_MIRROR:
    return '\\';
  case VERTICAL_SPLITTER:
    return '|';
  case HORIZONTAL_SPLITTER:
    return '-';
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

typedef std::map<Position, GridType> Grid;

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

bool in_bounds(const Position& p, const long long min_row, const long long min_col, const long long max_row, const long long max_col)
{
  if (p.x() < min_col || p.x() > max_col)
  {
    return false;
  }

  if (p.y() < min_row || p.y() > max_row)
  {
    return false;
  }

  return true;
}

Position find_terminating_position(const Grid& grid, const Ray& ray, bool& went_out_of_bounds)
{
  went_out_of_bounds = false;
  Position current{ ray.origin };
  std::set<Position> all_positions;
  std::transform(grid.begin(), grid.end(), std::inserter(all_positions, all_positions.begin()), [](const auto& kvp){ return kvp.first; });

  if (all_positions.end() != std::find(all_positions.begin(), all_positions.end(), current))
  {
    return current;
  }

  long long min_row, min_col, max_row, max_col;
  Position::get_boundaries(all_positions, min_row, min_col, max_row, max_col);
  min_row = 0;
  min_col = 0;

  //std::cout << "Grid ranges from row " << min_row << " to max row " << max_row << std::endl;
  //std::cout << "Grid ranges from col " << min_col << " to max col " << max_col << std::endl;

  while (1)
  {
    Position next;
    switch (ray.direction)
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

    //std::cout << "Checking next position " << next << std::endl;
    if (!in_bounds(next, min_row, min_col, max_row, max_col))
    {
      //std::cout << "\tOut of bounds!" << std::endl;
      went_out_of_bounds = true;
      break;
    }

    //std::cout << "Next position is in bounds, checking grid" << std::endl;
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
      if (ray.direction == EAST || ray.direction == WEST)
      {
        keep_going = true;
      }
      break;
    case VERTICAL_SPLITTER:
      if (ray.direction == NORTH || ray.direction == SOUTH)
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

std::vector<Ray> get_resultant_rays(const auto& ray, const Position& position, const GridType type)
{
  std::vector<Ray> next_rays;
  switch (type)
  {
  case FORWARD_MIRROR:
    switch (ray.direction)
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
    switch (ray.direction)
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
    switch (ray.direction)
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
    switch (ray.direction)
    {
    case EAST:
      next_rays.push_back(Ray{ position.east(), ray.direction });
      break;
    case WEST:
      next_rays.push_back(Ray{ position.west(), ray.direction });
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

size_t evaluate_rays(const Grid& grid, const Ray& initial_ray = Ray{ advent_of_code::ORIGIN, EAST })
{
  std::set<Position> all_positions;
  std::transform(grid.begin(), grid.end(), std::inserter(all_positions, all_positions.begin()), [](const auto& kvp){ return kvp.first; });
  long long min_row, min_col, max_row, max_col;
  Position::get_boundaries(all_positions, min_row, min_col, max_row, max_col);
  min_row = 0;
  min_col = 0;

  std::map<Position, std::set<Direction>> visited;
  std::queue<Ray> rays;
  rays.push(initial_ray);
  visited[initial_ray.origin].insert(initial_ray.direction);

  while (!rays.empty())
  {
    Ray ray{ rays.front() };
    rays.pop();
    bool went_out_of_bounds;
    Position terminal{ find_terminating_position(grid, ray, went_out_of_bounds) };
    std::vector<Position> positions{ advent_of_code::Position::getPositionsInLine(ray.origin, terminal) };
    for (const auto& p : positions)
    {
      visited[p].insert(ray.direction);
    }

    if (went_out_of_bounds)
    {
      //std::cout << "\tRay has exited the grid" << std::endl;
      continue;
    }

    for (const auto& neighbor : get_resultant_rays(ray, terminal, grid.at(terminal)))
    {
      if (!in_bounds(neighbor.origin, min_row, min_col, max_row, max_col))
      {
        continue;
      }

      if (visited.contains(neighbor.origin) && visited.at(neighbor.origin).contains(neighbor.direction))
      {
        continue;
      }

      rays.push(neighbor);
    }
  }

  // for (const auto& kvp : visited)
  // {
  //   std::cout << kvp.first << std::endl;
  // }
  return visited.size();
}
}

int main(int argc, char* argv[])
{
  cxxopts::Options options("d16", "Day 16 of Advent of Code");
  options.add_options()
    ("f,filename", "Input Filename", cxxopts::value<std::string>())
    ("h,help", "Print usage")
  ;

  auto result = options.parse(argc, argv);
  if (result.count("help"))
  {
    std::cout << options.help() << std::endl;
    return 0;
  }

  if (!result.count("filename"))
  {
    std::cout << "ERROR: Missing required argument: <filename>" << std::endl;
    std::cout << options.help() << std::endl;
    return -1;
  }

  advent_of_code::InputHandler input{ result["filename"].as<std::string>() };
  std::vector<std::string> lines{ input.read_all_lines() };
  Grid grid{ parse_grid(lines) };
  size_t visited{ evaluate_rays(grid) };

  std::cout << "Energized " << visited << " tiles." << std::endl;

  std::set<Position> all_positions;
  std::transform(grid.begin(), grid.end(), std::inserter(all_positions, all_positions.begin()), [](const auto& kvp){ return kvp.first; });
  long long min_row, min_col, max_row, max_col;
  Position::get_boundaries(all_positions, min_row, min_col, max_row, max_col);
  min_row = 0;
  min_col = 0;

  size_t max{0};
  for (long long row = min_row; row <= max_row; ++row)
  {
    Ray from_left{ Position{ min_col, row }, EAST };
    visited = evaluate_rays(grid, from_left);
    if (visited > max)
    {
      max = visited;
    }

    Ray from_right{ Position{ max_col, row}, WEST };
    visited = evaluate_rays(grid, from_right);
    if (visited > max)
    {
      max = visited;
    }
  }

  for (long long col = min_col; col <= max_col; ++col)
  {
    Ray from_top{ Position{ col, min_row }, SOUTH };
    visited = evaluate_rays(grid, from_top);
    if (visited > max)
    {
      max = visited;
    }

    Ray from_bottom{ Position{ col, max_row }, NORTH };
    visited = evaluate_rays(grid, from_bottom);
    if (visited > max)
    {
      max = visited;
    }
  }

  std::cout << "Optimal energy: " << max << " tiles." << std::endl;

  return 0;
}
