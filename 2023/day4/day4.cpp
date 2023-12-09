#include <deque>
#include <iostream>
#include <map>
#include <set>
#include <string>

#include "card.h"
#include "cxxopts.hpp"
#include "input_handler.h"

int main(int argc, char* argv[])
{
  cxxopts::Options options("d4", "Day 4 of Advent of Code");

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
  std::map<int, Card> cards;
  unsigned int total{0};
  std::deque<int> card_queue;
  for (const auto& line : lines)
  {
    Card card{ line };
    cards[card.id] = card;
    card_queue.emplace_back(card.id);

    total += 1 << (card.matching_numbers - 1);
  }

  std::cout << "Total points: " << total << std::endl;

  unsigned int pile{0};
  std::map<int, int> score_map;

  while (!card_queue.empty())
  {
    int id = card_queue.back();
    card_queue.pop_back();
    int card_total = 1;

    Card card = cards.at(id);
    for (auto count = 1; count <= card.matching_numbers; ++count)
    {
      int new_id = id + count;
      if (!cards.contains(new_id))
      {
        continue;
      }
      else
      {
        card_total += score_map.at(new_id);
      }
    }

    score_map[id] = card_total;
    pile += card_total;
  }

  std::cout << "Card pile contains " << pile << " cards." << std::endl;
  return 0;
}
