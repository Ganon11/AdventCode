import argparse
import re
from collections import defaultdict

class SimplePosition:
  '''A simple x,y coordinate'''
  def __init__(self, x, y):
    self.x = x
    self.y = y

  def __str__(self):
    return f'{self.x},{self.y}'

class SimpleLine:
  '''A simple line between two coordinates'''
  _LINE_PATTERN = re.compile(r'(\d+),(\d+) -> (\d+),(\d+)', re.IGNORECASE)

  def __init__(self, line):
    match = SimpleLine._LINE_PATTERN.search(line)
    if not match:
      raise 'Invalid format'

    x1 = int(match.group(1))
    y1 = int(match.group(2))
    x2 = int(match.group(3))
    y2 = int(match.group(4))

    if x1 == x2:
      if y1 <= y2:
        self.start = SimplePosition(x1, y1)
        self.end = SimplePosition(x2, y2)
      else:
        self.start = SimplePosition(x2, y2)
        self.end = SimplePosition(x1, y1)
    else:
      if x1 <= x2:
        self.start = SimplePosition(x1, y1)
        self.end = SimplePosition(x2, y2)
      else:
        self.start = SimplePosition(x2, y2)
        self.end = SimplePosition(x1, y1)

  def is_horizontal(self):
    '''Determines if this line is horizontal'''
    return self.start.y == self.end.y

  def is_vertical(self):
    '''Determines if this line is vertical'''
    return self.start.x == self.end.x

  def __str__(self):
    return f'({self.start.x}, {self.start.y}) -> ({self.end.x}, {self.end.y})'

def get_lines(filename):
  '''Parses a collection of lines from the given file.'''
  lines = list()
  with open(filename, 'r') as fh:
    for line in fh:
      lines.append(SimpleLine(line.strip()))
  return lines

def print_board(board, max_x, max_y):
  '''Prints a board of intersecting lines.'''
  for y in range(max_y + 1):
    for x in range(max_x + 1):
      point = f'{x},{y}'
      if board[point] == 0:
        print(' ', end='')
      else:
        print(board[point], end='')
    print('')

def get_simple_intersections(lines, ignore_diagonal=True):
  '''Determines which horizontal and vertical lines intersect'''
  board = defaultdict(int)
  max_x = -1
  max_y = -1
  for line in lines:
    max_x = max(line.start.x, line.end.x, max_x)
    max_y = max(line.start.y, line.end.y, max_y)

    if line.is_horizontal():
      y = line.start.y
      for x in range(line.start.x, line.end.x + 1):
        board[f'{x},{y}'] += 1
    elif line.is_vertical():
      x = line.start.x
      for y in range(line.start.y, line.end.y + 1):
        board[f'{x},{y}'] += 1
    else:
      if ignore_diagonal:
        continue
      # All diagonal lines are oriented from lower X to higher X
      x = line.start.x
      y = line.start.y
      while x <= line.end.x:
        board[f'{x},{y}'] += 1
        x += 1
        if y <= line.end.y:
          y += 1
        else:
          y -= 1

  total = 0
  for _, value in board.items():
    if value >= 2:
      total += 1
  return total

def main():
  '''Navigate the field of hydrothermal vents.'''
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  lines = get_lines(args.filename)
  result = get_simple_intersections(lines, args.part == 1)
  print(f'Total intersections: {result}')

if __name__ == "__main__":
  main()
