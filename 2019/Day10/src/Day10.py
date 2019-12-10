import argparse

from position import Position

def get_asteroid_field(filename):
  field = dict()
  with open(filename, 'r') as file:
    lines = [line.rstrip('\n') for line in file]
    rows = len(lines)
    cols = len(lines[0])
    for row in range(0, rows):
      for col in range(0, cols):
        p = Position(col, row)
        if lines[row][col] == '#':
          field[p] = True
        else:
          field[p] = False

  return(rows, cols, field)

def print_field(field, rows, cols):
  for row in range(0, rows):
    for col in range(0, cols):
      p = Position(col, row)
      if field[p]:
        print('#', end='')
      else:
        print('.', end='')
    print('')

def greatest_common_denominator(a, b):
  if b == 0:
    return a
  return greatest_common_denominator(b, a % b)

def in_field(x, y, rows, cols):
  if x < 0 or rows < x:
    return False
  if y < 0 or cols < y:
    return False
  return True

def count_visible_asteroids(field, asteroids, position, rows, cols):
  angles = set()
  #print(f'checking {position}')
  for asteroid in asteroids:
    if asteroid == position:
      continue

    if asteroid.x == position.x:
      if asteroid.y < position.y:
        angles.add('UP')
      else:
        angles.add('DOWN')
      continue

    if asteroid.y == position.y:
      if asteroid.x < position.x:
        angles.add('LEFT')
      else:
        angles.add('RIGHT')
      continue
    deltay = asteroid.y - position.y
    deltax = asteroid.x - position.x
    angle = deltay / deltax
    direction = None
    if deltax > 0:
      direction = 'L'
    else:
      direction = 'R'
    #if angle not in angles:
      #print(f'  adding {asteroid} at angle {angle}')
    angles.add(str(angle) + direction)

  #angle_list = list(angles)
  #angle_list.sort()
  #print(angle_list)
  return len(angles)

def place_monitoring_station(field, rows, cols):
  asteroids = [k for k, v in field.items() if field[k]]
  max_asteroids = 0
  best_position = None
  for possible_position in asteroids:
    score = count_visible_asteroids(field, asteroids, possible_position, rows, cols)
    if score > max_asteroids:
      max_asteroids = score
      best_position = possible_position

  return (best_position, max_asteroids)

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample1.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  (rows, cols, field) = get_asteroid_field(args.filename)
  #print_field(field, rows, cols)

  if args.part == 1:
    (best, count) = place_monitoring_station(field, rows, cols)
    print(f'Best is {best} with {count} other asteroids detected')
    #print(f'Count(1, 1) = {count_visible_asteroids(field, asteroids, Position(1, 1), 3, 3)}')
    #print(f'(5, 8) is asteroid? {Position(5, 8) in asteroids}')
    #print(f'Count(5, 8) = {count_visible_asteroids(field, asteroids, Position(5, 8), 10, 10)}')
    #print(f'(0, 1) is asteroid? {Position(0, 1) in asteroids}')
    #print(f'Count(0, 1) = {count_visible_asteroids(field, asteroids, Position(0, 1), 10, 10)}')

if __name__ == "__main__":
  main()
