import argparse
import copy
import itertools
import sys

class Target:
  def __init__(self, name, h, d, a):
    self.name = name
    self.health = h
    self.damage = d
    self.armor = a

  def take_hit(self, damage):
    actual = damage - self.armor
    if actual < 1:
      actual = 1

    self.health = self.health - actual

  def is_dead(self):
    return self.health <= 0

  def print(self):
    print("%s (health %d, damage %d, armor %d)" % (self.name, self.health, self.damage, self.armor))

def get_stats(filename):
  h = 0
  d = 0
  a = 0
  with open(filename, 'r') as fp:
    for line in fp.readlines():
      parts = line.split()
      if parts[0] == 'Hit':
        h = int(parts[-1])
      elif parts[0] == 'Damage:':
        d = int(parts[-1])
      elif parts[0] == 'Armor:':
        a = int(parts[-1])
      else:
        print("Unknown first value %s" % parts[0])

  return h, d, a

def simulate_combat(player, boss):
  while True:
    boss.take_hit(player.damage)
    if boss.is_dead():
      return True

    player.take_hit(boss.damage)
    if player.is_dead():
      return False

weapons = [
  [8, 4, 0],
  [10, 5, 0],
  [25, 6, 0],
  [40, 7, 0],
  [74, 8, 0]
]

armor = [
  [0, 0, 0],
  [13, 0, 1],
  [31, 0, 2],
  [53, 0, 3],
  [75, 0, 4],
  [102, 0, 5]
]

rings = [
  [0, 0, 0],
  [0, 0, 0],
  [25, 1, 0],
  [50, 2, 0],
  [100, 3, 0],
  [20, 0, 1],
  [40, 0, 2],
  [80, 0, 3]
]

def part_1(player, boss):
  mincost = 1e100
  for w in weapons:
    for a in armor:
      for r1, r2 in itertools.combinations(rings, 2):
        p = copy.deepcopy(player)
        b = copy.deepcopy(boss)
        cost = w[0] + a[0] + r1[0] + r2[0]
        p.damage = w[1] + a[1] + r1[1] + r2[1]
        p.armor = w[2] + a[2] + r1[2] + r2[2]
        if simulate_combat(p, b) and cost < mincost:
          mincost = cost
          print("Player wins with cost %d" % cost)

def part_2(player, boss):
  maxcost = 0
  for w in weapons:
    for a in armor:
      for r1, r2 in itertools.combinations(rings, 2):
        p = copy.deepcopy(player)
        b = copy.deepcopy(boss)
        cost = w[0] + a[0] + r1[0] + r2[0]
        p.damage = w[1] + a[1] + r1[1] + r2[1]
        p.armor = w[2] + a[2] + r1[2] + r2[2]
        if not simulate_combat(p, b) and cost > maxcost:
          maxcost = cost
          print("Boss wins with cost %d" % cost)

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', type=str)
  parser.add_argument('-ph', '--player-health', type=int, default=100)
  parser.add_argument('-pd', '--player-damage', type=int, default=0)
  parser.add_argument('-pa', '--player-armor', type=int, default=0)
  parser.add_argument('-bh', '--boss-health', type=int, default=12)
  parser.add_argument('-bd', '--boss-damage', type=int, default=7)
  parser.add_argument('-ba', '--boss-armor', type=int, default=2)
  args = parser.parse_args()

  player = Target('Player', args.player_health, args.player_damage, args.player_armor)
  player.print()

  (bh, bd, ba) = (args.boss_health, args.boss_damage, args.boss_armor)
  if args.filename:
    (bh, bd, ba) = get_stats(args.filename)
  boss = Target('Boss', bh, bd, ba)
  boss.print()

  part_1(player, boss)
  part_2(player, boss)

if __name__ == "__main__":
  main()
