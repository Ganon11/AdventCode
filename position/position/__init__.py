"""Represents simple points in 3D space."""

import math
import re

class Position:
  """A simple 2D point."""
  def __init__(self, x=0, y=0, z=0):
    self.x = x
    self.y = y
    self.z = z

  @staticmethod
  def get_points_in_line(p1, p2): # pylint: disable=R0912
    '''Returns all the points between p1 and p2, inclusive.'''
    # Only handles horizontal, vertical, or diagonal lines
    good_line = False
    num_points = 0
    if p1 == p2:
      good_line = True
      num_points = 1
    elif p1.x == p2.x and p1.y != p2.y and p1.z == p2.z:
      good_line = True # Vertical lines are ok
      num_points = abs(p1.y - p2.y) + 1
    elif p1.x != p2.x and p1.y == p2.y and p1.z == p2.z:
      good_line = True # Horizontal lines are ok
      num_points = abs(p1.x - p2.x) + 1
    elif p1.x == p2.x and p1.y == p2.y and p1.z != p2.z:
      # "stacked" lines are ok
      # (https://english.stackexchange.com/questions/56472/x-y-z-horizontal-vertical-and)
      good_line = True
      num_points = abs(p1.z - p2.z) + 1
    # Now check two-dimensional diagonals
    elif p1.x == p2.x and abs(p1.y - p2.y) == abs(p1.z - p2.z):
      good_line = True
      num_points = abs(p1.y - p2.y) + 1
    elif p1.y == p2.y and abs(p1.x - p2.x) == abs(p1.z - p2.z):
      good_line = True
      num_points = abs(p1.z - p2.z) + 1
    elif p1.z == p2.z and abs(p1.x - p2.x) == abs(p1.y - p2.y):
      good_line = True
      num_points = abs(p1.x - p2.x) + 1
    # Now check three-dimensional diagonals
    elif abs(p1.x - p2.x) == abs(p1.y - p2.y) and abs(p1.y - p2.y) == abs(p1.z - p2.z):
      good_line = True
      num_points = abs(p1.x - p2.x) + 1

    if not good_line:
      return []

    points = list()
    x = p1.x
    y = p1.y
    z = p1.z
    for _ in range(num_points):
      points.append(Position(x, y, z))

      if x < p2.x:
        x += 1
      elif x > p2.x:
        x -= 1

      if y < p2.y:
        y += 1
      elif y > p2.y:
        y -= 1

      if z < p2.z:
        z += 1
      elif z > p2.z:
        z -= 1

    return points

  def distance(self, other=None):
    """
    Returns the Manhattan Distance (change in X plus change in Y plus change in Z) from this point
    to other.

    If other is not provided, the origin (0, 0, 0) is used.
    """
    if other is None:
      other = Position()

    return abs(self.x - other.x) + abs(self.y - other.y) + abs(self.z - other.z)

  def real_distance(self, other=None):
    """
    Returns the real distance from this point to other. If other is not provided, the origin is
    used.
    """
    if other is None:
      other = Position()

    delta_x = abs(self.x - other.x)
    delta_y = abs(self.y - other.y)
    delta_z = abs(self.z - other.z)
    return math.sqrt(delta_x ** 2 + delta_y ** 2 + delta_z ** 2)

  def is_adjacent_to(self, other, include_z=True, include_diagonals=False):
    """
    Returns True if this point is one step (not counting diagonals) from other,
    and False otherwise
    """
    return other in self.get_adjacent_positions(include_z, include_diagonals)

  def north(self):
    """Returns the Position one step to the north (positive Y)"""
    return Position(self.x, self.y + 1, self.z)

  def south(self):
    """Returns the Position one step to the south (negative Y)"""
    return Position(self.x, self.y - 1, self.z)

  def east(self):
    """Returns the Position one step to the east (positive X)"""
    return Position(self.x + 1, self.y, self.z)

  def west(self):
    """Returns the Position one step to the west (negative X)"""
    return Position(self.x - 1, self.y, self.z)

  def up(self):
    """Returns the Position one step up (positive Z)"""
    return Position(self.x, self.y, self.z + 1)

  def down(self):
    """Returns the Position one step down (negative Z)"""
    return Position(self.x, self.y, self.z - 1)

  def get_adjacent_positions(self, include_z=True, include_diagonals=False):
    """Returns a set of adjacent positions."""
    adjacent_positions = set([self.north(), self.west(), self.east(), self.south()])

    if include_z:
      adjacent_positions.update([self.up(), self.down()])

    if include_diagonals:
      adjacent_positions.update([self.north().west(), self.north().east(), \
        self.south().west(), self.south().east()])

    if include_z and include_diagonals:
      adjacent_positions.update([
        self.up().north().west(), self.up().north(), self.up().north().east(),
        self.up().west(), self.up().east(),
        self.up().south().west(), self.up().south(), self.up().south().east(),
        self.down().north().west(), self.down().north(), self.down().north().east(),
        self.down().west(), self.down().east(),
        self.down().south().west(), self.down().south(), self.down().south().east()
      ])

    return adjacent_positions

  def __hash__(self):
    return hash((self.x, self.y, self.z))

  def __str__(self):
    return f'({self.x}, {self.y}, {self.z})'

  def __lt__(self, other):
    if self.z < other.z:
      return True

    if self.z == other.z and self.y < other.y:
      return True

    if self.z == other.z and self.y == other.y and self.x < other.x:
      return True

    return False

  def __le__(self, other):
    if self.z < other.z:
      return True

    if self.z == other.z and self.y < other.y:
      return True

    if self.z == other.z and self.y == other.y and self.x < other.x:
      return True

    return self == other

  def __eq__(self, other):
    return self.x == other.x and self.y == other.y and self.z == other.z

  def __gt__(self, other):
    if self.z > other.z:
      return True

    if self.z == other.z and self.y > other.y:
      return True

    if self.z == other.z and self.y == other.y and self.x > other.x:
      return True

    return False

  def __ge__(self, other):
    if self.z > other.z:
      return True

    if self.z == other.z and self.y > other.y:
      return True

    if self.z == other.z and self.y == other.y and self.x > other.x:
      return True

    return self == other

  def __ne__(self, other):
    return self.x != other.x or self.y != other.y or self.z != other.z

  def __add__(self, other):
    return Position(self.x + other.x, self.y + other.y, self.z + other.z)

  def __radd__(self, other):
    return Position(self.x + other.x, self.y + other.y, self.z + other.z)

  def __repr__(self):
    return str(self)

def walk_path(moves, start=Position()):
  """
  Returns a list of positions resulting from a set of moves

  A move is defined as a single character indicating direction,
  and an integer indicating a number of steps to walk in that
  direction.

  Acceptable directions are:
  U -- Up (North)
  D -- Down (South)
  L -- Left (West)
  R -- Right (East)
  """
  position = start
  visited = list()
  visited.append(position)
  for move in moves:
    direction = move[0]
    count = int(move[1:])

    if direction == 'U':
      for _ in range(count):
        new_position = position.north()
        visited.append(new_position)
        position = new_position

    elif direction == 'D':
      for _ in range(count):
        new_position = position.south()
        visited.append(new_position)
        position = new_position

    elif direction == 'L':
      for _ in range(count):
        new_position = position.west()
        visited.append(new_position)
        position = new_position

    elif direction == 'R':
      for _ in range(count):
        new_position = position.east()
        visited.append(new_position)
        position = new_position

    else:
      print(f'Unknown direction {direction}')

  return visited

def parse_string(string):
  """Parses a string in the form '<x=X, y=Y, z=Z>' to a Position"""
  pattern = re.compile(r'<x=(-?\d+), y=(-?\d+), z=(-?\d+)>')
  matches = pattern.match(string)
  if not matches:
    raise ValueError('Incorrect format of argument string.')

  return Position(int(matches[1]), int(matches[2]), int(matches[3]))
