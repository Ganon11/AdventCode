#include <iostream>
#include <vector>

#include "cxxopts.hpp"
#include "input_handler.h"
#include "valley.h"

namespace
{
std::vector<Valley> parse_valleys(const advent_of_code::InputHandler& input)
{
  std::vector<std::string> buffer;
  std::vector<Valley> valleys;
  for (const std::string& line : input.read_all_lines(true))
  {
    if (line.empty())
    {
      valleys.push_back(Valley{ buffer });
      buffer.clear();
    }
    else
    {
      buffer.push_back(line);
    }
  }

  if (!buffer.empty())
  {
    valleys.push_back(Valley{ buffer });
  }

  return valleys;
}
}

int main(int argc, char* argv[])
{
  cxxopts::Options options("d13", "Day 13 of Advent of Code");
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
  std::vector<Valley> valleys{ parse_valleys(input) };

  unsigned long long rows{0}, cols{0}, rows_smudge{0}, cols_smudge{0};
  for (const Valley& v : valleys)
  {
    int r{ v.rows() };
    if (r != -1)
    {
      rows += r;
    }

    int c{ v.columns() };
    if (c != -1)
    {
      cols += c;
    }

    int r_s{ v.rows(true) };
    if (r_s != -1)
    {
      rows_smudge += r;
    }

    int c_s{ v.columns(true) };
    if (c_s != -1)
    {
      cols_smudge += r;
    }
  }

  std::cout << (100 * rows) + cols << std::endl;
  std::cout << (100 * rows_smudge) + cols_smudge << std::endl;

  return 0;
}
