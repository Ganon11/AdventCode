#include "pch.h"
#include "PlantRule.h"

#include <regex>

PlantRule::PlantRule() {}

PlantRule::PlantRule(const std::wstring& line) {
  std::wregex plant_rule_regex{ L"([#.]{5}) => ([#.])" };
  std::wsmatch matches;
  std::regex_search(line, matches, plant_rule_regex);
  if (matches.size() == 3) { // one for whole string, one for pattern, one for result
    for (size_t index = 0; index < 5; ++index) {
      m_pattern[index] = matches[1].str()[index];
    }

    m_result = matches[2].str()[0] == L'#';
  }
}

bool PlantRule::matches(const PlantMap& plants, const int index) const {
  bool ch1{ plants.end() == plants.find(index - 2) ? false : plants.at(index - 2) };
  bool ch2{ plants.end() == plants.find(index - 1) ? false : plants.at(index - 1) };
  bool ch3{ plants.at(index) };
  bool ch4{ plants.end() == plants.find(index + 1) ? false : plants.at(index + 1) };
  bool ch5{ plants.end() == plants.find(index + 2) ? false : plants.at(index + 2) };
  
  if (ch1 == (m_pattern[0] == L'#')
      && ch2 == (m_pattern[1] == L'#')
      && ch3 == (m_pattern[2] == L'#')
      && ch4 == (m_pattern[3] == L'#')
      && ch5 == (m_pattern[4] == L'#')) {
    return true;
  }

  return false;
}

bool PlantRule::result() const {
  return m_result;
}
