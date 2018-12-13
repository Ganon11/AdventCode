// Day12.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <vector>

#include "../AoCHelpers/InputHandler.h"
#include "PlantRule.h"

PlantMap get_initial_state(const std::wstring& line);
std::vector<PlantRule> get_rules(const std::vector<std::wstring>& lines);
PlantMap calculate_generation(const PlantMap& old_plants, const std::vector<PlantRule>& rules);
__int64 count_plants(const PlantMap& plants);

int wmain(int argc, wchar_t* argv[]) {
  advent_of_code::InputHandler input{ argc, argv };
  std::vector<std::wstring> lines{ input.read_all_lines() };

  // Initial state is in first line
  PlantMap plants{ get_initial_state(lines[0]) };
  std::vector<PlantRule> rules{ get_rules(lines) };

  const size_t MAX_NUM_GENERATIONS_TO_SIMULATE{ 300 };

  __int64 repeat_factor;
  __int64 plant_sum{ count_plants(plants) };
  std::vector<unsigned __int64> all_sums;
  all_sums.push_back(plant_sum);

  size_t generation;
  for (generation = 1; generation <= MAX_NUM_GENERATIONS_TO_SIMULATE; ++generation) {
    plants = calculate_generation(plants, rules);
    plant_sum = count_plants(plants);
    if (generation == 20) {
      std::wcout << generation << L"\t" << plant_sum << std::endl;
    }
    all_sums.push_back(plant_sum);

    // A pattern seems to emerge after enough generations - each successive generation adds a
    // constant number of plants. If we detect this pattern by checking the difference between the
    // last few pairs of generations, we can exit early and determine the final sum in one
    // calculation.
    if (generation > 4) {
      unsigned __int64 lastDifference{ all_sums[generation] - all_sums[generation - 1] };
      unsigned __int64 twoDifferencesAgo{ all_sums[generation - 1] - all_sums[generation - 2] };
      unsigned __int64 threeDifferencesAgo{ all_sums[generation - 2] - all_sums[generation - 3] };
      unsigned __int64 fourDifferencesAgo{ all_sums[generation - 3] - all_sums[generation - 4] };
      if (lastDifference == twoDifferencesAgo
          && twoDifferencesAgo == threeDifferencesAgo
          && threeDifferencesAgo == fourDifferencesAgo) {
        std::wcout << L"Pattern detected at generation " << generation << L"!" << std::endl;
        repeat_factor = all_sums[generation] - all_sums[generation - 1];
        break;
      }
    }
  }

  const unsigned __int64 TOTAL_GENERATIONS{ 50000000000 };
  plant_sum += (repeat_factor * (TOTAL_GENERATIONS - generation));
  std::wcout << TOTAL_GENERATIONS << L"\t" << plant_sum << std::endl;

  return 0;
}

PlantMap get_initial_state(const std::wstring& line) {
  PlantMap plants;
  std::wregex initial_state_regex{ L"initial state: ([#.]+)" };
  std::wsmatch matches;
  std::regex_search(line, matches, initial_state_regex);
  if (matches.size() != 2) {
    std::wcerr << L"Malformed input" << std::endl;
    return plants;
  }

  std::wstring plants_string{ matches[1].str() };
  for (size_t index = 0; index < plants_string.length(); ++index) {
    if (matches[1].str()[index] == L'#') {
      plants[index] = true;
    } else {
      plants[index] = false;
    }
  }

  // Add some non-plants on the edges
  plants[-2] = false;
  plants[-1] = false;
  plants[plants_string.length()] = false;
  plants[plants_string.length() + 1] = false;

  return plants;
}

std::vector<PlantRule> get_rules(const std::vector<std::wstring>& lines) {
  // First line is initial state
  // Second line is blank
  std::vector<PlantRule> rules;
  rules.reserve(lines.size() - 2);
  for (size_t index = 2; index < lines.size(); ++index) {
    rules.push_back(lines[index]);
  }

  return rules;
}

PlantMap calculate_generation(const PlantMap& old_plants, const std::vector<PlantRule>& rules) {
  PlantMap new_plants;
  auto plant_comparator{ [](const PlantMap::value_type& p1, const PlantMap::value_type& p2) {
      return p1.first < p2.first; } };

  const int min_index = std::min_element(old_plants.begin(), old_plants.end(),
    plant_comparator)->first;
  const int max_index = std::max_element(old_plants.begin(), old_plants.end(),
    plant_comparator)->first;

  for (int index = min_index; index <= max_index; ++index) {
    bool rule_matched{ false };
    for (const PlantRule& rule : rules) {
      if (rule.matches(old_plants, index)) {
        rule_matched = true;
        new_plants[index] = rule.result();
        break;
      }
    }

    if (!rule_matched) {
      new_plants[index] = false;
    }
  }

  // Add some non-plants on the edges
  new_plants[min_index - 2] = false;
  new_plants[min_index - 1] = false;
  new_plants[max_index + 1] = false;
  new_plants[max_index + 2] = false;

  return new_plants;
}

__int64 count_plants(const PlantMap& plants) {
  __int64 sum{ 0 };
  for (const PlantMap::value_type& plant : plants) {
    if (plant.second) {
      sum += plant.first;
    }
  }

  return sum;
}
