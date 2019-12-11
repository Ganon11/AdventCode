import argparse
import math
import os
import time

from position import Position

class Angle: # pylint: disable=C0115
  def __init__(self, slope=None, direction=None, special=None):
    self.slope = slope
    self.direction = direction
    self.special = special

  def __lt__(self, other): # pylint: disable=R0911
    if self == other:
      return False

    # UP is the lowest value possible
    if self.special == 'UP':
      return True

    if other.special == 'UP':
      return False

    # Handle DOWN
    if self.special == 'DOWN':
      if other.direction == 'R':
        return False
      if other.direction == 'L':
        return True
    if other.special == 'DOWN':
      if self.direction == 'R':
        return True
      if self.direction == 'L':
        return False

    # 'RIGHT' directions sort less than 'LEFT' directions
    if self.direction == 'R' and other.direction == 'L':
      return True

    if self.direction == 'L' and other.direction == 'R':
      return False

    return self.slope > other.slope

  def __eq__(self, other):
    return self.special == other.special and self.slope == other.slope and self.direction == other.direction # pylint: disable=C0301

  def __hash__(self):
    return hash(str(self))

  def __str__(self):
    if self.special is not None:
      return self.special

    return f'{self.slope}{self.direction}'

  def __repr__(self):
    return str(self)

class Field: # pylint: disable=C0115
  _CLEAR_SCREEN = lambda: os.system('cls' if os.name == 'nt' else 'clear')

  def __init__(self, filename):
    self.field = dict()
    self.station = None
    with open(filename, 'r') as file:
      lines = [line.rstrip('\n') for line in file]
      self.rows = len(lines)
      self.cols = len(lines[0])
      for row in range(0, self.rows):
        for col in range(0, self.cols):
          position = Position(col, row)
          if lines[row][col] == '#':
            self.field[position] = True
          else:
            self.field[position] = False

  def __str__(self):
    string = ''
    for row in range(0, self.rows):
      for col in range(0, self.cols):
        position = Position(col, row)
        if self.field[position]:
          string += '#'
        else:
          string += '.'
      string += '\n'
    return string

  def print(self, target=None): # pylint: disable=C0116
    Field._CLEAR_SCREEN()
    string = ''
    for row in range(0, self.rows):
      for col in range(0, self.cols):
        position = Position(col, row)
        if self.station is not None and self.station == position:
          string += '*'
        elif target is not None and target == position:
          string += 'X'
        elif self.field[position]:
          string += '#'
        else:
          string += '.'
      string += '\n'
    print(string)

  def in_field(self, x, y): # pylint: disable=C0116
    if x < 0 or self.rows < x:
      return False
    if y < 0 or self.cols < y:
      return False
    return True

  @staticmethod
  def _get_angle_to_asteroid(position, asteroid): # pylint: disable=C0116
    if asteroid.x == position.x:
      if asteroid.y < position.y:
        return Angle(special='UP')
      return Angle(special='DOWN')

    if asteroid.y == position.y:
      if asteroid.x < position.x:
        return Angle(slope=0, direction='L')
      return Angle(slope=0, direction='R')

    deltay = -1 * (asteroid.y - position.y)
    deltax = asteroid.x - position.x
    angle = deltay / deltax
    direction = None
    if deltax > 0:
      direction = 'R'
    else:
      direction = 'L'

    return Angle(slope=angle, direction=direction)

  @staticmethod
  def get_angles_to_asteroids(position, asteroids): # pylint: disable=C0116
    angles = set()
    for asteroid in asteroids:
      if asteroid == position:
        continue
      angles.add(Field._get_angle_to_asteroid(position, asteroid))
    return angles

  def place_monitoring_station(self): # pylint: disable=C0116
    asteroids = [k for k, v in self.field.items() if self.field[k]]
    max_asteroids = 0
    best_position = None
    for possible_position in asteroids:
      angles_with_visible_asteroids = Field.get_angles_to_asteroids(possible_position, asteroids)
      score = len(angles_with_visible_asteroids)
      if score > max_asteroids:
        max_asteroids = score
        best_position = possible_position

    self.station = best_position
    return max_asteroids

  @staticmethod
  def _close_to_integer(number): # pylint: disable=C0116
    return abs(number - round(number)) < 0.0001

  THRESHOLD = 0.0001
  @staticmethod
  def _closest_integer(number):
    floor = math.floor(number)
    if number - floor < Field.THRESHOLD:
      return floor

    ceil = math.ceil(number)
    if ceil - number < Field.THRESHOLD:
      return ceil

    return None

  def get_first_asteroid_in_direction(self, position, angle): # pylint: disable=C0116
    x = position.x
    y = float(position.y)

    deltax = None
    deltay = None

    if angle.special == 'UP':
      deltax = 0
      deltay = -1
    elif angle.special == 'DOWN':
      deltax = 0
      deltay = 1
    else:
      if angle.direction == 'R':
        deltax = 1
        deltay = -1 * angle.slope
      elif angle.direction == 'L':
        deltax = -1
        deltay = angle.slope

    while True:
      x += deltax
      y += deltay
      tempy = Field._closest_integer(y)
      if tempy is None:
        continue

      if not self.in_field(x, tempy):
        return None

      possible_asteroid = Position(x, tempy)
      if self.field[possible_asteroid]:
        return possible_asteroid
    return None

  def run_laser(self, target=200, debug_print=False): # pylint: disable=C0116,R0913
    asteroids_destroyed = 0
    while asteroids_destroyed < target:
      new_count = asteroids_destroyed
      asteroids = [k for k, v in self.field.items() if self.field[k]]
      angles = list(self.get_angles_to_asteroids(self.station, asteroids))
      angles.sort()
      for angle in angles:
        asteroid = self.get_first_asteroid_in_direction(self.station, angle)
        self.field[asteroid] = False
        new_count += 1
        if debug_print:
          print(new_count)
          self.print(target=asteroid)
          time.sleep(0.1)
        if new_count == target:
          return asteroid
      if new_count == asteroids_destroyed:
        print(f'No asteroids destroyed in loop! Total: {asteroids_destroyed}')
        return None
      asteroids_destroyed = new_count

def main(): # pylint: disable=C0116
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample1.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  parser.add_argument('-t', '--target', default=200, type=int)
  parser.add_argument('-d', '--debug', action='store_true')
  args = parser.parse_args()

  field = Field(args.filename)

  count = field.place_monitoring_station()
  print(f'Best is {field.station} with {count} other asteroids detected')
  field.print()
  if args.part == 2:
    # Do part 2!
    answer = field.run_laser(args.target, args.debug)
    print(answer)

if __name__ == "__main__":
  main()
