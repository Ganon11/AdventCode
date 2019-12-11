import argparse
import math

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

    # On the right side, use > when comparing slope
    # if self.direction == 'R':
    #   return self.slope > other.slope

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

def get_asteroid_field(filename): # pylint: disable=C0116
  field = dict()
  with open(filename, 'r') as file:
    lines = [line.rstrip('\n') for line in file]
    rows = len(lines)
    cols = len(lines[0])
    for row in range(0, rows):
      for col in range(0, cols):
        position = Position(col, row)
        if lines[row][col] == '#':
          field[position] = True
        else:
          field[position] = False

  return(rows, cols, field)

def print_field(field, rows, cols, station=None, target=None): # pylint: disable=C0116
  for row in range(0, rows):
    for col in range(0, cols):
      position = Position(col, row)
      if station is not None and station == position:
        print('*', end='')
      elif target is not None and target == position:
        print('X', end='')
      elif field[position]:
        print('#', end='')
      else:
        print('.', end='')
    print('')

def greatest_common_denominator(a, b): # pylint: disable=C0116,C0103
  if b == 0:
    return a
  return greatest_common_denominator(b, a % b)

def in_field(x, y, rows, cols): # pylint: disable=C0116
  if x < 0 or rows < x:
    return False
  if y < 0 or cols < y:
    return False
  return True

def get_angle_to_asteroid(position, asteroid): # pylint: disable=C0116
  # print('get_angle_to_asteroid')
  # print(f'  position {position}')
  # print(f'  asteroid {asteroid}')
  if asteroid.x == position.x:
    # print(f'    up or down')
    if asteroid.y < position.y:
      return Angle(special='UP')
    return Angle(special='DOWN')

  if asteroid.y == position.y:
    # print(f'    left or right')
    if asteroid.x < position.x:
      return Angle(slope=0, direction='L')
    return Angle(slope=0, direction='R')

  deltay = -1 * (asteroid.y - position.y)
  deltax = asteroid.x - position.x
  # print(f'  deltax {deltax} deltay {deltay}')
  angle = deltay / deltax
  direction = None
  if deltax > 0:
    direction = 'R'
  else:
    direction = 'L'

  return Angle(slope=angle, direction=direction)

def get_angles_to_asteroids(position, asteroids): # pylint: disable=C0116
  angles = set()
  for asteroid in asteroids:
    if asteroid == position:
      continue

    angles.add(get_angle_to_asteroid(position, asteroid))

  return angles

def place_monitoring_station(field): # pylint: disable=C0116
  asteroids = [k for k, v in field.items() if field[k]]
  max_asteroids = 0
  best_position = None
  for possible_position in asteroids:
    angles_with_visible_asteroids = get_angles_to_asteroids(possible_position, asteroids)
    score = len(angles_with_visible_asteroids)
    if score > max_asteroids:
      max_asteroids = score
      best_position = possible_position

  return (best_position, max_asteroids)

def close_to_integer(number): # pylint: disable=C0116
  return abs(number - round(number)) < 0.0001

def get_first_asteroid_in_direction(position, angle, field, rows, cols): # pylint: disable=C0116
  # print('get_first_asteroid_in_direction')
  # print(f'  position {position}')
  # print(f'  angle {angle}')
  # print(f'  rows {rows}')
  # print(f'  cols {cols}')
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

  # print(f'  deltax {deltax} deltay {deltay}')
  while True:
    x += deltax
    y += deltay
    # print(f'  x {x} y {y}')
    if x < 0 or cols <= x:
      return None
    if y < 0 or rows <= y:
      return None
    if not close_to_integer(y):
      continue
    possible_asteroid = Position(x, round(y))
    if field[possible_asteroid]:
      return possible_asteroid
  return None

def run_laser(field, station, rows, cols): # pylint: disable=C0116
  asteroids_destroyed = 0
  while asteroids_destroyed < 2:
    new_count = asteroids_destroyed
    asteroids = [k for k, v in field.items() if field[k]]
    angles = list(get_angles_to_asteroids(station, asteroids))
    angles.sort()
    for angle in angles:
      asteroid = get_first_asteroid_in_direction(station, angle, field, rows, cols)
      if asteroid is None:
        print(f'Didn\'t find asteroid with station {station}, angle {angle}')
      field[asteroid] = False
      new_count += 1
      if new_count == 200:
        return asteroid
    if new_count == asteroids_destroyed:
      print(f'No asteroids destroyed in loop! Total: {asteroids_destroyed}')
      return None
    asteroids_destroyed = new_count

def main(): # pylint: disable=C0116
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample1.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  (rows, cols, field) = get_asteroid_field(args.filename)

  (best, count) = place_monitoring_station(field)
  print(f'Best is {best} with {count} other asteroids detected')
  if args.part == 2:
    # Do part 2!
    two_hundredth_asteroid = run_laser(field, best, rows, cols)
    print(two_hundredth_asteroid)
    print_field(field, rows, cols, station=best)

if __name__ == "__main__":
  main()
