import argparse
import math
import re
from position import Position

def get_target_area(filename):
  '''Gets the target area from the file.'''
  with open(filename, 'r') as fh:
    line = fh.read().strip()

  match = re.match(r'target area: x=(-?\d+)..(-?\d+), y=(-?\d+)..(-?\d+)', line, re.IGNORECASE)
  x1 = int(match.group(1))
  x2 = int(match.group(2))
  y1 = int(match.group(3))
  y2 = int(match.group(4))

  return (Position(min(x1, x2), max(y1, y2)), Position(max(x1, x2), min(y1, y2)))

def position_is_in_target_area(position, target_tl, target_br):
  ''' Determines if the given position is within the target area.'''
  return target_tl.x <= position.x and position.x <= target_br.x and \
    target_br.y <= position.y and position.y <= target_tl.y

def lands_in_target_area(x_velocity, y_velocity, target_tl, target_br):
  '''Determines if a shot lands in the target area, and returns the max height of the shot.'''
  position = Position(0, 0)
  max_height = None
  hits_target = False
  while position.x < target_br.x and position.y > target_br.y:
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

  # Stealing a bit of math from reddit to determine the search space

  # With a given initial x_velocity of x_v0, we travel x_v0 + (x_v0 -1) + (x_v0 - 2) + ... + 1
  # steps to the right (since drag pushes the velocity to 0). This is equal to
  # (x_v0 * (x_v0 + 1)) / 2.
  # We want this to be greater than the minimum x - that is, we want
  # (x_v0 * (x_v0 + 1)) / 2 >= x_min
  # Re-arrange this and use the quadratic formula to find that the smallest x_velocity that could
  # possibly get us to the target area is
  # x_v >= (-1 +- sqrt(8 * xmin)) / 2
  min_x_velocity = int((math.sqrt(8 * target_tl.x) - 1) / 2)

  # We overshoot the target area if our x_velocity is greater than xmax.
  max_x_velocity = target_br.x + 1

  # We overshoot the target area if our y_velocity is lower than y_min.
  min_y_velocity = target_br.y

  # We start at y = 0. We will eventually fall back to y = 0, at which point our y_velocity will
  # equal the opposite of our initial y_velocity (i.e. velocities go:
  #   5, 4, 3, 2, 1, 0, -1, -2, -3, -4, -5 <- at this point we cross y=0 again)
  # We will overshoot if this velocity exceeds the target area.
  # Therefore, the max_y_velocity equals the opposite of the min_y target position.
  max_y_velocity = (-1 * target_br.y)

  for x_velocity in range(min_x_velocity, max_x_velocity):
    for y_velocity in range(min_y_velocity, max_y_velocity):
      (hits_target, height) = lands_in_target_area(x_velocity, y_velocity, target_tl, target_br)
      if hits_target:
        if max_height is None or max_height < height:
          max_height = height
        velocities.add(Position(x_velocity, y_velocity))
  return (max_height, velocities)

def main():
  '''I hate golf.'''
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample.txt')
  args = parser.parse_args()

  (target_tl, target_br) = get_target_area(args.filename)
  (max_height, velocities) = get_max_height(target_tl, target_br)
  print(f'Max Height: {max_height}')
  print(f'Number of velocities: {len(velocities)}')

if __name__ == "__main__":
  main()
