#pragma once

#include <set>
#include <string>

#include "input_handler.h"

struct Card
{
public:
  Card() = default;
  Card(const std::string& line);

  int id;
  std::set<int> winners;
  std::set<int> numbers;

  int matching_numbers;
};
