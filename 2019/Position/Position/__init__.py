class Position:
  def __init__(self, x=0, y=0):
    self.x = x
    self.y = y

  def ManhattanDistance(self, other):
    return abs(self.x - other.x) + abs(self.y - other.y)

  def DistanceToOrigin(self):
    return abs(self.x) + abs(self.y)

  def __hash__(self):
    return hash(str(self))

  def __eq__(self, other):
    return self.x == other.x and self.y == other.y

  def __str__(self):
    return f'{self.x}, {self.y}'

def walk_path(moves, start=Position()):
  """Returns a list of positions resulting from a set of moves

  A move is defined as a single character indicating direction,
  and an integer indicating a number of steps to walk in that
  direction.

  Acceptable directions are:
  U -- Up (a positive change in Y)
  D -- Down (a negative change in Y)
  L -- Left (a negative change in X)
  R -- Right (a positive change in X)
  """
  p = start
  visited = list()
  for m in moves:
    direction = m[0]
    count = int(m[1:])
    if direction == 'U':
      for i in range(0, count):
        newP = Position(p.x, p.y + 1)
        visited.append(newP)
        p = newP
    elif direction == 'D':
      for i in range(0, count):
        newP = Position(p.x, p.y - 1)
        visited.append(newP)
        p = newP
    elif direction == 'L':
      for i in range(0, count):
        newP = Position(p.x - 1, p.y)
        visited.append(newP)
        p = newP
    elif direction == 'R':
      for i in range(0, count):
        newP = Position(p.x + 1, p.y)
        visited.append(newP)
        p = newP
    else:
      print(f'Unknown direction {direction}')
  return visited