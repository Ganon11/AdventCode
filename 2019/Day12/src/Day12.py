import argparse
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
    pass

if __name__ == "__main__":
  main()
