// Day22.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <regex>
#include <vector>

#include "../AoCHelpers/InputHandler.h"
#include "../AoCHelpers/Position.h"
#include "Cave.h"

typedef unsigned long long ull;

bool get_input(const advent_of_code::InputHandler input,
  unsigned int& depth,
  advent_of_code::Position& p);

int wmain(int argc, wchar_t* argv[]) {
  advent_of_code::InputHandler input{ argc, argv };
  unsigned int depth;
  advent_of_code::Position target;
  if (!get_input(input, depth, target)) {
    std::wcerr << L"Malformed input" << std::endl;
    return -1;
  }

  Cave cave{ depth, target };

  ull risk_level{ cave.get_risk_level() };

  std::wcout << L"Risk level: " << risk_level << std::endl;

  ull cost_to_reach{ cave.cost_to_target() };

  std::wcout << L"Can reach target in " << cost_to_reach << L" minutes." << std::endl;

  return 0;
}

bool get_input(const advent_of_code::InputHandler input,
  unsigned int& depth,
  advent_of_code::Position& p) {

  std::vector<std::wstring> lines{ input.read_all_lines() };
  if (lines.size() != 2) {
    return false;
  }

  std::wregex depth_regex{ L"depth: (\\d+)" };
  std::wregex target_regex{ L"target: (\\d+),(\\d+)" };
  std::wsmatch matches;
  if (!std::regex_search(lines[0], matches, depth_regex) || matches.size() != 2) {
    return false;
  }

  depth = _wtoi(matches[1].str().c_str());
  if (!std::regex_search(lines[1], matches, target_regex) || matches.size() != 3) {
    return false;
  }

  p.m_x = _wtoi(matches[1].str().c_str());
  p.m_y = _wtoi(matches[2].str().c_str());

  return true;
}
