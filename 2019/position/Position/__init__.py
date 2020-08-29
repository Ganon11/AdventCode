"""Represents simple points in 3D space."""

import re

class Position:
  """A simple 2D point."""
  def __init__(self, x=0, y=0, z=0):
    self.x = x
    self.y = y
    self.z = z

  def distance(self, other=None):
    """
    Returns the Manhattan Distance (change in X plus change in Y plus change in Z) from this point
    to other.

    If other is not provided, the origin (0, 0, 0) is used.
    """
    if other is None:
      other = Position()
    if not self._is_valid_operand(other):
      return NotImplemented
    return abs(self.x - other.x) + abs(self.y - other.y) + abs(self.z - other.z)

  def is_adjacent_to(self, other):
    """
    Returns True if this point is one step (not counting diagonals) from other,
    and False otherwise
    """
    if not self._is_valid_operand(other):
      return NotImplemented

    if self.x == other.x and self.y == other.y and abs(self.z - other.z) == 1:
      return True

    if self.x == other.x and abs(self.y - other.y) == 1 and self.z == other.z:
      return True

    if abs(self.x - other.x) == 1 and self.y == other.y and self.z == other.z:
      return True

    return False

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

  def get_adjacent_positions(self):
    """Returns a list of adjacent positions."""
    return [self.north(), self.west(), self.east(), self.south(), self.up(), self.down()]

  @staticmethod
  def _is_valid_operand(other):
    return hasattr(other, "x") and hasattr(other, "y") and hasattr(other, "z")

  def __hash__(self):
    return hash(str(self))

  def __str__(self):
    return f'({self.x}, {self.y}, {self.z})'

  def __lt__(self, other):
    if not Position._is_valid_operand(other):
      return NotImplemented

    if self.z < other.z:
      return True

    if self.z == other.z and self.y < other.y:
      return True

    if self.z == other.z and self.y == other.y and self.x < other.x:
      return True

    return False

  def __le__(self, other):
    if not Position._is_valid_operand(other):
      return NotImplemented

    if self.z <= other.z:
      return True

    if self.z == other.z and self.y <= other.y:
      return True

    if self.z == other.z and self.y == other.y and self.x <= other.x:
      return True

    return False

  def __eq__(self, other):
    if not Position._is_valid_operand(other):
      return NotImplemented

    return self.x == other.x and self.y == other.y and self.z == other.z

  def __gt__(self, other):
    if not Position._is_valid_operand(other):
      return NotImplemented

    if self.z > other.z:
      return True

    if self.z == other.z and self.y > other.y:
      return True

    if self.z == other.z and self.y == other.y and self.x > other.x:
      return True

    return False

  def __ge__(self, other):
    if not Position._is_valid_operand(other):
      return NotImplemented

    if self.z >= other.z:
      return True

    if self.z == other.z and self.y >= other.y:
      return True

    if self.z == other.z and self.y == other.y and self.x >= other.x:
      return True

    return False

  def __ne__(self, other):
    if not Position._is_valid_operand(other):
      return NotImplemented

    return self.x != other.x or self.y != other.y or self.z != other.z

  def __add__(self, other):
    return Position(self.x + other.x, self.y + other.y, self.z + other.z)

  def __radd__(self, other):
    return Position(self.x + other.x, self.y + other.y, self.z + other.z)

  def __repr__(self):
    return str(self)


def walk_path(moves, start=Position()):
  """Returns a list of positions resulting from a set of moves

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
      for _ in range(0, count):
        new_position = position.north()
        visited.append(new_position)
        position = new_position
    elif direction == 'D':
      for _ in range(0, count):
        new_position = position.south()
        visited.append(new_position)
        position = new_position
    elif direction == 'L':
      for _ in range(0, count):
        new_position = position.west()
        visited.append(new_position)
        position = new_position
    elif direction == 'R':
      for _ in range(0, count):
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
  return Position(int(matches[1]), int(matches[2]), int(matches[3]))
