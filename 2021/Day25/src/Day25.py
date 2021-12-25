import argparse
from functools import cache

def get_sea_cucumber_field(filename):
  '''Gets the field of SCs from the file'''
  with open(filename, 'r') as fh:
    lines = fh.readlines()

  grid = dict()
  for y, line in enumerate(lines):
    line = line.strip()
    for x, ch in enumerate(line):
      grid[(x, y)] = ch

  return grid

def print_grid(grid):
  '''Prints a grid of dots.'''
  for y in range(print_grid.Y_DIMENSION):
    for x in range(print_grid.X_DIMENSION):
      print(grid[(x, y)], end='')
    print('', flush=True)
  print('', flush=True)

@cache
def get_destination(x, y, ch):
  '''Gets the destination cell of the sea cucumber.'''
  new_x = x
  new_y = y
  if ch == '>':
    new_x = (new_x + 1) % get_destination.X_DIMENSION
  elif ch == 'v':
    new_y = (new_y + 1) % get_destination.Y_DIMENSION

  return (new_x, new_y)

def move_herd(grid, herd):
  '''The herd moves.'''
  newgrid = dict()
  changed = 0

  for y in range(do_step.Y_DIMENSION):
    for x in range(do_step.X_DIMENSION):
      source = (x, y)
      if source in newgrid:
        #print(f'{source} already set')
        continue
      ch = grid[source]
      if ch == '.' or ch != herd:
        #print(f'{source} does not move')
        newgrid[source] = ch
      elif ch == herd:
        destination = get_destination(x, y, ch)
        if grid[destination] == '.':
          #print(f'Cucumber at {source} moves to {destination}')
          newgrid[source] = '.'
          newgrid[destination] = ch
          changed += 1
        else:
          #print(f'Cucumber at {source} cannot move')
          newgrid[source] = ch

  return (newgrid, changed)

def do_step(grid):
  '''Simulates a step of time for the SCs.'''
  (grid, changed) = move_herd(grid, '>')
  (grid, temp) = move_herd(grid, 'v')
  return (grid, changed + temp)

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample1.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  grid = get_sea_cucumber_field(args.filename)

  maxx = max(grid.keys(), key=lambda p: p[0])[0]
  maxy = max(grid.keys(), key=lambda p: p[1])[1]

  print_grid.X_DIMENSION = maxx + 1
  print_grid.Y_DIMENSION = maxy + 1

  get_destination.X_DIMENSION = maxx + 1
  get_destination.Y_DIMENSION = maxy + 1

  do_step.X_DIMENSION = maxx + 1
  do_step.Y_DIMENSION = maxy + 1

  #print_grid(grid)

  step = 1
  (grid, changed) = do_step(grid)
  while changed:
    step += 1
    (grid, changed) = do_step(grid)

  print(f'Equilibrium after {step} steps.')

  #print_grid(grid)

if __name__ == "__main__":
  main()
