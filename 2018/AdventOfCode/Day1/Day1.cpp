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

using namespace std;

bool add_and_check_frequency(int frequency, int& total, set<int> &seenFrequencies);

int main() {
  AdventOfCode::InputHandler input;
  int total{ 0 };
  vector<int> frequencies;
  set<int> seenFrequencies;
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

  wcout << L"First Total " << total << endl;

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

bool add_and_check_frequency(int frequency, int& total, set<int> &seenFrequencies) {
  total += frequency;
  if (seenFrequencies.end() != seenFrequencies.find(total)) {
    wcout << "Repeat frequency found: " << total << endl;
    return true;
  }

  seenFrequencies.insert(total);
  return false;
}
