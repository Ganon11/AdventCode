import argparse
import re
from position import Position

def get_target_area(filename):
  '''Gets the target area from the file.'''
  with open(filename, 'r') as fh:
    line = fh.read().strip()

  target_area_pattern = re.compile(r'target area: x=(-?\d+)..(-?\d+), y=(-?\d+)..(-?\d+)', re.IGNORECASE)
  match = target_area_pattern.match(line)
  x1 = int(match.group(1))
  x2 = int(match.group(2))
  y1 = int(match.group(3))
  y2 = int(match.group(4))

  return (Position(x1, y1), Position(x2, y2))

def position_is_in_target_area(position, target_tl, target_br):
  ''' Determines if the given position is within the target area.'''
  min_x = min(target_tl.x, target_br.x)
  max_x = max(target_tl.x, target_br.x)
  min_y = min(target_tl.y, target_br.y)
  max_y = max(target_tl.y, target_br.y)
  return min_x <= position.x and position.x <= max_x and \
    min_y <= position.y and position.y <= max_y

def lands_in_target_area(x_velocity, y_velocity, target_tl, target_br):
  '''Determines if a shot lands in the target area, and returns the max height of the shot.'''
  position = Position(0, 0)
  max_height = None
  hits_target = False
  min_x = min(target_tl.x, target_br.x)
  max_x = max(target_tl.x, target_br.x)
  min_y = min(target_tl.y, target_br.y)
  max_y = max(target_tl.y, target_br.y)
  while position.x < max_x and position.y > min_y:
    position.x += x_velocity # x position increases by x_velocity
    position.y += y_velocity # y position increases by y_velocity
    if max_height is None or position.y > max_height:
      max_height = position.y

    if position_is_in_target_area(position, target_tl, target_br):
      hits_target = True
      break

    # x_velocity tends to 0 due to drag
    if x_velocity > 0:
      x_velocity -= 1
    elif x_velocity < 0:
      x_velocity += 1

    # y_velocity decreases due to gravity
    y_velocity -= 1

  return (hits_target, max_height)

def get_max_height(target_tl, target_br):
  '''Gets the max height the probe can reach while ending up in the target area.'''
  max_height = None
  velocities = set()
  max_x_velocity = max(target_br.x, target_tl.x) + 1
  print(f'Xv 0..{max_x_velocity}')
  min_y_velocity = min(target_tl.y, target_br.y) - 1
  print(f'Yv {min_y_velocity}..200')
  for x_velocity in range(max_x_velocity):
    for y_velocity in range(min_y_velocity, 200):
      (hits_target, height) = lands_in_target_area(x_velocity, y_velocity, target_tl, target_br)
      if hits_target:
        if max_height is None or max_height < height:
          max_height = height
        velocities.add(Position(x_velocity, y_velocity))
  return (max_height, velocities)

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample.txt')
  args = parser.parse_args()

  (target_tl, target_br) = get_target_area(args.filename)
  (max_height, velocities) = get_max_height(target_tl, target_br)
  print(f'Max Height: {max_height}')
  print(f'Number of velocities: {len(velocities)}')

if __name__ == "__main__":
  main()
