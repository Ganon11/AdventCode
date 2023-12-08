#include <iostream>

#include "cxxopts.hpp"
#include "input_handler.h"
#include "maze.h"

int main(int argc, char* argv[])
{
  cxxopts::Options options("d8", "Day 8 of Advent of Code");
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
    return -1;
  }

  advent_of_code::InputHandler input{ result["filename"].as<std::string>() };
  std::vector<std::string> lines{ input.read_all_lines() };
  std::string directions = lines[0];
  std::map<std::string, MazeNode> maze;
  for (size_t index = 1; index < lines.size(); ++index)
  {
    MazeNode node{ lines[index] };
    maze[node.label()] = node;
  }

  std::string location{ "AAA" };
  size_t directions_index = 0;
  unsigned long long steps{0};
  while (location != "ZZZ")
  {
    MazeNode node{ maze.at(location) };
    ++steps;
    switch (directions[directions_index])
    {
      case 'L':
        location = node.left();
        break;
      case 'R':
        location = node.right();
        break;
    }

    directions_index = (directions_index + 1) % directions.size();
  }

  std::cout << "Reached ZZZ in " << steps << " steps." << std::endl;

  return 0;
}
