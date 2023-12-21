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
typedef std::set<Position> Garden;

Garden parse_garden(const std::vector<std::string>& lines, Position& start, unsigned int& rows,
  unsigned int& columns)
{
  Garden g;
  rows = lines.size();
  columns = lines[0].size();
  for (size_t row = 0; row < rows; ++row)
  {
    for (size_t col = 0; col < columns; ++col)
    {
      Position p{ col, row };
      switch (lines[row][col])
      {
      case 'S':
        start = p;
      case '.':
        g.insert(p);
        break;
      default:
        break;
      }
    }
  }

  return g;
}

std::set<Position> take_step(const Garden& g, const std::set<Position>& positions,
  const unsigned int rows, const unsigned int columns)
{
  std::set<Position> reachable;

  for (const Position& p : positions)
  {
    for (const Position& neighbor : p.get_adjacent_positions())
    {
      Position actual{ neighbor.x() % columns, neighbor.y() % rows };
      auto itr{ g.find(actual) };
      if (g.end() != itr)
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
  unsigned int rows, columns;
  Garden garden{ parse_garden(input.read_all_lines(), start, rows, columns) };

  std::cout << "Start found at " << start << std::endl;

  std::set<Position> reachable;
  reachable.insert(start);
  for (unsigned int step = 0; step < steps; ++step)
  {
    reachable = take_step(garden, reachable, rows, columns);
  }

  std::cout << "After " << steps << " step(s), there are " << reachable.size() << " possible locations." << std::endl;
  return 0;
}
