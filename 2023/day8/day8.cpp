#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

#include "cxxopts.hpp"
#include "input_handler.h"
#include "maze.h"

namespace {
unsigned long long steps_to_solve(const std::map<std::string, MazeNode>& maze, const std::string directions, const std::string start = "AAA", const bool strict_end = true)
{
  std::string location = start;
  size_t directions_index = 0;
  unsigned long long steps{0};
  std::function<bool(const std::string&)> end{ [=](const std::string& location){
    if (strict_end)
    {
      return location == "ZZZ";
    }
    else
    {
      return location.find('Z') != std::string::npos;
    }
  }};

  while (!end(location))
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

  return steps;
}
}

int main(int argc, char* argv[])
{
  cxxopts::Options options("d8", "Day 8 of Advent of Code");
  options.add_options()
    ("f,filename", "Input Filename", cxxopts::value<std::string>())
    ("p,part", "Part 1 or 2", cxxopts::value<unsigned short>())
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

  unsigned short part = 1;
  if (result.count("part"))
  {
    part = result["part"].as<unsigned short>();
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

  if (1 == part)
  {
    std::cout << "Reached ZZZ in " << steps_to_solve(maze, directions, "AAA") << " steps." << std::endl;
  }
  else if (2 == part)
  {
    std::vector<std::string> start_locations;
    {
      std::vector<std::string> locations;
      std::transform(maze.begin(), maze.end(), std::back_inserter(locations), [](const auto& kvp){ return kvp.first; });
      std::copy_if(locations.begin(), locations.end(), std::back_inserter(start_locations), [](const std::string& loc){ return loc.find('A') != std::string::npos; });
    }

    std::vector<unsigned long long> steps;
    std::transform(start_locations.begin(), start_locations.end(), std::back_inserter(steps), [&maze,&directions](const std::string& location){ return steps_to_solve(maze, directions, location, false); });
    unsigned long long answer = std::accumulate(steps.begin(), steps.end(), 1ull, std::lcm<unsigned long long, unsigned long long>);

    std::cout << "All exits reached in " << answer << " steps." << std::endl;
  }
  else
  {
    std::cerr << "Unknown part: " << part << std::endl;
    return 1;
  }

  return 0;
}
