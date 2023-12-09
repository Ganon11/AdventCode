#include <iostream>
#include <numeric>
#include <vector>

#include "cxxopts.hpp"
#include "input_handler.h"
#include "race.h"

int main(int argc, char* argv[])
{
  cxxopts::Options options("d6", "Day 6 of Advent of Code");
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

  // Part 1
  std::vector<Race> races = parse_races(lines);
  std::vector<unsigned long long> ways_to_win;
  std::transform(races.begin(), races.end(), std::back_inserter(ways_to_win), [](const Race& r){ return r.ways_to_win(); });
  std::cout << "Win-ways product: " << std::reduce(ways_to_win.begin(), ways_to_win.end(), 1, std::multiplies<>{}) << std::endl;

  // Part 2
  Race big_race = Race(lines);
  std::cout << "Big race ways: " << big_race.ways_to_win() << std::endl;

  return 0;
}
