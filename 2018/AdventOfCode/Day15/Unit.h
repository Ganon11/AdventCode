#pragma once

#include "../AoCHelpers/Position.h"
#include <memory>

enum UnitType
{
  ELF,
  GOBLIN,
  NONE
};

extern unsigned int DEFAULT_ATTACK_POWER;
extern unsigned int ELF_ATTACK_POWER;
extern unsigned int GOBLIN_ATTACK_POWER;

class Unit;

typedef std::shared_ptr<Unit> UnitPtr;

class Unit
{
public:
  virtual ~Unit() = default;

  advent_of_code::Position get_position() const;
  unsigned int get_remaining_health() const;
  unsigned int get_attack_power() const;
  unsigned int get_id() const;
  bool is_dead() const;
  bool operator<(const Unit& other) const;
  bool operator==(const Unit& other) const;

  void move(const advent_of_code::Position& new_position);
  bool take_hit(const unsigned int damage);

  virtual UnitType get_type() const = 0;

  static void set_attack_power(unsigned int power);
  static UnitPtr make_shared(const advent_of_code::Position& position, const UnitType type);

protected:
  Unit(const advent_of_code::Position& position, const unsigned int attack_power = DEFAULT_ATTACK_POWER);

private:
  advent_of_code::Position m_position;
  unsigned int m_health;
  unsigned int m_attack_power;
  unsigned int m_id;

  static unsigned int s_next_id;
};

class Elf : public Unit
{
public:
  Elf(const advent_of_code::Position& position);
  virtual ~Elf() = default;
  virtual UnitType get_type() const;

  static void set_attack_power(unsigned int power);
};

class Goblin : public Unit
{
public:
  Goblin(const advent_of_code::Position& position);
  virtual ~Goblin() = default;
  virtual UnitType get_type() const;

  static void set_attack_power(unsigned int power);
};
