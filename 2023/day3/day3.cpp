#include <algorithm>
#include <iostream>
#include <map>
#include <set>

#include "cxxopts.hpp"
#include "input_handler.h"
#include "position.h"
#include "schematic.h"

unsigned long long part_1(const std::map<advent_of_code::Position, SchematicSquare>& schematic)
{
  unsigned long long total{0};
  std::set<advent_of_code::Position> numbers_accounted_for;
  for (const auto& [k, v] : schematic)
  {
    if (v.type == POSITION_TYPE::SYMBOL)
    {
      //std::cout << "Looking at symbol " << v.character << " at " << v.position << std::endl;
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

        if (numbers_accounted_for.find(adj) != numbers_accounted_for.end())
        {
          continue;
        }

        //std::cout << "Unaccounted number found at adjacent position " << adj << std::endl;

        // Find first column with number
        long long start = -1;
        for (long long index = adj.x(); index >= 0; --index)
        {
          advent_of_code::Position p{ index, adj.y() };
          //std::cout << "Is " << p << " a number?" << std::endl;
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
        long long end;
        for (long long index = adj.x(); index < 1000; ++index)
        {
          advent_of_code::Position p{ index, adj.y() };
          if (schematic.find(p) == schematic.end() ||
            schematic.at(p).type != POSITION_TYPE::NUMBER)
          {
            end = index - 1;
            break;
          }
        }

        //std::cout << "Formed number at y=" << adj.y() << " from columns " << start << " through " << end << std::endl;

        // Build number
        unsigned long long subtotal{0};
        for (long long index = start; index <= end; ++index)
        {
          advent_of_code::Position p{ index, adj.y() };
          numbers_accounted_for.insert(p);
          subtotal *= 10;
          subtotal += schematic.at(p).character - '0';
        }

        //std::cout << "Adding part number " << subtotal << std::endl;
        total += subtotal;
      }
    }
  }

  return total;
}

unsigned long long part_2(const std::map<advent_of_code::Position, SchematicSquare>& schematic)
{
  unsigned long long total{0};
  for (const auto& [k, v] : schematic)
  {
    if (v.type == POSITION_TYPE::SYMBOL && v.character == '*')
    {
      unsigned short number_count{0};
      std::set<advent_of_code::Position> numbers_accounted_for;
      unsigned long long product{1};
      //std::cout << "Looking at symbol " << v.character << " at " << v.position << std::endl;
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

        if (numbers_accounted_for.find(adj) != numbers_accounted_for.end())
        {
          continue;
        }

        //std::cout << "Unaccounted number found at adjacent position " << adj << std::endl;

        // Find first column with number
        long long start = -1;
        for (long long index = adj.x(); index >= 0; --index)
        {
          advent_of_code::Position p{ index, adj.y() };
          //std::cout << "Is " << p << " a number?" << std::endl;
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
        long long end;
        for (long long index = adj.x(); index < 1000; ++index)
        {
          advent_of_code::Position p{ index, adj.y() };
          if (schematic.find(p) == schematic.end() ||
            schematic.at(p).type != POSITION_TYPE::NUMBER)
          {
            end = index - 1;
            break;
          }
        }

        //std::cout << "Formed number at y=" << adj.y() << " from columns " << start << " through " << end << std::endl;

        // Build number
        unsigned long long subtotal{0};
        for (long long index = start; index <= end; ++index)
        {
          advent_of_code::Position p{ index, adj.y() };
          numbers_accounted_for.insert(p);
          subtotal *= 10;
          subtotal += schematic.at(p).character - '0';
        }

        //std::cout << "Adding part number " << subtotal << std::endl;
        product *= subtotal;
        ++number_count;
      }

      if (number_count == 2)
      {
        total += product;
      }
    }
  }

  return total;
}

int main(int argc, char* argv[])
{
  cxxopts::Options options("d1", "Day 1 of Advent of Code");

  options.add_options()
    ("f,filename", "Input Filename", cxxopts::value<std::string>())
  ;

  auto result = options.parse(argc, argv);
  if (!result.count("filename"))
  {
    return -1;
  }

  advent_of_code::InputHandler input{ result["filename"].as<std::string>() };

  std::vector<std::string> lines = input.read_all_lines();
  std::map<advent_of_code::Position, SchematicSquare> schematic;
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
