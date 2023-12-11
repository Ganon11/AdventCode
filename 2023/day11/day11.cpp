#include <algorithm>
#include <iostream>
#include <iterator>
#include <set>

#include "cxxopts.hpp"
#include "input_handler.h"
#include "position.h"

using advent_of_code::Position;

namespace
{
std::set<Position> get_galaxies(const std::vector<std::string>& lines)
{
  std::set<Position> galaxies;

  long long row{0};
  for (const std::string& line : lines)
  {
    for (long long col = 0; col < line.size(); ++col)
    {
      if (line[col] == '#')
      {
        galaxies.insert(Position{ col, row });
      }
    }

    ++row;
  }

  return galaxies;
}

void find_boundaries(const std::set<Position>& galaxies, long long& min_row, long long& min_col, long long& max_row, long long& max_col)
{
  min_row = std::min_element(galaxies.begin(), galaxies.end(), [](const auto& a, const auto& b){ return a.y() < b.y(); })->y();
  min_col = std::min_element(galaxies.begin(), galaxies.end(), [](const auto& a, const auto& b){ return a.x() < b.x(); })->x();
  max_row = std::max_element(galaxies.begin(), galaxies.end(), [](const auto& a, const auto& b){ return a.y() < b.y(); })->y();
  max_col = std::max_element(galaxies.begin(), galaxies.end(), [](const auto& a, const auto& b){ return a.x() < b.x(); })->x();
}

std::set<Position> expand_rows(const std::set<Position>& galaxies, const long long multiplier)
{
  std::set<Position> new_galaxies;
  long long offset{0};
  long long min_row, min_col, max_row, max_col;
  find_boundaries(galaxies, min_row, min_col, max_row, max_col);

  for (auto row = min_row; row <= max_row; ++row)
  {
    auto filter_func = [row](const Position& p){ return p.y() == row; };
    std::set<Position> temporary;
    std::copy_if(galaxies.begin(), galaxies.end(), std::inserter(temporary, temporary.end()), filter_func);
    if (temporary.empty())
    {
      // No galaxies in this row - increase the offset
      ++offset;
    }
    else
    {
      // Increase each galaxy's y by the offset in the new universe.
      for (const Position& galaxy : temporary)
      {
        new_galaxies.insert(Position{ galaxy.x(), galaxy.y() + (offset * multiplier) });
      }
    }
  }

  return new_galaxies;
}

std::set<Position> expand_columns(const std::set<Position>& galaxies, const long long multiplier)
{
  std::set<Position> new_galaxies;
  long long offset{0};
  long long min_row, min_col, max_row, max_col;
  find_boundaries(galaxies, min_row, min_col, max_row, max_col);

  for (auto col = min_col; col <= max_col; ++col)
  {
    auto filter_func = [col](const Position& p){ return p.x() == col; };
    std::set<Position> temporary;
    std::copy_if(galaxies.begin(), galaxies.end(), std::inserter(temporary, temporary.end()), filter_func);
    if (temporary.empty())
    {
      // No galaxies in this row - increase the offset
      ++offset;
    }
    else
    {
      // Increase each galaxy's y by the offset in the new universe.
      for (const Position& galaxy : temporary)
      {
        new_galaxies.insert(Position{ galaxy.x() + (offset * multiplier), galaxy.y() });
      }
    }
  }

  return new_galaxies;
}

std::set<Position> expand_by(const std::set<Position>& galaxies, const long long multiplier)
{
  return expand_rows(expand_columns(galaxies, multiplier), multiplier);
}

std::set<Position> expand(const std::set<Position>& galaxies)
{
  return expand_by(galaxies, 1);
}

unsigned long long sum_of_distances(const std::set<Position>& galaxies)
{
  unsigned long long sum{0};

  // Can't think of a better way to get pairs
  for (auto itr1 = galaxies.begin(); itr1 != galaxies.end(); ++itr1)
  {
    for (auto itr2 = itr1; itr2 != galaxies.end(); ++itr2)
    {
      if (itr1 == itr2) continue;
      sum += itr1->distance_to(*itr2);
    }
  }

  return sum;
}
}

int main(int argc, char* argv[])
{
  cxxopts::Options options("d11", "Day 11 of Advent of Code");
  options.add_options()
    ("f,filename", "Input Filename", cxxopts::value<std::string>())
    ("h,help", "Print usage")
    ("m,multiplier", "How much to super-expand by", cxxopts::value<unsigned long long>())
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

  if (!result.count("multiplier"))
  {
    std::cout << "ERROR: Missing required argument: <multiplier>" << std::endl;
    std::cout << options.help() << std::endl;
    return -1;
  }

  advent_of_code::InputHandler input{ result["filename"].as<std::string>() };
  std::vector<std::string> lines{ input.read_all_lines() };

  std::set<Position> galaxies{ get_galaxies(lines) };

  std::set<Position> expanded{ expand(galaxies) };
  unsigned long long sum{ sum_of_distances(expanded) };
  std::cout << "Sum of distances: " << sum << std::endl;

  unsigned long long multiplier{ result["multiplier"].as<unsigned long long>() };
  std::set<Position> super_expanded{ expand_by(galaxies, multiplier) };
  sum = sum_of_distances(super_expanded);
  std::cout << "Sum of distances: " << sum << std::endl;

  return 0;
}
