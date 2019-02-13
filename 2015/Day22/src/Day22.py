import argparse
import copy
import itertools
import sys

class Target:
  def __init__(self, name, h, d):
    self.name = name
    self.health = h
    self.damage = d

  def take_hit(self, damage):
    self.health = self.health - damage

  def is_dead(self):
    return self.health <= 0

  def __str__(self):
    return "%s (health %d, damage %d)" % (self.name, self.health, self.damage)

class Player(Target):
  def __init__(self, name, h, m):
    Target.__init__(self, name, h, 0)
    self.mana = m
    self.armor = 0

  def __str__(self):
    return "%s (health %d, mana %d)" % (self.name, self.health, self.mana)

class InstantEffect():
  def __init__(self, d, h):
    self.damage = d
    self.heal = h

  def perform(self, player, boss):
    player.health += self.heal
    boss.take_hit(self.damage)

class PersistentEffect():
  def __init__(self, id, duration, armor, damage, mana):
    self._id = id
    self.duration = duration
    self.armor = armor
    self.damage = damage
    self.mana = mana
    self.turns_left = duration

  def reset(self):
    return PersistentEffect(self._id, self.duration, self.armor, self.damage, self.mana)

  def __hash__(self):
    return self._id

  def __eq__(self, other):
    return self._id == other._id

  def perform(self, player, boss):
    if self.turns_left == self.duration:
      # First turn active
      player.armor += self.armor

    boss.take_hit(self.damage)

    player.mana += self.mana

    self.turns_left -= 1
    if self.turns_left == 0:
      player.armor -= self.armor

EMPTY_PERSISTENT_EFFECT = PersistentEffect(-1, 0, 0, 0, 0)

class Spell():
  def __init__(self, name, cost, persEffect, instEffect):
    self.name = name
    self.cost = cost
    self.persistent_effect = persEffect
    self.instant_effect = instEffect

  def cast_spell(self, player, boss, currentEffects):
    player.mana -= self.cost
    if player.mana < 0 or self.persistent_effect in currentEffects:
      return False
    if self.instant_effect is not None:
      self.instant_effect.perform(player, boss)
    if self.persistent_effect != EMPTY_PERSISTENT_EFFECT:
      currentEffects.add(self.persistent_effect)
    return True

def total_mana(spells):
  return sum([sp.cost for sp in spells])

def use_effects(effects, player, boss):
  for effect in effects:
    effect.perform(player, boss)
  return set([effect for effect in effects if effect.turns_left > 0])

def simulate_battle(spell_order, b, p):
    spell_order = [Spell(s.name, s.cost, s.persistent_effect.reset(), s.instant_effect) for s in spell_order]
    boss = copy.deepcopy(b)
    player = copy.deepcopy(p)
    effects = set()
    sp = 0
    for sp in range(len(spell_order)):
        player.health -= 1
        if player.health <= 0:
            return -3
        effects = use_effects(effects, player, boss)
        if boss.health <= 0:
            return total_mana(spell_order[:sp])
        if not spell_order[sp].cast_spell(player, boss, effects):
            return -2
        effects = use_effects(effects, player, boss)
        if boss.health <= 0:
            return total_mana(spell_order[:sp+1])
        player.health -= max(1, boss.damage - player.armor)
        if player.health <= 0:
            return -3
    return -1


spells = [
    Spell('Magic Missile', 53, EMPTY_PERSISTENT_EFFECT, InstantEffect(4, 0)),
    Spell('Drain', 73, EMPTY_PERSISTENT_EFFECT, InstantEffect(2, 2)),
    Spell('Shield', 113, PersistentEffect(1, 6, 7, 0, 0), None),
    Spell('Poison', 173, PersistentEffect(2, 6, 0, 3, 0), None),
    Spell('Recharge', 229, PersistentEffect(3, 5, 0, 0, 101), None)
]

def get_stats(filename):
  h = 0
  d = 0
  with open(filename, 'r') as fp:
    for line in fp.readlines():
      parts = line.split()
      if parts[0] == 'Hit':
        h = int(parts[-1])
      elif parts[0] == 'Damage:':
        d = int(parts[-1])
      else:
        print("Unknown first value %s" % parts[0])
  return h, d

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', type=str)
  parser.add_argument('-ph', '--player-health', type=int, default=50)
  parser.add_argument('-pm', '--player-mana', type=int, default=500)
  parser.add_argument('-bh', '--boss-health', type=int, default=12)
  parser.add_argument('-bd', '--boss-damage', type=int, default=7)
  args = parser.parse_args()

  player = Player('Player', args.player_health, args.player_mana)

  (bh, bd) = (args.boss_health, args.boss_damage)
  if args.filename:
    (bh, bd) = get_stats(args.filename)
  boss = Target('Boss', bh, bd)

  sp_perm = [[]]
  while True:
      n_sp = []
      for comb in sp_perm:
          for s in spells:
              n_sp.append(comb + [s])
      sp_perm = n_sp
      mn = -1
      next_sp_perm = []
      for sp in sp_perm:
          v = simulate_battle(sp, boss, player)
          if v == -1:
             next_sp_perm.append(sp)
             continue
          if v == -2 or v == -3:
             continue
          if mn == -1 or v < mn:
              mn = v
      if mn != -1:
          break
      sp_perm = next_sp_perm
  print(mn)

if __name__ == "__main__":
  main()
