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

def position_is_in_target_area(position, top_left, bottom_right):
  ''' Determines if the given position is within the target area.'''
  return top_left.x <= position.x and position.x <= bottom_right.x and \
    bottom_right.y <= position.y and position.y <= top_left.y

def lands_in_target_area(v_x, v_y, top_left, bottom_right):
  '''Determines if a shot lands in the target area, and returns the max height of the shot.'''
  position = Position(0, 0)
  max_height = None
  hits_target = False
  while position.x < bottom_right.x and position.y > bottom_right.y:
    position.x += v_x # x position increases by v_x
    position.y += v_y # y position increases by v_y
    if max_height is None or position.y > max_height:
      max_height = position.y

    if position_is_in_target_area(position, top_left, bottom_right):
      hits_target = True
      break

    # v_x tends to 0 due to drag
    if v_x > 0:
      v_x -= 1
    elif v_x < 0:
      v_x += 1

    # v_y decreases due to gravity
    v_y -= 1

  return (hits_target, max_height)

def get_max_height(top_left, bottom_right):
  '''Gets the max height the probe can reach while ending up in the target area.'''
  max_height = None
  velocities = set()

  # Stealing a bit of math from reddit to determine the search space

  # With a given initial v_x of x_v0, we travel x_v0 + (x_v0 -1) + (x_v0 - 2) + ... + 1
  # steps to the right (since drag pushes the velocity to 0). This is equal to
  # (x_v0 * (x_v0 + 1)) / 2.
  # We want this to be greater than the minimum x - that is, we want
  # (x_v0 * (x_v0 + 1)) / 2 >= x_min
  # Re-arrange this and use the quadratic formula to find that the smallest v_x that could
  # possibly get us to the target area is
  # x_v >= (-1 +- sqrt(8 * xmin)) / 2
  min_v_x = int((math.sqrt(8 * top_left.x) - 1) / 2)

  # We overshoot the target area if our v_x is greater than xmax.
  max_v_x = bottom_right.x + 1

  # We overshoot the target area if our v_y is lower than y_min.
  min_v_y = bottom_right.y

  # We start at y = 0. We will eventually fall back to y = 0, at which point our v_y will
  # equal the opposite of our initial v_y (i.e. velocities go:
  #   5, 4, 3, 2, 1, 0, -1, -2, -3, -4, -5 <- at this point we cross y=0 again)
  # We will overshoot if this velocity exceeds the target area.
  # Therefore, the max_v_y equals the opposite of the min_y target position.
  max_v_y = (-1 * bottom_right.y)

  for v_x in range(min_v_x, max_v_x):
    for v_y in range(min_v_y, max_v_y):
      (hits_target, height) = lands_in_target_area(v_x, v_y, top_left, bottom_right)
      if hits_target:
        if max_height is None or max_height < height:
          max_height = height
        velocities.add(Position(v_x, v_y))
  return (max_height, velocities)

def main():
  '''I hate golf.'''
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample.txt')
  args = parser.parse_args()

  (top_left, bottom_right) = get_target_area(args.filename)
  (max_height, velocities) = get_max_height(top_left, bottom_right)
  print(f'Max Height: {max_height}')
  print(f'Number of velocities: {len(velocities)}')

if __name__ == "__main__":
  main()
