#include <algorithm>
#include <iostream>
#include <map>
#include <set>

#include "cxxopts.hpp"
#include "input_handler.h"
#include "position.h"
#include "schematic.h"

typedef std::map<advent_of_code::Position, SchematicSquare> Schematic;
typedef Schematic::value_type SchematicValue;

unsigned long long part_1(const Schematic& schematic)
{
  unsigned long long total{0};
  auto max_column = std::max_element(schematic.begin(), schematic.end(), [](const SchematicValue& a, const SchematicValue& b){ return a.first.x() < b.first.x(); })->first.x() + 1;
  std::set<advent_of_code::Position> numbers_accounted_for;
  for (const auto& [k, v] : schematic)
  {
    if (v.type != POSITION_TYPE::SYMBOL)
    {
      continue;
    }

    for (const auto& adj : k.get_adjacent_positions(true))
    {
      if (schematic.find(adj) == schematic.end())
      {
        continue;
      }

      if (schematic.at(adj).type != POSITION_TYPE::NUMBER)
      {
        continue;
      }

      if (numbers_accounted_for.contains(adj))
      {
        continue;
      }

      // Find first column with number
      long long start = -1;
      for (long long index = adj.x(); index >= 0; --index)
      {
        advent_of_code::Position p{ index, adj.y() };
        if (schematic.at(p).type != POSITION_TYPE::NUMBER)
        {
          start = index + 1;
          break;
        }
      }

      if (start == -1)
      {
        start = 0;
      }

      // Find last column with number
      long long end{-1};
      for (long long index = adj.x(); index <= max_column; ++index)
      {
        advent_of_code::Position p{ index, adj.y() };
        if (!schematic.contains(p) ||
          schematic.at(p).type != POSITION_TYPE::NUMBER)
        {
          end = index - 1;
          break;
        }
      }

      // Build number
      unsigned long long subtotal{0};
      for (long long index = start; index <= end; ++index)
      {
        advent_of_code::Position p{ index, adj.y() };
        numbers_accounted_for.insert(p);
        subtotal *= 10;
        subtotal += schematic.at(p).character - '0';
      }

      total += subtotal;
    }
  }

  return total;
}

unsigned long long part_2(const Schematic& schematic)
{
  unsigned long long total{0};
  auto max_column = std::max_element(schematic.begin(), schematic.end(), [](const SchematicValue& a, const SchematicValue& b){ return a.first.x() < b.first.x(); })->first.x() + 1;
  for (const auto& [k, v] : schematic)
  {
    if (v.character != '*')
    {
      continue;
    }

    unsigned short number_count{0};
    std::set<advent_of_code::Position> numbers_accounted_for;
    unsigned long long product{1};
    for (const auto& adj : k.get_adjacent_positions(true))
    {
      if (!schematic.contains(adj))
      {
        continue;
      }

      if (schematic.at(adj).type != POSITION_TYPE::NUMBER)
      {
        continue;
      }

      if (numbers_accounted_for.contains(adj))
      {
        continue;
      }

      // Find first column with number
      long long start = -1;
      for (long long index = adj.x(); index >= 0; --index)
      {
        advent_of_code::Position p{ index, adj.y() };
        if (schematic.at(p).type != POSITION_TYPE::NUMBER)
        {
          start = index + 1;
          break;
        }
      }

      if (start == -1)
      {
        start = 0;
      }

      // Find last column with number
      long long end{-1};
      for (long long index = adj.x(); index <= max_column; ++index)
      {
        advent_of_code::Position p{ index, adj.y() };
        if (!schematic.contains(p) ||
          schematic.at(p).type != POSITION_TYPE::NUMBER)
        {
          end = index - 1;
          break;
        }
      }

      // Build number
      unsigned long long subtotal{0};
      for (long long index = start; index <= end; ++index)
      {
        advent_of_code::Position p{ index, adj.y() };
        numbers_accounted_for.insert(p);
        subtotal *= 10;
        subtotal += schematic.at(p).character - '0';
      }

      product *= subtotal;
      ++number_count;
    }

    if (number_count == 2)
    {
      total += product;
    }
  }

  return total;
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

  std::vector<std::string> lines = input.read_all_lines();
  Schematic schematic;
  for (size_t row = 0; row < lines.size(); ++row)
  {
    for (size_t col = 0; col < lines[row].size(); ++col)
    {
      advent_of_code::Position p{ static_cast<unsigned long long>(col), static_cast<unsigned long long>(row) };
      schematic[p] = SchematicSquare{ p, lines[row][col] };
    }
  }

  std::cout << "Part number total: " << part_1(schematic) << std::endl;
  std::cout << "Gear ratio total: " << part_2(schematic) << std::endl;

  return 0;
}
