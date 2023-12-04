#include "card.h"

#include <vector>

Card::Card(const std::string& line) : matching_numbers{ 0 }
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
