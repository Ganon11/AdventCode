import argparse
import re
from collections import defaultdict
from position import Position # My own 2019 code!

class SimpleLine:
  '''A simple line between two coordinates'''
  _LINE_PATTERN = re.compile(r'(\d+),(\d+) -> (\d+),(\d+)', re.IGNORECASE)

  def __init__(self, line):
    match = SimpleLine._LINE_PATTERN.search(line)
    if not match:
      raise 'Invalid format'

    self.p1 = Position(int(match.group(1)), int(match.group(2)))
    self.p2 = Position(int(match.group(3)), int(match.group(4)))

  def is_horizontal(self):
    '''Determines if this line is horizontal'''
    return self.p1.y == self.p2.y

  def is_vertical(self):
    '''Determines if this line is vertical'''
    return self.p1.x == self.p2.x

  def __str__(self):
    return f'({self.p1}) -> ({self.p2})'

def get_lines(filename):
  '''Parses a collection of lines from the given file.'''
  lines = list()
  with open(filename, 'r') as fh:
    for line in fh:
      lines.append(SimpleLine(line.strip()))
  return lines

def get_intersections(lines, ignore_diagonal):
  '''Determines which lines intersect'''
  board = defaultdict(int)
  for line in lines:
    if line.is_horizontal():
      y = line.p1.y
      for x in range(min(line.p1.x, line.p2.x), max(line.p1.x, line.p2.x) + 1):
        board[Position(x, y)] += 1
    elif line.is_vertical():
      x = line.p1.x
      for y in range(min(line.p1.y, line.p2.y), max(line.p1.y, line.p2.y) + 1):
        board[Position(x, y)] += 1
    else:
      if ignore_diagonal:
        continue

      count_points = abs(line.p1.x - line.p2.x) + 1
      x = line.p1.x
      y = line.p1.y
      for _ in range(count_points):
        board[Position(x, y)] += 1

        if x < line.p2.x:
          x += 1
        else:
          x -= 1

        if y < line.p2.y:
          y += 1
        else:
          y -= 1

  return len([value for value in board.values() if value >= 2])

def main():
  '''Navigate the field of hydrothermal vents.'''
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  lines = get_lines(args.filename)
  ignore_diagonal = True
  if args.part == 2:
    ignore_diagonal = False
  result = get_intersections(lines, ignore_diagonal)
  print(f'Total intersections: {result}')

if __name__ == "__main__":
  main()
