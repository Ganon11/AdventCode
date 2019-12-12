import argparse
import functools
import math
import position

class Moon:
  def __init__(self, name, initial_position):
    self.name = name
    self.position = initial_position
    self.velocity = position.Position()

  def apply_gravity(self, moons):
    """Applies gravity from other moons to this moon. Skips itself."""
    for moon in moons:
      if moon.name == self.name:
        continue

      if self.position.x < moon.position.x:
        self.velocity.x += 1
      elif self.position.x > moon.position.x:
        self.velocity.x -= 1

      if self.position.y < moon.position.y:
        self.velocity.y += 1
      elif self.position.y > moon.position.y:
        self.velocity.y -= 1

      if self.position.z < moon.position.z:
        self.velocity.z += 1
      elif self.position.z > moon.position.z:
        self.velocity.z -= 1

  def move(self):
    """Moves by applying velocity to the position"""
    self.position = self.position + self.velocity

  def potential_energy(self):
    return abs(self.position.x) + abs(self.position.y) + abs(self.position.z)

  def kinetic_energy(self):
    return abs(self.velocity.x) + abs(self.velocity.y) + abs(self.velocity.z)

  def total_energy(self):
    return self.potential_energy() * self.kinetic_energy()

  def __str__(self):
    return f'{self.name} at {self.position} with velocity {self.velocity}'

  def __repr__(self):
    return str(self)

  def __hash__(self):
    return hash(str(self))

  def get_x_state(self):
    return (self.position.x, self.velocity.x)

  def get_y_state(self):
    return (self.position.y, self.velocity.y)

  def get_z_state(self):
    return (self.position.z, self.velocity.z)

def get_moons(filename):
  lines = list()
  with open(filename, 'r') as file:
    lines.extend(file.readlines())

  io = Moon('io', position.parse_string(lines[0]))
  europa = Moon('europa', position.parse_string(lines[1]))
  ganymede = Moon('ganymede', position.parse_string(lines[2]))
  callisto = Moon('callisto', position.parse_string(lines[3]))

  return [io, europa, ganymede, callisto]

def step(moons):
  for moon in moons:
    moon.apply_gravity(moons)

  for moon in moons:
    moon.move()

def all_velocities_are_zero(moons):
  for moon in moons:
    if moon.velocity.x != 0 or moon.velocity.y != 0 or moon.velocity.z != 0:
      return False

  return True

def least_common_multiple(numbers):
  return functools.reduce(lambda x, y: int((x * y) / math.gcd(x, y)), numbers, 1)

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample1.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  parser.add_argument('-s', '--steps', default=10, type=int)
  args = parser.parse_args()

  moons = get_moons(args.filename)
  if args.part == 1:
    for _ in range(0, args.steps):
      step(moons)

    total_energy = sum(map(lambda m: m.total_energy(), moons))
    print(f'Total energy: {total_energy}')
  elif args.part == 2:
    x_seen = set()
    x_seen.add(tuple(map(lambda m: m.get_x_state(), moons)))
    x_cycle = -1
    y_seen = set()
    y_seen.add(tuple(map(lambda m: m.get_y_state(), moons)))
    y_cycle = -1
    z_seen = set()
    z_seen.add(tuple(map(lambda m: m.get_z_state(), moons)))
    z_cycle = -1
    time = 1
    step(moons)
    while x_cycle == -1 or y_cycle == -1 or z_cycle == -1:
      if x_cycle == -1:
        x_state = tuple(map(lambda m: m.get_x_state(), moons))
        if x_state in x_seen:
          x_cycle = time
        x_seen.add(x_state)

      if y_cycle == -1:
        y_state = tuple(map(lambda m: m.get_y_state(), moons))
        if y_state in y_seen:
          y_cycle = time
        y_seen.add(y_state)

      if z_cycle == -1:
        z_state = tuple(map(lambda m: m.get_z_state(), moons))
        if z_state in z_seen:
          z_cycle = time
        z_seen.add(x_state)

      time += 1
      step(moons)

    print(f'xycle {x_cycle}, yycle {y_cycle}, zycle {z_cycle}, lcm is {least_common_multiple((x_cycle, y_cycle, z_cycle))}')

if __name__ == "__main__":
  main()
