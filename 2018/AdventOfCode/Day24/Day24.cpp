// Day24.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <vector>

#include "../AoCHelpers/InputHandler.h"
#include "AttackGroup.h"

typedef std::map<unsigned short, unsigned short> TargetAssignment;
typedef std::vector<AttackGroup> AttackGroups;

AttackGroups get_attack_groups(const advent_of_code::InputHandler& input, ull boost = 0);
Team simulate_combat(AttackGroups& attack_groups);
bool victory_achieved(const AttackGroups& groups, Team& victorious_team);
unsigned short select_target(const AttackGroup& g, const AttackGroups& all_groups, const std::set<unsigned short> groups_being_attacked);
TargetAssignment target_selection(AttackGroups& attack_groups);
void attack_phase(AttackGroups& groups, const TargetAssignment& targets);
ull unit_sum(const AttackGroups& groups);

int wmain(int argc, wchar_t* argv[]) {
  advent_of_code::InputHandler input{ argc, argv };
  ull boost{ 0 };
  ull boost_step = 64;
  std::wstring boost_string;
  if (input.get_argument(L"-boost", boost_string)) {
    boost = std::stoull(boost_string);
    boost_step = 1;
  }

  Team winning_team;
  ull winning_unit_sum{ 0 };
  do {
    AttackGroups groups{ get_attack_groups(input, boost) };

    winning_team = simulate_combat(groups);
    winning_unit_sum = unit_sum(groups);

    std::wcout << L"With a boost of " << boost
      << L", the " << team_to_string(winning_team)
      << L" team wins with " << winning_unit_sum
      << L" units remaining." << std::endl;

    if (winning_team != IMMUNE_SYSTEM) {
      boost += boost_step;
    } else if (1 == boost_step) {
      std::wcout << L"Answer found!" << std::endl;
      break;
    } else {
      std::wcout << L"Boost of " << boost << L" was enough." << std::endl;
      std::wcout << L"Stepping down from " << boost_step << L" to " << boost_step / 2 << std::endl;
      boost -= boost_step;
      boost_step /= 2;
    }
  } while (true);

  return 0;
}

AttackGroups get_attack_groups(const advent_of_code::InputHandler& input, ull boost) {
  ull current_boost{ 0 };
  AttackGroups groups;
  Team current_team;
  for (const std::wstring& line : input.read_all_lines()) {
    if (line.empty()) {
      continue;
    }

    if (0 == _wcsicmp(L"Immune System:", line.c_str())) {
      current_team = IMMUNE_SYSTEM;
      current_boost = boost;
    } else if (0 == _wcsicmp(L"Infection:", line.c_str())) {
      current_team = INFECTION;
      current_boost = 0;
    } else {
      groups.push_back(AttackGroup{ line, current_team, current_boost });
    }
  }

  return groups;
}

Team simulate_combat(AttackGroups& attack_groups) {
  Team victorious_team{ NO_TEAM };
  ull old_unit_sum{ unit_sum(attack_groups) };
  do {
    TargetAssignment targets{ target_selection(attack_groups) };
    attack_phase(attack_groups, targets);
    ull new_unit_sum{ unit_sum(attack_groups) };
    if (new_unit_sum == old_unit_sum) {
      // Stalemate detected!
      return NO_TEAM;
    }

    old_unit_sum = new_unit_sum;
  } while (!victory_achieved(attack_groups, victorious_team));

  return victorious_team;
}

bool victory_achieved(const AttackGroups& groups, Team& victorious_team) {
  bool immune_alive{ false };
  bool infection_alive{ false };

  for (const AttackGroup& g : groups) {
    if (g.is_dead()) {
      continue;
    }

    Team t{ g.get_team() };
    if (IMMUNE_SYSTEM == t) {
      immune_alive = true;
    } else if (INFECTION == t) {
      infection_alive = true;
    }

    if (immune_alive && infection_alive) {
      return false;
    }
  }

  if (immune_alive) {
    victorious_team = IMMUNE_SYSTEM;
    return true;
  }

  if (infection_alive) {
    victorious_team = INFECTION;
    return true;
  }

  std::wcerr << L"IT'S A TIE!" << std::endl;
  return true;
}

unsigned short select_target(const AttackGroup& g, const AttackGroups& all_groups, const std::set<unsigned short> groups_being_attacked) {
  ull effective_damage{ 0 };
  ull enemy_effective_power{ 0 };
  ull enemy_initiative{ 0 };
  unsigned short target_id{ AttackGroup::NO_TARGET };
  for (const AttackGroup& target_group : all_groups) {
    // Skip ourselves
    if (g.get_id() == target_group.get_id()) {
      continue;
    }

    // Skip allies
    if (g.get_team() == target_group.get_team()) {
      continue;
    }

    // Skip dead targets
    if (target_group.is_dead()) {
      continue;
    }

    // Skip groups already targeted
    if (groups_being_attacked.end() != groups_being_attacked.find(target_group.get_id())) {
      continue;
    }

    ull estimated_damage{ target_group.estimate_total_damage(g.get_damage_amount(), g.get_damage_type()) };

    if (effective_damage < estimated_damage
      || (effective_damage == estimated_damage && enemy_effective_power < target_group.effective_power())
      || (effective_damage == estimated_damage && enemy_effective_power == target_group.effective_power() && enemy_initiative < target_group.get_initiative())) {

      effective_damage = estimated_damage;
      enemy_effective_power = target_group.effective_power();
      enemy_initiative = target_group.get_initiative();
      target_id = target_group.get_id();
    }
  }

  if (effective_damage > 0) {
    return target_id;
  }

  return AttackGroup::NO_TARGET;
}

TargetAssignment target_selection(AttackGroups& attack_groups) {
  TargetAssignment targets;
  std::set<unsigned short> groups_being_attacked;
  auto attack_group_comparator{ [](const AttackGroup& g1, const AttackGroup& g2){
    if (g1.effective_power() > g2.effective_power()) {
      return true;
    }

    if (g1.effective_power() == g2.effective_power() && g1.get_initiative() > g2.get_initiative()) {
      return true;
    }

    return false;
  } };

  // Immune system groups select target infection groups
  std::sort(attack_groups.begin(), attack_groups.end(), attack_group_comparator);
  for (const AttackGroup& group : attack_groups) {
    if (group.is_dead()) {
      continue;
    }

    unsigned short target_id{ select_target(group, attack_groups, groups_being_attacked) };
    if (AttackGroup::NO_TARGET != target_id) {
      groups_being_attacked.insert(target_id);
      targets[group.get_id()] = target_id;
    }
  }

  return targets;
}

void attack_phase(AttackGroups& groups, const TargetAssignment& targets) {
  std::sort(groups.begin(), groups.end(), [](const AttackGroup g1, const AttackGroup g2) {
    return g1.get_initiative() > g2.get_initiative();
  });

  for (const AttackGroup& g : groups) {
    if (g.is_dead()) {
      continue;
    }

    auto target_id{ targets.find(g.get_id()) };
    if (targets.end() == target_id) {
      continue;
    }

    auto target_finder{ [&target_id](const AttackGroup& t) {
      return target_id->second == t.get_id();
    } };
    auto target{ std::find_if(groups.begin(), groups.end(), target_finder) };
    if (groups.end() == target) {
      std::wcerr << L"Couldn't find target group" << std::endl;
      continue;
    }

    target->take_damage(g.effective_power(), g.get_damage_type());
  }
}

ull unit_sum(const AttackGroups& groups) {
  ull sum{ 0 };
  for (const AttackGroup& g : groups) {
    sum += g.get_remaining_units();
  }

  return sum;
}
