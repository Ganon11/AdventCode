#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>

#include "cxxopts.hpp"
#include "input_handler.h"

namespace {
static short PART = 1;
struct Card
{
public:
  Card() = default;
  Card(const std::string& line) : matching_numbers{ 0 }
  {
    std::vector<std::string> tokens = advent_of_code::tokenize(line, ": ");
    std::string name = tokens[0];
    id = std::stoi(advent_of_code::tokenize(name, ' ')[1]);

    std::string data = tokens[1];
    tokens = advent_of_code::tokenize(data, " | ");
    std::string winners_str = tokens[0];
    std::string numbers_str = tokens[1];

    std::set<int> winning_numbers;
    for (const auto& number_str : advent_of_code::tokenize(winners_str, ' '))
    {
      winners.insert(std::stoi(number_str));
    }

    for (const auto& number_str : advent_of_code::tokenize(numbers_str, ' '))
    {
      int number{ std::stoi(number_str) };
      numbers.insert(number);
      if (winners.contains(number))
      {
        ++matching_numbers;
      }
    }
  }

  int id;
  std::set<int> winners;
  std::set<int> numbers;

  int matching_numbers;
};
}

int main(int argc, char* argv[])
{
  cxxopts::Options options("d1", "Day 1 of Advent of Code");

  options.add_options()
    ("f,filename", "Input Filename", cxxopts::value<std::string>())
    ("p,part", "Part 1 or 2", cxxopts::value<short>()->default_value("1"))
  ;

  auto result = options.parse(argc, argv);
  if (!result.count("filename"))
  {
    return -1;
  }

  if (result.count("part"))
  {
    ::PART = result["part"].as<short>();
  }

  advent_of_code::InputHandler input{ result["filename"].as<std::string>() };
  std::vector<std::string> lines = input.read_all_lines();
  std::map<int, Card> cards;
  unsigned int total{0};
  std::queue<int> card_queue;
  for (const auto& line : lines)
  {
    Card card{ line };
    cards[card.id] = card;
    card_queue.emplace(card.id);

    if (card.matching_numbers == 1)
    {
      total += 1;
    }
    else if (card.matching_numbers > 1)
    {
      total += 1 << (card.matching_numbers - 1);
    }
  }

  std::cout << "Total points: " << total << std::endl;

  unsigned int pile{0};
  while (!card_queue.empty())
  {
    int id = card_queue.front();
    card_queue.pop();
    ++pile;

    //std::cout << "Processing card id " << id << std::endl;
    if (!cards.contains(id))
    {
      //std::cout << "\tCard doesn't exist" << std::endl;
      continue;
    }

    Card card = cards.at(id);
    for (auto count = 1; count <= card.matching_numbers; ++count)
    {
      //std::cout << "\tAdding card with id " << id + count << " to processing queue." << std::endl;
      card_queue.emplace(id + count);
    }
  }

  std::cout << "Card pile contains " << pile << " cards." << std::endl;
  return 0;
}
