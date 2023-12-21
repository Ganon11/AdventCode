#include "counter.h"

Condition convert(const char ch)
{
  switch (ch)
  {
  case '.':
    return EMPTY;
  case '#':
    return SPRING;
  case '?':
  default:
    return UNKNOWN;
  }
}

Counter::Counter(const std::vector<Condition>& conditions, const std::vector<int>& groups)
  : m_conditions{ conditions },
    m_groups{ groups }
{
  m_required_space.resize(groups.size());
  int space = 0;
  for (int index = groups.size() - 1; index >= 0; --index)
  {
    space += groups[index];
    m_required_space[index] = space;
    ++space;
  }
}

unsigned long long Counter::count()
{
  return count(0, 0);
}

unsigned long long Counter::count(int index, int group)
{
  std::pair<int, int> precalculated{ std::make_pair(index, group) };
  if (!m_precalculated.contains(precalculated))
  {
    unsigned long long result = calculate(index, group);
    m_precalculated.insert(CacheType::value_type{ precalculated, result });
    return result;
  }

  return m_precalculated.at(precalculated);
}

bool Counter::remaining_could_be_empty(int index)
{
  for (; index < m_conditions.size(); ++index)
  {
    if (m_conditions[index] == SPRING)
    {
      return false;
    }
  }

  return true;
}

bool Counter::is_spring(int index)
{
  return 0 <= index && m_conditions.size() > index && m_conditions[index] == SPRING;
}

bool Counter::could_be_spring(int index, int length)
{
  if (is_spring(index + length))
  {
    return false;
  }

  int end_index = index + length;
  for (; index < end_index; ++index)
  {
    if (m_conditions[index] == EMPTY)
    {
      return false;
    }
  }

  return true;
}

unsigned long long Counter::calculate(int index, int group)
{
  if (m_groups.size() == group)
  {
    return remaining_could_be_empty(index) ? 1 : 0;
  }

  if (index + m_required_space[group] > m_conditions.size())
  {
    return 0;
  }

  unsigned long long sum{0};
  int group_value{ m_groups[group] };
  int next_index{ index + group_value + 1 };

  if (could_be_spring(index, group_value))
  {
    sum += count(next_index, group + 1);
  }

  if (m_conditions[index] != SPRING)
  {
    sum += count(index + 1, group);
  }

  return sum;
}
