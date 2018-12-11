// Day3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include "Claim.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "../AoCHelpers/InputHandler.h"

using namespace std;

int main() {
  AdventOfCode::InputHandler input;
  vector<Claim> claims;
  for (const auto& line : input.read_all_lines()) {
    claims.push_back(Claim{ line });
  }

  map<Claim::Coordinate, unsigned int> fabric;

  for (const Claim& claim : claims) {
    for (const Claim::Coordinate& coordinate : claim.generate_coordinates()) {
      if (fabric.end() == fabric.find(coordinate)) {
        fabric[coordinate] = 1u;
      } else {
        ++fabric[coordinate];
      }
    }
  }

  auto inch_is_double_claimed{ [](const auto& claim) { return claim.second > 1u; } };
  unsigned int inches_double_claimed = count_if(fabric.begin(), fabric.end(),
      inch_is_double_claimed);

  wcout << L"Square inches claimed multiple times: " << inches_double_claimed << endl;

  auto coordinate_is_single_claimed{ [fabric](const Claim::Coordinate& coordinate) {
      return fabric.at(coordinate) == 1u; } };
  for (const Claim& claim : claims) {
    const vector<Claim::Coordinate> coordinates{ claim.generate_coordinates() };
    if (all_of(coordinates.begin(), coordinates.end(), coordinate_is_single_claimed)) {
      wcout << L"Claim #" << claim.get_id() << L" does not overlap with any others." << endl;
      break;
    }
  }

  return 0;
}
