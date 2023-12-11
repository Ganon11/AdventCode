#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <vector>

#include "cxxopts.hpp"
#include "input_handler.h"
#include "position.h"
#include "maze.h"

using advent_of_code::Position;

namespace
{
PipeType convert(const char ch)
{
  switch (ch)
  {
    case '|':
      return VERTICAL;
    case '-':
      return HORIZONTAL;
    case 'L':
      return NORTH_EAST_BEND;
    case 'J':
      return NORTH_WEST_BEND;
    case '7':
      return SOUTH_WEST_BEND;
    case 'F':
      return SOUTH_EAST_BEND;
    case '.':
      return GROUND;
    case 'S':
      return START;
      
    default:
      return NONE;
  }
}
}

bool parse_input(const std::vector<std::string>& lines, Position& start, Maze& maze)
{
  bool start_set{ false };
  for (size_t row = 0; row < lines.size(); ++row)
  {
    for (size_t col = 0; col < lines[row].size(); ++col)
    {
      Position current{ col, row };
      MazeNode node{ current };
      PipeType type{ convert(lines[row][col]) };
      if (type == NONE)
      {
        return false;
      }
      else if (type == START)
      {
        start = current;
        start_set = true;
      }

      node.set_type(type);

      maze[current] = node;
    }
  }

  if (!start_set)
  {
    return false;
  }

  Position n{ start.north() }, s{ start.south() }, e{ start.east() }, w{ start.west() };
  for (const Position& neighbor : start.get_adjacent_positions())
  {
    if (maze.find(neighbor) != maze.end() && maze.at(neighbor).can_reach(start))
    {
      maze[start].add_adjacent_position(neighbor);
    }
  }

  maze[start].deduce_type();
  return true;
}

void explore_loop(const Maze& maze, const Position& start, std::set<Position>& loop)
{
  std::queue<Position> frontier;
  frontier.push(start);
  loop.insert(start);

  while (!frontier.empty())
  {
    Position current = frontier.front();
    frontier.pop();

    for (const Position& neighbor : maze.at(current).exits())
    {
      if (loop.find(neighbor) == loop.end())
      {
        loop.insert(neighbor);
        frontier.push(neighbor);
      }
    }
  }
}

unsigned long long scan_for_ground(const Maze& maze, const std::set<Position>& loop)
{
  unsigned long long total{0};
  unsigned long long min_row = std::min_element(loop.begin(), loop.end(), [](const auto& a, const auto& b){ return a.y() < b.y(); })->y();
  unsigned long long min_col = std::min_element(loop.begin(), loop.end(), [](const auto& a, const auto& b){ return a.x() < b.x(); })->x();
  unsigned long long max_row = std::max_element(loop.begin(), loop.end(), [](const auto& a, const auto& b){ return a.y() < b.y(); })->y();
  unsigned long long max_col = std::max_element(loop.begin(), loop.end(), [](const auto& a, const auto& b){ return a.x() < b.x(); })->x();

  for (unsigned long long row = min_row; row <= max_row; ++row)
  {
    bool in_loop{ false };
    PipeType last_corner_found{ NONE };
    for (unsigned long long col = min_col; col <= max_col; ++col)
    {
      Position p{ col, row };
      if (loop.find(p) != loop.end())
      {
        PipeType type{ maze.at(p).type() };
        // Horizontal pipes do not change the "inside" state
        if (type == HORIZONTAL)
        {
          continue;
        }

        // Vertical lines trivially change the "inside" state
        if (type == VERTICAL)
        {
          in_loop = !in_loop;
          continue;
        }

        // The only remaining types are corners. Two corners _may_ change the "inside" state iff
        // they form an "S-bend". This takes two corners, so we keep track of the first corner in
        // last_corner_found.
        if (last_corner_found == NONE)
        {
          last_corner_found = type;
          continue;
        }

        // A north-east and south-west pair form an S-bend, and a north-west and south-east pair
        // also form an S-bend.
        if ((NORTH_EAST_BEND == last_corner_found && SOUTH_WEST_BEND == type) ||
          (SOUTH_WEST_BEND == last_corner_found && NORTH_EAST_BEND == type) ||
          (NORTH_WEST_BEND == last_corner_found && SOUTH_EAST_BEND == type) ||
          (SOUTH_EAST_BEND == last_corner_found && NORTH_WEST_BEND == type))
        {
          in_loop = !in_loop;
        }

        // Other combinations form a U-bend, which does not change the "inside" state.
        // In all cases, clear the temporary last_corner_found cache.
        last_corner_found = NONE;
      }
      else if (in_loop)
      {
        ++total;
      }
    }
  }

  return total;
}

int main(int argc, char* argv[])
{
  cxxopts::Options options("d10", "Day 10 of Advent of Code");
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
  Position start;
  Maze maze;

  if (!parse_input(lines, start, maze))
  {
    return 1;
  }

  std::set<Position> loop;
  explore_loop(maze, start, loop);
  std::cout << "Farthest point is " << loop.size() / 2 << " away." << std::endl;
  unsigned long long enclosed_count = scan_for_ground(maze, loop);
  std::cout << "Loop contains " << enclosed_count << " tiles." << std::endl;

  return 0;
}
