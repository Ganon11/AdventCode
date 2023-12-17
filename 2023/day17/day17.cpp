#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

#include "cxxopts.hpp"
#include "input_handler.h"
#include "position.h"

using advent_of_code::Position;

namespace
{
typedef std::map<Position, unsigned> Grid;

Grid parse_input(const std::vector<std::string>& lines)
{
  Grid grid;
  for (size_t row = 0; row < lines.size(); ++row)
  {
    for (size_t col = 0; col < lines[row].size(); ++col)
    {
      Position p{ col, row };
      grid.insert(Grid::value_type{ p, lines[row][col] - '0' });
    }
  }

  return grid;
}

enum Direction
{
  NONE,
  NORTH,
  WEST,
  EAST,
  SOUTH
};

struct LavaState
{
  Position position;
  unsigned straight_count;
  Direction direction;
  unsigned long long priority;

  bool operator<(const LavaState& other)
  {
    return priority < other.priority;
  }

  bool operator>(const LavaState& other)
  {
    return priority > other.priority;
  }
};

bool operator<(const LavaState& a, const LavaState& b)
{
  return a.priority < b.priority;
}

bool operator>(const LavaState& a, const LavaState& b)
{
  return a.priority > b.priority;
}

std::vector<LavaState> get_neighbors(const Grid& grid, const LavaState& state,
  const long long min_row, const long long min_col,
  const long long max_row, const long long max_col)
{
  std::vector<LavaState> neighbors;
  // go north
  if (state.direction != SOUTH)
  {
    if (state.direction != NORTH || state.straight_count < 3)
    {
      Position new_position{ state.position.north() };
      if (Position::in_bounds(min_row, min_col, max_row, max_col, new_position))
      {
        unsigned new_straight_count = state.direction == NORTH ? state.straight_count + 1 : 1;
        neighbors.push_back(LavaState{ new_position, new_straight_count, NORTH });
      }
    }
  }

  // go west
  if (state.direction != EAST)
  {
    if (state.direction != WEST || state.straight_count < 3)
    {
      Position new_position{ state.position.west() };
      if (Position::in_bounds(min_row, min_col, max_row, max_col, new_position))
      {
        unsigned new_straight_count = state.direction == WEST ? state.straight_count + 1 : 1;
        neighbors.push_back(LavaState{ new_position, new_straight_count, WEST });
      }
    }
  }

  // go east
  if (state.direction != WEST)
  {
    if (state.direction != EAST || state.straight_count < 3)
    {
      Position new_position{ state.position.east() };
      if (Position::in_bounds(min_row, min_col, max_row, max_col, new_position))
      {
        unsigned new_straight_count = state.direction == EAST ? state.straight_count + 1 : 1;
        neighbors.push_back(LavaState{ new_position, new_straight_count, EAST });
      }
    }
  }

  // go south
  if (state.direction != NORTH)
  {
    if (state.direction != SOUTH || state.straight_count < 3)
    {
      Position new_position{ state.position.south() };
      if (Position::in_bounds(min_row, min_col, max_row, max_col, new_position))
      {
        unsigned new_straight_count = state.direction == SOUTH ? state.straight_count + 1 : 1;
        neighbors.push_back(LavaState{ new_position, new_straight_count, SOUTH });
      }
    }
  }

  return neighbors;
}

unsigned long long find_shortest_path(const Grid& grid, const Position& destination, const Position& starting_position = advent_of_code::ORIGIN)
{
  std::priority_queue<LavaState, std::vector<LavaState>, std::greater<LavaState>> frontier;
  std::map<Position, Position> came_from;
  std::map<Position, unsigned long long> cost_so_far;
  long long min_row, min_col, max_row, max_col;

  {
    std::set<Position> all_positions;
    std::transform(grid.begin(), grid.end(), std::inserter(all_positions, all_positions.begin()), [](const auto& kvp){ return kvp.first; });
    Position::get_boundaries(all_positions, min_row, min_col, max_row, max_col);
  }

  frontier.push(LavaState{ starting_position, 0, NONE, 0 });
  came_from[starting_position] = starting_position;
  cost_so_far[starting_position] = 0;

  auto heuristic{ [&destination](const Position& p){ return destination.distance_to(p); } };

  while (!frontier.empty())
  {
    LavaState current{ frontier.top() };
    frontier.pop();

    //std::cout << "Currently at (" << current.position << "), facing " << current.direction << ", have been going straight for " << current.straight_count << " steps." << std::endl;

    if (destination == current.position)
    {
      break;
    }

    for (LavaState neighbor : get_neighbors(grid, current, min_row, min_col, max_row, max_col))
    {
      //std::cout << "\tExamining neighbor (" << neighbor.position << ")" << std::endl;
      unsigned long long new_cost = cost_so_far[current.position] + grid.at(neighbor.position);

      auto itr{ std::find_if(cost_so_far.begin(), cost_so_far.end(), [&neighbor](const auto& kvp){ return kvp.first == neighbor.position; }) };
      if (itr == cost_so_far.end() || new_cost < itr->second)
      {
        //std::cout << "\t\tNeighbor is new: new cost " << new_cost << std::endl;
        cost_so_far[neighbor.position] = new_cost;
        neighbor.priority = new_cost + heuristic(neighbor.position);
        frontier.push(neighbor);
        came_from[neighbor.position] = current.position;
      }
    }
  }

  std::vector<Position> path;
  {
    Position p{ destination };
    while (p != starting_position)
    {
      path.push_back(p);
      p = came_from.at(p);
    }
  }

  std::reverse(path.begin(), path.end());
  for (const auto& p : path)
  {
    std::cout << "(" << p << "), ";
  }
  std::cout << std::endl;

  return cost_so_far[destination];
}
}

int main(int argc, char* argv[])
{
  cxxopts::Options options("d17", "Day 17 of Advent of Code");
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

  Grid grid{ parse_input(lines) };
  Position source{ advent_of_code::ORIGIN };
  Position destination{ lines[0].size() - 1, lines.size() - 1 };
  unsigned long long cost{ find_shortest_path(grid, destination, source) };

  std::cout << "Shortest: " << cost << std::endl;

  return 0;
}
