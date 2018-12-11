// Day2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../AoCHelpers/InputHandler.h"

#include "Box.h"

int main() {
  AdventOfCode::InputHandler input;
  std::vector<Box> boxes;
  for (const auto& box_name : input.read_all_lines()) {
    boxes.push_back(Box{ box_name });
  }

  auto box_has_two_count{ [](const Box& b) { return b.has_exactly_two_of_a_character(); } };
  unsigned int count_two = std::count_if(boxes.begin(), boxes.end(), box_has_two_count);

  auto box_has_three_count{ [](const Box& b) { return b.has_exactly_three_of_a_character(); } };
  unsigned int count_three = std::count_if(boxes.begin(), boxes.end(), box_has_three_count);

  std::wcout << L"Boxes with two: " << count_two << std::endl;
  std::wcout << L"Boxes with three: " << count_three << std::endl;
  std::wcout << L"Checksum: " << count_two * count_three << std::endl;

  for (unsigned int i = 0; i < boxes.size(); ++i) {
    for (unsigned int j = i + 1; j < boxes.size(); ++j) {
      if (1 == boxes[i].edit_distance(boxes[j])) {
        std::wcout << L"Boxes \"" << boxes[i].get_name() << L"\" and \"" << boxes[j].get_name()
            << L"\" differ by only 1 letter." << std::endl;
      }
    }
  }

  return 0;
}
