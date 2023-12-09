#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

#include "input_handler.h"
#include "cxxopts.hpp"

namespace {
static std::map<std::string, unsigned short> LIMITS =
{
  { "red", 12 },
  { "green", 13 },
  { "blue", 14 }
};
}

int main(int argc, char* argv[])
{
  cxxopts::Options options("d1", "Day 1 of Advent of Code");

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
  unsigned int valid_game_sum{0};
  unsigned long long power_set_sum{0};

  std::vector<std::string> games = input.read_all_lines();

  for (size_t index = 0; index < games.size(); ++index)
  {
    std::map<std::string, unsigned short> maximums = {
      { "red", 0 },
      { "green", 0 },
      { "blue", 0 }
    };
    std::vector<std::string> tokens = advent_of_code::tokenize(games[index], ": ");

    const std::string& pulls_string = tokens[1];
    std::vector<std::string> pulls = advent_of_code::tokenize(pulls_string, "; ");
    bool valid_game = true;
    for (const auto& pull : pulls)
    {
      std::vector<std::string> colors = advent_of_code::tokenize(pull, ", ");
      for (const auto& color : colors)
      {
        std::vector<std::string> info = advent_of_code::tokenize(color, " ");
        unsigned short count = std::stoi(info[0]);
        std::string c = info[1];
        maximums[c] = std::max(maximums[c], count);

        if (count > LIMITS[c])
        {
          valid_game = false;
        }
      }
    }

    power_set_sum += maximums["red"] * maximums["green"] * maximums["blue"];
    if (valid_game)
    {
      valid_game_sum += index + 1;
    }
  }

  std::cout << "Valid puzzle sum: " << valid_game_sum << std::endl;
  std::cout << "Power set sum: " << power_set_sum << std::endl;
  return 0;
}
