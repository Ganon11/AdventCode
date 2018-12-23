#include "pch.h"
#include "Cave.h"
#include <algorithm>
#include <functional>
#include <map>
#include <queue>
#include <set>
#include <vector>

namespace {
unsigned long long EXTRA_SPACE{ 1000 };
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

unsigned long long get_next_cost(const RegionType current_type,
  const RegionType next_type,
  const Equipment current_equipment,
  const unsigned long long cost_so_far) {

  if (current_type == next_type) {
    return cost_so_far + 1;
  }

  switch (current_type) {
  case ROCKY:
    switch (next_type) {
    case WET:
      switch (current_equipment) {
      case CLIMBING_GEAR:
        return cost_so_far + 1;
      }
      break;
    case NARROW:
      switch (current_equipment) {
      case TORCH:
        return cost_so_far + 1;
      }
      break;
    }
    break;
  case WET:
    switch (next_type) {
    case ROCKY:
      switch (current_equipment) {
      case CLIMBING_GEAR:
        return cost_so_far + 1;
      }
      break;
    case NARROW:
      switch (current_equipment) {
      case NEITHER:
        return cost_so_far + 1;
      }
      break;
    }
    break;
  case NARROW:
    switch (next_type) {
    case ROCKY:
      switch (current_equipment) {
      case TORCH:
        return cost_so_far + 1;
      }
      break;
    case WET:
      switch (current_equipment) {
      case NEITHER:
        return cost_so_far + 1;
      }
      break;
    }
    break;
  }

  return ::IMPOSSIBLE_TO_REACH;
}
}

unsigned long long Cave::cost_to_target() const {
  std::map<Explorer, unsigned long long> cost_so_far;
  advent_of_code::Position target{ m_target };

  auto cost_heuristic{ [&cost_so_far, &target](const Explorer& e1, const Explorer& e2) {
    unsigned long long cost1;
    auto found1{ cost_so_far.find(e1) };
    if (cost_so_far.end() == found1) {
      cost1 = IMPOSSIBLE_TO_REACH;
    } else {
      cost1 = found1->second;
    }

    unsigned long long cost2;
    auto found2{ cost_so_far.find(e2) };
    if (cost_so_far.end() == found2) {
      cost2 = IMPOSSIBLE_TO_REACH;
    } else {
      cost2 = found2->second;
    }

    if (cost1 > cost2) {
      return true;
    } else if (cost1 == cost2
      && target.distance_to(e1.get_position()) > target.distance_to(e2.get_position())) {
      return true;
    }

    return false;
  } };

  std::priority_queue<Explorer,
    std::vector<Explorer>,
    std::function<bool(Explorer, Explorer)>> frontier{ cost_heuristic };

  frontier.push(Explorer{});
  cost_so_far[Explorer{}] = 0;

  while (!frontier.empty()) {
    Explorer current{ frontier.top() };
    frontier.pop();
    advent_of_code::Position current_position{ current.get_position() };
    Equipment current_equipment{ current.get_currently_equipped() };
    unsigned long long current_cost{ cost_so_far.at(current) };

    // Made it to target with torch out!
    if (m_target == current_position && TORCH == current_equipment) {
      return current_cost;
    }

    // Still looking
    RegionType current_region{ m_cave.at(current_position) };

    // Add equipment swaps
    Explorer equipment_swap{ current_position, current_equipment };
    switch (current_region) {
    case ROCKY:
      if (CLIMBING_GEAR == current_equipment) {
        equipment_swap.set_equipment(TORCH);
      } else {
        equipment_swap.set_equipment(CLIMBING_GEAR);
      }
      break;
    case WET:
      if (CLIMBING_GEAR == current_equipment) {
        equipment_swap.set_equipment(NEITHER);
      } else {
        equipment_swap.set_equipment(CLIMBING_GEAR);
      }
      break;
    case NARROW:
      if (TORCH == current_equipment) {
        equipment_swap.set_equipment(NEITHER);
      } else {
        equipment_swap.set_equipment(TORCH);
      }
      break;
    }

    if (cost_so_far.end() == cost_so_far.find(equipment_swap)) {
      cost_so_far[equipment_swap] = current_cost + 7;
      frontier.push(equipment_swap);
    }

    for (auto next : current_position.get_adjacent_positions()) {
      if (next.m_x < 0 || next.m_y < 0) {
        // Impossible to dig through cave at these points.
        continue;
      }

      if (next.m_x > 1000 || next.m_y > 1000) {
        // I've gone too far?
        continue;
      }

      RegionType next_region{ m_cave.at(next) };
      unsigned long long new_cost{ ::get_next_cost(current_region, next_region, current_equipment, current_cost) };

      if (IMPOSSIBLE_TO_REACH == new_cost) {
        // Can't get to next area with current equipment.
        // Switching equipment is handled above.
        continue;
      }

      Explorer new_explorer{ next, current_equipment };

      auto seen_already{ cost_so_far.find(new_explorer) };
      if (cost_so_far.end() == seen_already || new_cost < seen_already->second) {
        cost_so_far[new_explorer] = new_cost;
        frontier.push(new_explorer);
      }
    }
  }

  return IMPOSSIBLE_TO_REACH;
}
