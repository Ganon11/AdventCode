import argparse
import os

class Position:
  def __init__(self, x, y):
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

def walk_path(moves):
  p = Position(0, 0)
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

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample1.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  text = list()
  with open(args.filename, 'r') as f:
    text = f.readlines()

  path1 = walk_path(text[0].split(','))
  path2 = walk_path(text[1].split(','))

  path1set = {p for p in path1 if p.x != 0 and p.y != 0}
  path2set = {p for p in path2 if p.x != 0 and p.y != 0}
  intersections = path1set.intersection(path2set)
  if args.part == 1:
    intersections = sorted(intersections, key=lambda p: p.DistanceToOrigin())
    print(f'Closest intersection distance: {intersections[0].DistanceToOrigin()}')
  elif args.part == 2:
    intersections = sorted(intersections, key=lambda p: path1.index(p) + path2.index(p))
    print(f'Best step count: {path1.index(intersections[0]) + path2.index(intersections[0]) + 2}')
  else:
    print(f'Unrecognized part {args.part}')

if __name__ == "__main__":
  main()