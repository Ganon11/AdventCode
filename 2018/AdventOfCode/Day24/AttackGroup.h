#pragma once

#include <map>
#include <string>

typedef unsigned long long ull;

enum DamageType
{
  NONE,
  BLUDGEONING,
  SLASHING,
  RADIATION,
  FIRE,
  COLD
};

enum ResistanceType
{
  NORMAL,
  WEAK,
  IMMUNE
};

enum Team
{
  IMMUNE_SYSTEM,
  INFECTION,
  NO_TEAM
};

std::wstring team_to_string(const Team t);

typedef std::map<DamageType, ResistanceType> DamageResistanceMap;

class AttackGroup
{
public:
  AttackGroup(const std::wstring& line, const Team team, const ull boost = 0);

  unsigned short get_id() const;
  Team get_team() const;
  ull get_remaining_units() const;
  ull effective_power() const;
  ull get_damage_amount() const;
  DamageType get_damage_type() const;
  ull estimate_total_damage(const ull damage_amount, const DamageType damage_type) const;
  ResistanceType get_damage_resistance_type(const DamageType damage_type) const;
  ull get_initiative() const;
  bool is_dead() const;

  void take_damage(const ull damage_amount, const DamageType damage_type);

  static unsigned short NO_TARGET;

private:
  void parse_resistances(const std::wstring& s);
  static DamageType get_damage_type(const std::wstring& s);

  unsigned short m_id;
  Team m_team;
  ull m_units;
  ull m_hit_points_per_unit;
  DamageResistanceMap m_damage_map;
  ull m_damage_amount;
  DamageType m_damage_type;
  ull m_initiative;
  unsigned short m_team_id;

  static unsigned short s_next_id;
  static unsigned short s_next_immune_id;
  static unsigned short s_next_infection_id;
};
