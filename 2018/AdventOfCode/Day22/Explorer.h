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
  Explorer(const advent_of_code::Position& p = advent_of_code::ORIGIN, const Equipment equipped = TORCH);

  void set_position(const advent_of_code::Position& p);
  void set_equipment(const Equipment e);

  advent_of_code::Position get_position() const;
  Equipment get_currently_equipped() const;

  bool operator<(const Explorer& other) const;
  bool operator==(const Explorer& other) const;

private:
  advent_of_code::Position m_position;
  Equipment m_currently_equipped;
};
