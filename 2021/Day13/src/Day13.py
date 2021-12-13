import argparse
import re

from position import Position # My own 2019 code!

def get_grid_and_folds(filename):
  '''Gets a grid of dots and collection of folds from the file.'''
  lines = []
  with open(filename, 'r') as fh:
    lines = list(map(lambda line: line.strip(), fh.readlines()))

  parsing_dots = True
  index = 0
  grid = dict()
  folds = list()
  pattern = re.compile(r'fold along (x|y)=(\d+)', re.IGNORECASE)
  while index < len(lines):
    line = lines[index]
    if parsing_dots:
      if len(line) == 0:
        parsing_dots = False
      else:
        (x, y) = map(int, line.split(','))
        pos = Position(x, y)
        grid[pos] = 1
    else:
      match = pattern.match(line)
      axis = match.group(1)
      value = int(match.group(2))
      folds.append((axis, value))
    index += 1
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

def count_grid(grid):
  '''Counts the number of visible dots.'''
  return len(grid.keys())

def execute_fold(grid, fold):
  '''Folds the paper along a line.'''
  axis = fold[0]
  value = fold[1]

  new_positions = []
  positions_to_delete = []
  for position, _ in grid.items():
    if axis == 'x' and position.x > value:
      positions_to_delete.append(position)
      new_x = value - abs(value - position.x)
      new_position = Position(new_x, position.y)
      new_positions.append(new_position)
    elif axis == 'y' and position.y > value:
      positions_to_delete.append(position)
      new_y = value - abs(value - position.y)
      new_position = Position(position.x, new_y)
      new_positions.append(new_position)

  for position in new_positions:
    grid[position] = 1

  for position in positions_to_delete:
    del grid[position]

  return grid

def main():
  '''I hope it makes a swan!'''
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  (grid, folds) = get_grid_and_folds(args.filename)
  if args.part == 1:
    grid = execute_fold(grid, folds[0])
    print(f'After 1 fold, number of dots: {count_grid(grid)}')
  elif args.part == 2:
    for fold in folds:
      grid = execute_fold(grid, fold)
    print_grid(grid)

if __name__ == "__main__":
  main()
