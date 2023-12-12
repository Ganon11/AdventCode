#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

#include "cxxopts.hpp"
#include "input_handler.h"

namespace
{
bool is_valid_arrangement(const std::string& springs, const::std::vector<unsigned long long> groups, const bool verbose = false)
{
  if (verbose)
  {
    std::cout << "Checking if \"" << springs << " is a valid arrangement." << std::endl;
    std::cout << "\tGroups are: ";
    for (const unsigned long long group : groups)
    {
      std::cout << group << ", ";
    }
    std::cout << std::endl;
  }

  size_t group_index{0};
  char previous_spring = '.';
  unsigned long long current_group_size{0};
  for (size_t spring_index = 0; spring_index < springs.size(); ++spring_index)
  {
    char spring{ springs[spring_index] };

    switch (spring)
    {
      case '.':
        if (previous_spring == '#')
        {
          if (current_group_size != groups[group_index])
          {
            if (verbose) std::cout << "\tInvalid group found at springs[" << spring_index << "], group " << group_index << std::endl;
            return false;
          }

          current_group_size = 0;
          ++group_index;
        }
        break;
      case '#':
        ++current_group_size;
        break;
      case '?':
      default:
        if (verbose) std::cout << "\tInvalid character found at springs[" << spring_index << "]" << std::endl;
        return false;
    }

    if (groups.size() < group_index)
    {
      if (verbose) std::cout << "\tToo many groups!" << std::endl;
      return false;
    }

    previous_spring = spring;
  }

  // If the last character was part of a spring, the group is now closed, so increment the index.
  if (previous_spring == '#')
  {
    if (current_group_size != groups[group_index])
    {
      if (verbose) std::cout << "\tLast group is too big!" << std::endl;
      return false;
    }

    ++group_index;
  }

  if (groups.size() != group_index)
  {
    if (verbose) std::cout << "\tNot all groups used! Group index = " << group_index << std::endl;
    return false;
  }

  if (verbose)
  {
    std::cout << "Checking if \"" << springs << " is a valid arrangement." << std::endl;
    std::cout << "\tGroups are: ";
    for (const unsigned long long group : groups)
    {
      std::cout << group << ", ";
    }
    std::cout << std::endl;
    std::cout << "\tValid!" << std::endl;
  }
  return true;
}

unsigned long long count_arrangements(std::string& springs, size_t index, const std::vector<unsigned long long>& groups)
{
  while (index < springs.size() && springs[index] != '?')
  {
    ++index;
  }

  if (index == springs.size())
  {
    return is_valid_arrangement(springs, groups) ? 1 : 0;
  }

  unsigned long long sum{0};
  springs[index] = '.';
  sum += count_arrangements(springs, index + 1, groups);
  springs[index] = '#';
  sum += count_arrangements(springs, index + 1, groups);
  springs[index] = '?';

  return sum;
}

unsigned long long count_arrangements(const std::string& springs, const std::vector<unsigned long long>& groups)
{
  std::string copy{ springs };
  return count_arrangements(copy, 0, groups);
}

unsigned long long count_arrangements(const std::string& line)
{
  std::vector<std::string> tokens{ advent_of_code::tokenize(line, ' ') };
  std::string springs{ tokens[0] };
  std::vector<std::string> group_strings{ advent_of_code::tokenize(tokens[1], ',') };
  std::vector<unsigned long long> groups;
  std::transform(group_strings.begin(), group_strings.end(), std::back_inserter(groups), [](const std::string& t){ return std::stoll(t); });

  return count_arrangements(springs, groups);
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

  unsigned long long sum{0};
  for (const std::string& line : lines)
  {
    unsigned long long count{ count_arrangements(line) };
    //std::cout << "Line \"" << line << "\" arrangements: " << count << std::endl;
    sum += count;
  }

  std::cout << sum << " arrangements." << std::endl;

  return 0;
}
