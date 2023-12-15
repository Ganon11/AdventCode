#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <numeric>

#include "cxxopts.hpp"
#include "hashmap.h"
#include "input_handler.h"

int main(int argc, char* argv[])
{
  cxxopts::Options options("d15", "Day 15 of Advent of Code");
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
  std::string input_line{ input.read_single_line() };

  std::vector<std::string> tokens{ advent_of_code::tokenize(input_line, ',') };
  std::vector<unsigned int> hashes;
  std::transform(tokens.begin(), tokens.end(), std::back_inserter(hashes), hash);
  std::cout << "Hash total: " << std::reduce(hashes.begin(), hashes.end()) << std::endl;

  HASHMAP hashmap{ hash };
  for (const std::string& command : tokens)
  {
    hashmap.process_command(command);
  }
  std::cout << "Focusing power: " << hashmap.focusing_power() << std::endl;

  return 0;
}
