#include "pch.h"
#include "Unit.h"

unsigned int DEFAULT_ATTACK_POWER = 3;
unsigned int ELF_ATTACK_POWER = DEFAULT_ATTACK_POWER;
unsigned int GOBLIN_ATTACK_POWER = DEFAULT_ATTACK_POWER;

Position Unit::get_position() const {
  return m_position;
}

unsigned int Unit::get_remaining_health() const {
   return m_health;
}

unsigned int Unit::get_attack_power() const {
   return m_attack_power;
}

unsigned int Unit::get_id() const {
   return m_id;
}

bool Unit::is_dead() const {
   return m_health == 0;
}

bool Unit::operator<(const Unit& other) const {
   return m_position < other.get_position();
}

bool Unit::operator==(const Unit& other) const {
   return m_id == other.get_id();
}

void Unit::move(const Position& new_position) {
  m_position = new_position;
}

bool Unit::take_hit(const unsigned int damage) {
  if (damage >= m_health) {
    m_health = 0;
    return false;
  }

  m_health -= damage;
  return true;
}

void Unit::set_attack_power(unsigned int power) {
   DEFAULT_ATTACK_POWER = power;
}

UnitPtr Unit::make_shared(const Position& position, const UnitType type) {
   switch (type) {
   case ELF:
      return std::make_shared<Elf>(position);
   case GOBLIN:
      return std::make_shared<Goblin>(position);
   }

   return std::shared_ptr<Unit>();
}

Unit::Unit(const Position& position, const unsigned int attack_power)
   : m_position{ position },
     m_health{ 200 },
     m_attack_power{ attack_power },
     m_id{ s_next_id++ }
{}

unsigned int Unit::s_next_id = 1;

Elf::Elf(const Position& position) : Unit(position, ELF_ATTACK_POWER)
{}

UnitType Elf::get_type() const {
   return ELF;
}

void Elf::set_attack_power(unsigned int power) {
   ELF_ATTACK_POWER = power;
}

Goblin::Goblin(const Position& position) : Unit(position)
{}

UnitType Goblin::get_type() const {
   return GOBLIN;
}

void Goblin::set_attack_power(unsigned int power) {
   GOBLIN_ATTACK_POWER = power;
}
