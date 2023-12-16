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
      //if (type != SPACE)
      //{
        grid.insert(Grid::value_type{ Position{ col, row }, type });
      //}
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

std::set<Position> evaluate_rays(const Grid& grid, const Ray& initial_ray = Ray{ advent_of_code::ORIGIN, EAST })
{
  std::set<Position> all_positions;
  std::transform(grid.begin(), grid.end(), std::inserter(all_positions, all_positions.begin()), [](const auto& kvp){ return kvp.first; });
  long long min_row, min_col, max_row, max_col;
  Position::get_boundaries(all_positions, min_row, min_col, max_row, max_col);
  min_row = 0;
  min_col = 0;

  std::set<Position> visited;
  std::unordered_set<Ray, Ray::HashFunction> evaluated;
  std::queue<Ray> rays;
  rays.push(initial_ray);

  while (!rays.empty())
  {
    Ray ray{ rays.front() };
    rays.pop();
    if (evaluated.end() != std::find(evaluated.begin(), evaluated.end(), ray))
    {
      //std::cout << "Already evaluated ray at " << ray.origin << ", direction " << convert(ray.direction) << std::endl;
      continue;
    }

    //std::cout << "Evaluating ray at " << ray.origin << ", direction " << convert(ray.direction) << std::endl;
    evaluated.insert(ray);
    Position current{ ray.origin };
    //auto itr{ std::find(all_positions.begin(), all_positions.end(), current) };
    auto itr{ std::find_if(grid.begin(), grid.end(), [&current](const auto& kvp){ return kvp.first == current; }) };
    //while (in_bounds(current, min_row, min_col, max_row, max_col) && itr == all_positions.end() && grid.at(current) == SPACE)
    while (in_bounds(current, min_row, min_col, max_row, max_col) && itr->second == SPACE)
    {
      visited.insert(current);
      switch (ray.direction)
      {
      case NORTH:
        current = current.north();
        break;
      case WEST:
        current = current.west();
        break;
      case EAST:
        current = current.east();
        break;
      case SOUTH:
        current = current.south();
        break;
      }

      //itr = std::find(all_positions.begin(), all_positions.end(), current);
      itr = std::find_if(grid.begin(), grid.end(), [&current](const auto& kvp){ return kvp.first == current; });
    };

    if (!in_bounds(current, min_row, min_col, max_row, max_col))
    {
      //std::cout << "\tRay has exited the grid" << std::endl;
      continue;
    }

    visited.insert(current);
    GridType type{ grid.at(current) };
    //std::cout << "\tRay has terminated at " << current << ", which is a " << convert(type) << std::endl;

    switch (type)
    {
    case FORWARD_MIRROR:
      switch (ray.direction)
      {
      case NORTH:
        rays.push(Ray{ current.east(), EAST });
        break;
      case EAST:
        rays.push(Ray{ current.north(), NORTH });
        break;
      case WEST:
        rays.push(Ray{ current.south(), SOUTH });
        break;
      case SOUTH:
        rays.push(Ray{ current.west(), WEST });
        break;
      }
      break;
    case BACKWARD_MIRROR:
      switch (ray.direction)
      {
      case NORTH:
        rays.push(Ray{ current.west(), WEST });
        break;
      case EAST:
        rays.push(Ray{ current.south(), SOUTH });
        break;
      case WEST:
        rays.push(Ray{ current.north(), NORTH });
        break;
      case SOUTH:
        rays.push(Ray{ current.east(), EAST });
        break;
      }
      break;
    case VERTICAL_SPLITTER:
      switch (ray.direction)
      {
      case EAST:
      case WEST:
        rays.push(Ray{ current.south(), SOUTH });
        rays.push(Ray{ current.north(), NORTH });
        break;
      case SOUTH:
        rays.push(Ray{ current.south(), SOUTH });
        break;
      case NORTH:
        rays.push(Ray{ current.north(), NORTH });
        break;
      }
      break;
    case HORIZONTAL_SPLITTER:
      switch (ray.direction)
      {
      case EAST:
        rays.push(Ray{ current.east(), ray.direction });
        break;
      case WEST:
        rays.push(Ray{ current.west(), ray.direction });
        break;
      case SOUTH:
      case NORTH:
        rays.push(Ray{ current.east(), EAST });
        rays.push(Ray{ current.west(), WEST });
        break;
      }
      break;
    case SPACE:
    default:
      break;
    }
  }

  return visited;
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
  std::set<Position> visited{ evaluate_rays(grid) };

  std::cout << "Energized " << visited.size() << " tiles." << std::endl;

  std::set<Position> all_positions;
  std::transform(grid.begin(), grid.end(), std::inserter(all_positions, all_positions.begin()), [](const auto& kvp){ return kvp.first; });
  long long min_row, min_col, max_row, max_col;
  Position::get_boundaries(all_positions, min_row, min_col, max_row, max_col);
  min_row = 0;
  min_col = 0;

  unsigned long long max{0};
  for (long long row = min_row; row <= max_row; ++row)
  {
    Ray from_left{ Position{ min_col, row }, EAST };
    visited = evaluate_rays(grid, from_left);
    if (visited.size() > max)
    {
      max = visited.size();
    }

    Ray from_right{ Position{ max_col, row}, WEST };
    visited = evaluate_rays(grid, from_right);
    if (visited.size() > max)
    {
      max = visited.size();
    }
  }

  for (long long col = min_col; col <= max_col; ++col)
  {
    Ray from_top{ Position{ col, min_row }, SOUTH };
    visited = evaluate_rays(grid, from_top);
    if (visited.size() > max)
    {
      max = visited.size();
    }

    Ray from_bottom{ Position{ col, max_row}, NORTH };
    visited = evaluate_rays(grid, from_top);
    if (visited.size() > max)
    {
      max = visited.size();
    }
  }

  std::cout << "Optimal energy: " << max << " tiles." << std::endl;

  return 0;
}
