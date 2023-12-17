#include <iostream>
#include <iterator>
#include <map>
#include <queue>
#include <set>
#include <unordered_set>
#include <utility>
#include <vector>

#include "cxxopts.hpp"
#include "grid.h"
#include "input_handler.h"
#include "position.h"
#include "ray.h"

using advent_of_code::Position;

namespace
{
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
  visited[initial_ray.origin()].insert(initial_ray.direction());

  while (!rays.empty())
  {
    Ray ray{ rays.front() };
    rays.pop();
    bool went_out_of_bounds;
    Position terminal{ find_terminating_position(grid, ray, went_out_of_bounds) };
    std::vector<Position> positions{ advent_of_code::Position::get_positions_in_line(ray.origin(), terminal) };
    for (const auto& p : positions)
    {
      visited[p].insert(ray.direction());
    }

    if (went_out_of_bounds)
    {
      continue;
    }

    for (const auto& neighbor : get_resultant_rays(ray, terminal, grid.at(terminal)))
    {
      if (!Position::in_bounds(min_row, min_col, max_row, max_col, neighbor.origin()))
      {
        continue;
      }

      if (visited.contains(neighbor.origin()) && visited.at(neighbor.origin()).contains(neighbor.direction()))
      {
        continue;
      }

      rays.push(neighbor);
    }
  }

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
