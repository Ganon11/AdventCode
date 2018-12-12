// AdventOfCode.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <Windows.h>

#include "../AoCHelpers/InputHandler.h"

bool add_and_check_frequency(int frequency, int& total, std::set<int> &seenFrequencies);

int wmain(int argc, wchar_t* argv[]) {
  AdventOfCode::InputHandler input{ argc, argv };
  int total{ 0 };
  std::vector<int> frequencies;
  std::set<int> seenFrequencies;
  seenFrequencies.insert(total);
  bool found{ false };
  for (const auto& line : input.read_all_lines()) {
    int frequency{ _wtoi(line.c_str()) };
    frequencies.push_back(frequency);

    if (add_and_check_frequency(frequency, total, seenFrequencies)) {
      found = true;
      break;
    }
  }

  std::wcout << L"First Total " << total << std::endl;

  if (!found) {
    while (true) {
      for (const int& frequency : frequencies) {
        found = add_and_check_frequency(frequency, total, seenFrequencies);
        if (found) {
          break;
        }
      }

      if (found) {
        break;
      }
    }
  }

  return 0;
}

bool add_and_check_frequency(int frequency, int& total, std::set<int> &seenFrequencies) {
  total += frequency;
  if (seenFrequencies.end() != seenFrequencies.find(total)) {
    std::wcout << "Repeat frequency found: " << total << std::endl;
    return true;
  }

  seenFrequencies.insert(total);
  return false;
}
