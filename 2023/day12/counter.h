#pragma once

#include <map>
#include <vector>

enum Condition
{
  EMPTY,
  SPRING,
  UNKNOWN
};

Condition convert(const char ch);

class Counter
{
public:
  Counter(const std::vector<Condition>& conditions, const std::vector<int>& groups);

  unsigned long long count();

private:
  unsigned long long count(int index, int group);

  bool remaining_could_be_empty(int index);
  bool is_spring(int index);
  bool could_be_spring(int index, int length);
  unsigned long long calculate(int index, int group);

private:
  std::vector<Condition> m_conditions;
  std::vector<int> m_groups;
  std::vector<int> m_required_space;

  typedef std::map<std::pair<int, int>, unsigned long long> CacheType;
  CacheType m_precalculated;
};
