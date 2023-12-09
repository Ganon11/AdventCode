#include <algorithm>
#include <iostream>
#include <iterator>

#include "camel_card.h"
#include "cxxopts.hpp"
#include "input_handler.h"

int main(int argc, char* argv[])
{
  cxxopts::Options options("d7", "Day 7 of Advent of Code");
  options.add_options()
    ("f,filename", "Input Filename", cxxopts::value<std::string>())
    ("h,help", "Print usage")
    ("p,part", "Part 1 or 2", cxxopts::value<unsigned short>())
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

  unsigned short part = 1;
  if (result.count("part"))
  {
    part = result["part"].as<unsigned short>();
  }

  advent_of_code::InputHandler input{ result["filename"].as<std::string>() };
  std::vector<std::string> lines{ input.read_all_lines() };
  std::vector<CamelCardHand> hands;
  if (1 == part)
  {
    std::transform(lines.begin(), lines.end(), std::back_inserter(hands), [](const std::string& s){ return CamelCardHand{ s }; });
  }
  else
  {
    std::transform(lines.begin(), lines.end(), std::back_inserter(hands), [](const std::string& s){ return CamelCardHand{ s, true }; });
  }
  std::sort(hands.begin(), hands.end());
  unsigned long long sum{0};
  for (size_t index = 0; index < hands.size(); ++index)
  {
    const auto& hand = hands[index];
    //std::cout << "Hand " << hand.hand() << ", which is a " << hand.type() << ", came in rank " << index + 1 << ", winnings " << hand.bid() << std::endl;
    sum += (index + 1) * hand.bid();
  }

  std::cout << "Total winnings: " << sum << std::endl;

  return 0;
}
