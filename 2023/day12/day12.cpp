#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "counter.h"
#include "cxxopts.hpp"
#include "input_handler.h"

namespace
{
unsigned long long count_arrangements(const std::string& line, const bool folded = false)
{
  std::vector<std::string> tokens{ advent_of_code::tokenize(line, ' ') };
  std::string springs{ tokens[0] };
  std::string groups_string{ tokens[1] };

  if (folded)
  {
    std::ostringstream os;
    os << springs << "?" << springs << "?" << springs << "?" << springs << "?" << springs;
    springs = os.str();

    os.str(std::string{});
    os << groups_string << "," << groups_string << "," << groups_string << "," << groups_string << "," << groups_string;
    groups_string = os.str();
  }

  std::vector<Condition> conditions;
  std::transform(springs.begin(), springs.end(), std::back_inserter(conditions), convert);

  std::vector<std::string> group_strings{ advent_of_code::tokenize(groups_string, ',') };
  std::vector<int> groups;
  std::transform(group_strings.begin(), group_strings.end(), std::back_inserter(groups), [](const std::string& t){ return std::stoi(t); });

  Counter c{ conditions, groups };
  return c.count();
}
}

int main(int argc, char* argv[])
{
  cxxopts::Options options("d12", "Day 12 of Advent of Code");
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

  unsigned long long sum_unfolded{0}, sum_folded{0};
  for (const std::string& line : lines)
  {
    sum_unfolded += count_arrangements(line);
    sum_folded += count_arrangements(line, true);
  }

  std::cout << "Unfolded: " << sum_unfolded << std::endl;
  std::cout << "Folded: " << sum_folded << std::endl;

  return 0;
}
