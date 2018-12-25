#include "pch.h"
#include "AttackGroup.h"
#include <regex>

std::wstring team_to_string(const Team t) {
  switch (t) {
  case IMMUNE_SYSTEM:
    return L"Immune System";
  case INFECTION:
    return L"Infection";
  case NO_TEAM:
  default:
    return L"N/A";
  }
}

AttackGroup::AttackGroup(const std::wstring& line, const Team team, const ull boost)
  : m_id{ s_next_id++ }, m_team{ team } {
  std::wregex attack_group_regex{ L"(\\d+) units each with (\\d+) hit points (?:\\((.+)\\) )?with an attack that does (\\d+) (\\w+) damage at initiative (\\d+)" };
  std::wsmatch matches;
  std::regex_search(line, matches, attack_group_regex);

  m_units = std::stoull(matches[1].str());
  m_hit_points_per_unit = std::stoull(matches[2].str());
  if (matches[3].matched) {
    parse_resistances(matches[3].str());
  }
  m_damage_amount = std::stoull(matches[4].str()) + boost;
  m_damage_type = get_damage_type(matches[5].str());
  m_initiative = std::stoull(matches[6].str());

  if (IMMUNE_SYSTEM == team) {
    m_team_id = s_next_immune_id++;
  } else if (INFECTION == team) {
    m_team_id = s_next_infection_id++;
  }
}

unsigned short AttackGroup::get_id() const {
  return m_id;
}

Team AttackGroup::get_team() const {
  return m_team;
}

ull AttackGroup::get_remaining_units() const {
  return m_units;
}

ull AttackGroup::effective_power() const {
  return m_units * m_damage_amount;
}

ull AttackGroup::get_damage_amount() const {
  return m_damage_amount;
}

DamageType AttackGroup::get_damage_type() const {
  return m_damage_type;
}

ull AttackGroup::estimate_total_damage(const ull damage_amount, const DamageType damage_type) const {
  switch (get_damage_resistance_type(damage_type)) {
  case IMMUNE:
    return 0;
  case WEAK:
    return 2 * damage_amount;
  case NORMAL:
  default:
    return damage_amount;
  }
}

ResistanceType AttackGroup::get_damage_resistance_type(const DamageType damage_type) const {
  if (m_damage_map.end() == m_damage_map.find(damage_type)) {
    return NORMAL;
  } else {
    return m_damage_map.at(damage_type);
  }
}

ull AttackGroup::get_initiative() const {
  return m_initiative;
}

bool AttackGroup::is_dead() const {
  return m_units == 0;
}

void AttackGroup::take_damage(const ull damage_amount, const DamageType damage_type) {
  ull actual_damage{ estimate_total_damage(damage_amount, damage_type) };
  ull dead_units{ actual_damage / m_hit_points_per_unit };
  if (dead_units > m_units) {
    m_units = 0;
  } else {
    m_units -= dead_units;
  }
}

void AttackGroup::parse_resistances(const std::wstring& s) {
  std::wregex weak_regex{ L"weak to (\\w+)(?:, (\\w+))*" };
  std::wregex immune_regex{ L"immune to (\\w+)(?:, (\\w+))*" };
  std::wsmatch matches;

  if (std::regex_search(s, matches, weak_regex)) {
    for (size_t index = 1; index < matches.size(); ++index) {
      std::wstring damage_type{ matches[index].str() };
      if (!damage_type.empty()) {
        m_damage_map[get_damage_type(damage_type)] = WEAK;
      }
    }
  }

  if (std::regex_search(s, matches, immune_regex)) {
    for (size_t index = 1; index < matches.size(); ++index) {
      std::wstring damage_type{ matches[index].str() };
      if (!damage_type.empty()) {
        m_damage_map[get_damage_type(damage_type)] = IMMUNE;
      }
    }
  }
}

DamageType AttackGroup::get_damage_type(const std::wstring& s) {
  if (0 == _wcsicmp(L"bludgeoning", s.c_str())) {
    return BLUDGEONING;
  }

  if (0 == _wcsicmp(L"slashing", s.c_str())) {
    return SLASHING;
  }

  if (0 == _wcsicmp(L"radiation", s.c_str())) {
    return RADIATION;
  }

  if (0 == _wcsicmp(L"fire", s.c_str())) {
    return FIRE;
  }

  if (0 == _wcsicmp(L"cold", s.c_str())) {
    return COLD;
  }

  return NONE;
}

unsigned short AttackGroup::NO_TARGET = std::numeric_limits<unsigned short>::max();

unsigned short AttackGroup::s_next_id = 1;

unsigned short AttackGroup::s_next_immune_id = 1;

unsigned short AttackGroup::s_next_infection_id = 1;
