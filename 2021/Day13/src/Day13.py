import argparse
from collections import namedtuple
import re

from position import Position # My own 2019 code!

def get_grid_and_folds(filename):
  '''Gets a grid of dots and collection of folds from the file.'''
  parsing_dots = True
  grid = dict()
  Fold = namedtuple('Fold', 'axis value')
  folds = list()
  pattern = re.compile(r'fold along (x|y)=(\d+)', re.IGNORECASE)
  with open(filename, 'r') as fh:
    for line in fh.readlines():
      line = line.strip()
      if parsing_dots:
        if len(line) == 0:
          parsing_dots = False
        else:
          (x, y) = map(int, line.split(','))
          pos = Position(x, y)
          grid[pos] = 1
      else:
        match = pattern.match(line)
        folds.append(Fold(match.group(1), int(match.group(2))))

  return (grid, folds)

def print_grid(grid):
  '''Prints a grid of dots.'''
  minx = min(grid.keys(), key=lambda p: p.x).x
  maxx = max(grid.keys(), key=lambda p: p.x).x
  miny = min(grid.keys(), key=lambda p: p.y).y
  maxy = max(grid.keys(), key=lambda p: p.y).y

  for y in range(miny, maxy + 1):
    for x in range(minx, maxx + 1):
      pos = Position(x, y)
      if pos in grid:
        print('#', end='')
      else:
        print(' ', end='')
    print('', flush=True)

def execute_fold(grid, fold):
  '''Folds the paper along a line.'''
  new_positions = []
  positions_to_delete = []
  for position, _ in grid.items():
    if fold.axis == 'x' and position.x > fold.value:
      positions_to_delete.append(position)
      new_x = fold.value - abs(fold.value - position.x)
      new_position = Position(new_x, position.y)
      new_positions.append(new_position)
    elif fold.axis == 'y' and position.y > fold.value:
      positions_to_delete.append(position)
      new_y = fold.value - abs(fold.value - position.y)
      new_position = Position(position.x, new_y)
      new_positions.append(new_position)

  for position in new_positions:
    grid[position] = 1

  for position in positions_to_delete:
    del grid[position]

def main():
  '''I hope it makes a swan!'''
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  (grid, folds) = get_grid_and_folds(args.filename)
  if args.part == 1:
    execute_fold(grid, folds[0])
    print(f'After 1 fold, number of dots: {len(grid)}')
  elif args.part == 2:
    for fold in folds:
      execute_fold(grid, fold)
    print_grid(grid)

if __name__ == "__main__":
  main()
