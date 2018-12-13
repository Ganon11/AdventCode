#pragma once

#include <map>
#include <string>

typedef std::map<int, bool> PlantMap;

class PlantRule
{
public:
  PlantRule();
  PlantRule(const std::wstring& line);

  bool matches(const PlantMap& plants, const int index) const;
  bool result() const;

private:
  wchar_t m_pattern[5];
  bool m_result;
};
