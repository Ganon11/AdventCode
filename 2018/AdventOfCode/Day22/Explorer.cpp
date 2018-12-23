#include "pch.h"
#include "Explorer.h"

Explorer::Explorer(const advent_of_code::Position& p, const Equipment equipped)
  : m_currently_equipped{ equipped }
{
  m_position.m_x = p.m_x;
  m_position.m_y = p.m_y;
}

void Explorer::set_position(const advent_of_code::Position& p) {
  m_position.m_x = p.m_x;
  m_position.m_y = p.m_y;
}

void Explorer::set_equipment(const Equipment e) {
  m_currently_equipped = e;
}

advent_of_code::Position Explorer::get_position() const {
  return m_position;
}

Equipment Explorer::get_currently_equipped() const {
  return m_currently_equipped;
}

bool Explorer::operator<(const Explorer& other) const {
  if (m_position < other.m_position) {
    return true;
  } else if (m_position == other.m_position && m_currently_equipped < other.m_currently_equipped) {
    return true;
  }

  return false;
}

bool Explorer::operator==(const Explorer& other) const {
  return m_position == other.m_position && m_currently_equipped == other.m_currently_equipped;
}
