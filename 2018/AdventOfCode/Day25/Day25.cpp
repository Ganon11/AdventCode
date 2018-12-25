// Day25.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <regex>
#include <set>
#include <vector>

#include "../AoCHelpers/InputHandler.h"
#include "../AoCHelpers/Position.h"

typedef std::set<advent_of_code::Position> Constellation;

bool constellations_should_merge(const Constellation& c1, const Constellation& c2);

int wmain(int argc, wchar_t* argv[]) {
  advent_of_code::InputHandler input{ argc, argv };
  std::set<advent_of_code::Position> stars;
  std::vector<Constellation> constellations;
  std::wregex star_regex{ L"(-?\\d+),(-?\\d+),(-?\\d+),(-?\\d+)" };
  std::wsmatch matches;

  for (const std::wstring& line : input.read_all_lines()) {
    std::regex_search(line, matches, star_regex);
    long long x = std::stoll(matches[1].str());
    long long y = std::stoll(matches[2].str());
    long long z = std::stoll(matches[3].str());
    long long a = std::stoll(matches[4].str());

    Constellation new_constellation;
    new_constellation.insert(advent_of_code::Position{ x, y, z, a });
    constellations.push_back(new_constellation);
  }

  bool dirty;
  std::vector<Constellation>::iterator c1, c2;

  do {
    dirty = false;
    for (c1 = constellations.begin(); c1 != constellations.end(); ++c1) {
      for (c2 = c1 + 1; c2 != constellations.end(); ++c2) {
        if (constellations_should_merge(*c1, *c2)) {
          dirty = true;
          break;
        }
      }

      if (dirty) {
        break;
      }
    }

    if (dirty) {
      c1->insert(c2->begin(), c2->end());
      constellations.erase(c2);
    }
  } while(dirty);

  std::wcout << L"Merging complete - " << constellations.size() << L" constellations." << std::endl;

  return 0;
}

bool constellations_should_merge(const Constellation& c1, const Constellation& c2) {
  for (const auto& p1 : c1) {
    for (const auto& p2 : c2) {
      if (p1.distance_to(p2) <= 3) {
        return true;
      }
    }
  }

  return false;
}
