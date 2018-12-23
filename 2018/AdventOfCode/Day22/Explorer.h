#pragma once

#include "../AoCHelpers/Position.h"

enum Equipment
{
  CLIMBING_GEAR,
  TORCH,
  NEITHER
};

class Explorer
{
public:
  Explorer(const advent_of_code::Position& p = advent_of_code::ORIGIN,
    const Equipment equipped = TORCH,
    const unsigned long long cost_so_far = 0);

  advent_of_code::Position get_position() const;
  Equipment get_currently_equipped() const;
  unsigned long long get_cost_so_far() const;

  bool operator<(const Explorer& other) const;
  bool operator==(const Explorer& other) const;

private:
  advent_of_code::Position m_position;
  Equipment m_currently_equipped;
  unsigned long long m_cost_so_far;
};
