#include "pch.h"
#include "Explorer.h"

Explorer::Explorer(const advent_of_code::Position& p, const Equipment equipped, const unsigned long long cost_so_far)
  : m_position{ p },
  m_currently_equipped{ equipped },
  m_cost_so_far{ cost_so_far }
{}

advent_of_code::Position Explorer::get_position() const {
  return m_position;
}

Equipment Explorer::get_currently_equipped() const {
  return m_currently_equipped;
}

unsigned long long Explorer::get_cost_so_far() const {
  return m_cost_so_far;
}

bool Explorer::operator<(const Explorer& other) const {
  if (m_position < other.m_position) {
    return true;
  } else if (m_position == other.m_position && m_cost_so_far < other.m_cost_so_far) {
    return true;
  }

  return false;
}

bool Explorer::operator==(const Explorer& other) const {
  return m_position == other.m_position && m_currently_equipped == other.m_currently_equipped;
}
