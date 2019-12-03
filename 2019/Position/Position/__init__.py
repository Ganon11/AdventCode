class Position:
  def __init__(self, x=0, y=0):
    self.x = x
    self.y = y

  def Distance(self, other=None):
    """
    Returns the Manhattan Distance (change in X plus change in Y) from this point to other.

    If other is not provided, the origin (0, 0) is used.
    """
    if other is None:
      other = Position()
    if not self._is_valid_operand(other):
      return NotImplemented
    return abs(self.x - other.x) + abs(self.y - other.y)

  def IsAdjacentTo(self, other):
    """Returns True if this point is one step (not counting diagonals) from other, and False otherwise"""
    if not self._is_valid_operand(other):
      return NotImplemented

    if self.x == other.x and 1 == abs(self.y - other.y):
      return True

    if 1 == abs(self.x - other.x) and self.y == other.y:
      return True

    return False

  def North(self):
    """Returns the Position one step to the north (positive Y)"""
    return Position(self.x, self.y + 1)

  def South(self):
    """Returns the Position one step to the south (negative Y)"""
    return Position(self.x, self.y - 1)

  def East(self):
    """Returns the Position one step to the east (positive X)"""
    return Position(self.x + 1, self.y)

  def West(self):
    """Returns the Position one step to the west (negative X)"""
    return Position(self.x - 1, self.y)

  def _is_valid_operand(self, other):
    return hasattr(other, "x") and hasattr(other, "y")

  def __hash__(self):
    return hash(str(self))

  def __str__(self):
    return f'{self.x}, {self.y}'

  def __lt__(self, other):
    if not self._is_valid_operand(other):
      return NotImplemented

    if self.y < other.y:
      return True

    if self.y == other.y and self.x < other.x:
      return True

    return False

  def __le__(self, other):
    if not self._is_valid_operand(other):
      return NotImplemented

    if self.y <= other.y:
      return True

    if self.y == other.y and self.x <= other.x:
      return True

    return False

  def __eq__(self, other):
    if not self._is_valid_operand(other):
      return NotImplemented

    return self.x == other.x and self.y == other.y

  def __gt__(self, other):
    if not self._is_valid_operand(other):
      return NotImplemented

    if self.y > other.y:
      return True

    if self.y == other.y and self.x > other.x:
      return True

    return False

  def __ge__(self, other):
    if not self._is_valid_operand(other):
      return NotImplemented

    if self.y >= other.y:
      return True

    if self.y == other.y and self.x >= other.x:
      return True

    return False

  def __ne__(self, other):
    if not self._is_valid_operand(other):
      return NotImplemented

    return self.x != other.x or self.y != other.y

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
  p = start
  visited = list()
  for m in moves:
    direction = m[0]
    count = int(m[1:])
    if direction == 'U':
      for i in range(0, count):
        newP = p.North()
        visited.append(newP)
        p = newP
    elif direction == 'D':
      for i in range(0, count):
        newP = p.South()
        visited.append(newP)
        p = newP
    elif direction == 'L':
      for i in range(0, count):
        newP = p.West()
        visited.append(newP)
        p = newP
    elif direction == 'R':
      for i in range(0, count):
        newP = p.East()
        visited.append(newP)
        p = newP
    else:
      print(f'Unknown direction {direction}')
  return visited