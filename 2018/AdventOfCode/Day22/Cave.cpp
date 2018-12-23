#include "pch.h"
#include "Cave.h"
#include <functional>
#include <queue>
#include <set>
#include <vector>

namespace {
unsigned long long EXTRA_SPACE{ 100 };
}

Cave::Cave(const unsigned long long cave_depth, const advent_of_code::Position& target)
  : m_target{ target } {

  std::vector<std::vector<unsigned long long>> erosion_levels;
  long long max_x = target.m_x + EXTRA_SPACE;
  long long max_y = target.m_y + EXTRA_SPACE;
  erosion_levels.reserve(static_cast<size_t>(max_y + 1));

  for (auto y = 0; y <= max_y; ++y) {
    std::vector<unsigned long long> new_row;
    new_row.reserve(static_cast<size_t>(max_x + 1));

    for (auto x = 0; x <= max_x; ++x) {
      advent_of_code::Position current{ x, y };
      // Find geologic index
      unsigned long long geologic_index;
      if (x == 0 && y == 0) {
        geologic_index = 0;
      } else if (current == target) {
        geologic_index = 0;
      } else if (x == 0) {
        geologic_index = y * 48271;
      } else if (y == 0) {
        geologic_index = x * 16807;
      } else {
        geologic_index = erosion_levels[y - 1][x] * new_row[x - 1];
      }

      unsigned long long erosion_level{ (geologic_index + cave_depth) % 20183 };
      m_cave[current] = static_cast<RegionType>(erosion_level % 3);
      new_row.push_back(erosion_level);
    }

    erosion_levels.push_back(new_row);
  }
}

unsigned long long Cave::get_risk_level() const {
  unsigned long long risk_level{ 0 };
  for (auto y = 0; y <= m_target.m_y; ++y) {
    for (auto x = 0; x <= m_target.m_x; ++x) {
      risk_level += static_cast<unsigned long long>(m_cave.at(advent_of_code::Position{ x, y }));
    }
  }

  return risk_level;
}

namespace {
const auto IMPOSSIBLE_TO_REACH{ std::numeric_limits<unsigned long long>::max() };

void get_next_cost(const RegionType current_type,
  const RegionType next_type,
  const Equipment current_equipment,
  const unsigned long long cost_so_far,
  unsigned long long& new_cost,
  Equipment& new_equipment) {

  if (current_type == next_type) {
    new_cost = cost_so_far + 1;
    new_equipment = current_equipment;
    return;
  }

  switch (current_type) {
  case ROCKY:
    if (next_type == NARROW) {
      if (current_equipment == TORCH) {
        new_cost = cost_so_far + 1;
        new_equipment = current_equipment;
        return;
      } else if (current_equipment == CLIMBING_GEAR) {
        new_cost = cost_so_far + 7;
        new_equipment = TORCH;
        return;
      }
    } else if (next_type == WET) {
      if (current_equipment == TORCH) {
        new_cost = cost_so_far + 7;
        new_equipment = CLIMBING_GEAR;
        return;
      } else if (current_equipment == CLIMBING_GEAR) {
        new_cost = cost_so_far + 1;
        new_equipment = current_equipment;
        return;
      }
    }
    break;
  case WET:
    if (next_type == NARROW) {
      if (current_equipment == CLIMBING_GEAR) {
        new_cost = cost_so_far + 7;
        new_equipment = NEITHER;
        return;
      } else if (current_equipment == NEITHER) {
        new_cost = cost_so_far + 1;
        new_equipment = current_equipment;
        return;
      }
    } else if (next_type == ROCKY) {
      if (current_equipment == CLIMBING_GEAR) {
        new_cost = cost_so_far + 1;
        new_equipment = current_equipment;
        return;
      } else if (current_equipment == NEITHER) {
        new_cost = cost_so_far + 7;
        new_equipment = CLIMBING_GEAR;
        return;
      }
    }
    break;
  case NARROW:
    if (next_type == WET) {
      if (current_equipment == TORCH) {
        new_cost = cost_so_far + 7;
        new_equipment = NEITHER;
        return;
      } else if (current_equipment == NEITHER) {
        new_cost = cost_so_far + 1;
        new_equipment = current_equipment;
        return;
      }
    } else if (next_type == ROCKY) {
      if (current_equipment == TORCH) {
        new_cost = cost_so_far + 1;
        new_equipment = current_equipment;
        return;
      } else if (current_equipment == NEITHER) {
        new_cost = cost_so_far + 7;
        new_equipment = TORCH;
        return;
      }
    }
    break;
  }

  new_cost = ::IMPOSSIBLE_TO_REACH;
  return;
}
}

unsigned long long Cave::cost_to_target() const {
  auto cost_heuristic{ [](const Explorer& e1, const Explorer& e2) {
    return e1.get_cost_so_far() > e2.get_cost_so_far();
  } };


  std::priority_queue<Explorer,
    std::vector<Explorer>,
    std::function<bool(Explorer, Explorer)>> frontier{ cost_heuristic };

  frontier.push(Explorer{});
  std::set<Explorer> seen;
  seen.insert(Explorer{});

  while (!frontier.empty()) {
    Explorer current{ frontier.top() };
    frontier.pop();
    advent_of_code::Position current_position{ current.get_position() };

    // Made it to target - remember to switch to the torch!
    if (m_target == current_position) {
      if (TORCH == current.get_currently_equipped()) {
        return current.get_cost_so_far();
      } else {
        return current.get_cost_so_far() + 7;
      }
    }

    // Still looking
    RegionType current_region{ m_cave.at(current_position) };
    for (auto next : current_position.get_adjacent_positions()) {
      if (next.m_x < 0 || next.m_y < 0) {
        // Impossible to dig through cave at these points.
        continue;
      }

      RegionType next_region{ m_cave.at(next) };

      Equipment next_equipment;
      unsigned long long new_cost;
      ::get_next_cost(current_region,
        next_region,
        current.get_currently_equipped(),
        current.get_cost_so_far(),
        new_cost,
        next_equipment);

      Explorer new_state{ next, next_equipment, new_cost };
      auto seen_already{ seen.find(new_state) };
      if (seen.end() == seen.find(new_state)) {
        seen.insert(new_state);
        frontier.push(new_state);
      } else if (new_cost < seen_already->get_cost_so_far()) {
        seen.erase(seen_already);
        seen.insert(new_state);
        frontier.push(new_state);
      }
    }
  }

  return IMPOSSIBLE_TO_REACH;
}
