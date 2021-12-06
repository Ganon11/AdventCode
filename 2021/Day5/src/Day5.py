import argparse
import re
from collections import defaultdict

class SimpleLine:
  '''A simple line between two coordinates'''
  _LINE_PATTERN = re.compile(r'(\d+),(\d+) -> (\d+),(\d+)', re.IGNORECASE)

  def __init__(self, line):
    match = SimpleLine._LINE_PATTERN.search(line)
    if not match:
      raise 'Invalid format'

    self.x1 = int(match.group(1))
    self.y1 = int(match.group(2))
    self.x2 = int(match.group(3))
    self.y2 = int(match.group(4))

  def is_horizontal(self):
    '''Determines if this line is horizontal'''
    return self.y1 == self.y2

  def is_vertical(self):
    '''Determines if this line is vertical'''
    return self.x1 == self.x2

  def __str__(self):
    return f'({self.x1}, {self.y1}) -> ({self.x2}, {self.y2})'

def get_lines(filename):
  '''Parses a collection of lines from the given file.'''
  lines = list()
  with open(filename, 'r') as fh:
    for line in fh:
      lines.append(SimpleLine(line.strip()))
  return lines

def get_intersections(lines, ignore_diagonal=True):
  '''Determines which lines intersect'''
  board = defaultdict(int)
  for line in lines:
    if line.is_horizontal():
      y = line.y1
      for x in range(min(line.x1, line.x2), max(line.x1, line.x2) + 1):
        board[f'{x},{y}'] += 1
    elif line.is_vertical():
      x = line.x1
      for y in range(min(line.y1, line.y2), max(line.y1, line.y2) + 1):
        board[f'{x},{y}'] += 1
    else:
      if ignore_diagonal:
        continue

      count_points = abs(line.x1 - line.x2) + 1
      x = line.x1
      y = line.y1
      for _ in range(count_points):
        board[f'{x},{y}'] += 1

        if x < line.x2:
          x += 1
        else:
          x -= 1

        if y < line.y2:
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
  result = get_intersections(lines, args.part == 1)
  print(f'Total intersections: {result}')

if __name__ == "__main__":
  main()
