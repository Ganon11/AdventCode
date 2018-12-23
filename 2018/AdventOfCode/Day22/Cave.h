#pragma once

#include <map>

#include "../AoCHelpers/Position.h"
#include "Explorer.h"

enum RegionType
{
  ROCKY = 0,
  WET = 1,
  NARROW = 2
};

class Cave
{
public:
  Cave(const unsigned long long cave_depth, const advent_of_code::Position& target);

  unsigned long long get_risk_level() const;
  unsigned long long cost_to_target() const;

private:
  std::map<advent_of_code::Position, RegionType> m_cave;
  advent_of_code::Position m_target;
};
