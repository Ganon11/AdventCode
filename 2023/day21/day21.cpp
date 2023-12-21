#include <iostream>
#include <map>
#include <set>
#include <vector>

#include "cxxopts.hpp"
#include "input_handler.h"
#include "position.h"

using advent_of_code::Position;

namespace
{
enum GardenSpace
{
  PLOT,
  ROCK
};

typedef std::map<Position, GardenSpace> Garden;

Garden parse_garden(const std::vector<std::string>& lines, Position& start)
{
  Garden g;
  for (size_t row = 0; row < lines.size(); ++row)
  {
    for (size_t col = 0; col < lines[row].size(); ++col)
    {
      Position p{ col, row };
      switch (lines[row][col])
      {
      case 'S':
        start = p;
      case '.':
        g[p] = PLOT;
        break;
      case '#':
        g[p] = ROCK;
        break;
      default:
        break;
      }
    }
  }

  return g;
}

std::set<Position> take_step(const Garden& g, const std::set<Position>& positions)
{
  std::set<Position> reachable;

  for (const Position& p : positions)
  {
    for (const Position& neighbor : p.get_adjacent_positions())
    {
      auto itr{ g.find(neighbor) };
      if (g.end() == itr || itr->second == PLOT)
      {
        reachable.insert(neighbor);
      }
    }
  }

  return reachable;
}
}

int main(int argc, char* argv[])
{
  cxxopts::Options options("d21", "Day 21 of Advent of Code");
  options.add_options()
    ("f,filename", "Input Filename", cxxopts::value<std::string>())
    ("h,help", "Print usage")
    ("s,steps", "Steps to take", cxxopts::value<unsigned int>())
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

  unsigned int steps = 1;
  if (result.count("steps"))
  {
    steps = result["steps"].as<unsigned int>();
  }

  advent_of_code::InputHandler input{ result["filename"].as<std::string>() };
  Position start;
  Garden garden{ parse_garden(input.read_all_lines(), start) };

  std::cout << "Start found at " << start << std::endl;

  std::set<Position> reachable;
  reachable.insert(start);
  for (unsigned int step = 0; step < steps; ++step)
  {
    reachable = take_step(garden, reachable);
  }

  std::cout << "After " << steps << " step(s), there are " << reachable.size() << " possible locations." << std::endl;
  return 0;
}
