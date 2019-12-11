import argparse

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
    if self.direction == 'R':
      return self.slope > other.slope

    return self.slope < other.slope

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

def print_field(field, rows, cols): # pylint: disable=C0116
  for row in range(0, rows):
    for col in range(0, cols):
      if field[Position(col, row)]:
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

def get_angles_to_asteroids(position, asteroids): # pylint: disable=C0116
  angles = set()
  for asteroid in asteroids:
    if asteroid == position:
      continue

    if asteroid.x == position.x:
      if asteroid.y < position.y:
        angles.add(Angle(special='UP'))
      else:
        angles.add(Angle(special='DOWN'))
      continue

    if asteroid.y == position.y:
      if asteroid.x < position.x:
        angles.add(Angle(slope=0, direction='L'))
      else:
        angles.add(Angle(slope=0, direction='R'))
      continue
    deltay = asteroid.y - position.y
    deltax = asteroid.x - position.x
    angle = deltay / deltax
    direction = None
    if deltax > 0:
      direction = 'L'
    else:
      direction = 'R'
    angles.add(Angle(slope=angle, direction=direction))

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
  int_number = int(number)
  delta = number - int_number
  return delta < 0.0001

def get_first_asteroid_in_direction(position, angle, field, rows, cols): # pylint: disable=C0116
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
    deltay = -1 * angle.slope
    if angle.direction == 'R':
      deltax = 1
    elif angle.direction == 'L':
      deltax = -1

  print(f'Angle {angle} resulted in deltax {deltax} deltay {deltay}')
  while True:
    x += deltax
    y += deltay
    if x < 0 or cols <= x:
      return None
    if y < 0 or rows <= y:
      return None
    if not close_to_integer(y):
      continue
    possible_asteroid = Position(x, int(y))
    if field[possible_asteroid]:
      return possible_asteroid
  return None

def run_laser(field, station, rows, cols): # pylint: disable=C0116
  asteroids_destroyed = 0
  while asteroids_destroyed < 200:
    new_count = asteroids_destroyed
    asteroids = [k for k, v in field.items() if field[k]]
    angles = list(get_angles_to_asteroids(station, asteroids))
    angles.sort()
    for angle in angles:
      asteroid = get_first_asteroid_in_direction(station, angle, field, rows, cols)
      if asteroid is None:
        print(f'Didn\'t find asteroid with station {station}, angle {angle}')
        print_field(field, rows, cols)
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
  print_field(field, rows, cols)

  (best, count) = place_monitoring_station(field)
  print(f'Best is {best} with {count} other asteroids detected')
  if args.part == 2:
    # Do part 2!
    run_laser(field, best, rows, cols)

    # Below is driver code for the example given in Part 2
    # best = Position(8, 3)
    # asteroids = [k for k, v in field.items() if field[k]]
    # angles = list(get_angles_to_asteroids(best, asteroids))
    # angles.sort()
    # print(f'Found angles {angles}')
    # for index in range(0, 10):
    #   print(f'Index {index}, angle {angles[index]}')
    #   asteroid = get_first_asteroid_in_direction(best, angles[index], field, rows, cols)
    #   print(f'Found asteroid at {asteroid}')

if __name__ == "__main__":
  main()
