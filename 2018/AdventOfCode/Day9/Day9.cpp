// Day9.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <regex>
#include <string>

#include "../AoCHelpers/InputHandler.h"
#include "MarbleList.h"

int main() {
  AdventOfCode::InputHandler input;
  std::wstring line{ input.read_single_line() };

  std::wregex marble_regex{ L"(\\d+) players; last marble is worth (\\d+) points" };
  std::wsmatch matches;
  std::regex_search(line, matches, marble_regex);
  if (matches.size() != 3) // whole match, then players and marble
  {
    std::wcout << L"Malformed input" << std::endl;
    return -1;
  }

  const unsigned int players{ static_cast<unsigned int>(_wtoi(matches[1].str().c_str())) };
  const unsigned int max_marbles{ static_cast<unsigned int>(_wtoi(matches[2].str().c_str())) };

  MarbleList list{ players, max_marbles };
  unsigned int winning_score{ list.play_game() };

  std::wcout << players << L" players; last marble is worth " << max_marbles
    << L" points: high score is " << winning_score << std::endl;

  MarbleList list2{ players, max_marbles * 100 };
  winning_score = list2.play_game();
  std::wcout << players << L" players; last marble is worth " << max_marbles * 100
    << L" points: high score is " << winning_score << std::endl;

  return 0;
}
