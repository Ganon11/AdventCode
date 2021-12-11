import argparse
from my_collections import Queue # My own 2019 code!
from position import Position # My own 2019 code!

def get_grid(filename):
  '''Gets the initial grid from the file.'''
  lines = None
  with open(filename, 'r') as fh:
    lines = list(map(lambda line: line.strip(), fh.readlines()))

  grid = dict()
  for y, line in enumerate(lines):
    for x, value in enumerate(line):
      grid[Position(x, y)] = int(value)

  return grid

def print_grid(grid):
  '''Prints a depth grid'''
  minx = min(grid.keys(), key=lambda p: p.x).x
  maxx = max(grid.keys(), key=lambda p: p.x).x
  miny = min(grid.keys(), key=lambda p: p.y).y
  maxy = max(grid.keys(), key=lambda p: p.y).y

  for y in range(miny, maxy + 1):
    for x in range(minx, maxx + 1):
      position = Position(x, y)
      print(grid[position], end='')
    print('', flush=True)

def run_day(grid):
  # First, the energy level of each octopus increases by 1
  flashing = Queue()
  for position in grid.keys():
    grid[position] += 1
    if grid[position] > 9:
      flashing.put(position)

  # Then, any octopus with an energy level greater than 9 flashes
  flashed = set()
  while not flashing.empty():
    current = flashing.get()
    if current in flashed:
      continue

    flashed.add(current)
    for neighbor in current.get_adjacent_positions(include_diagonals=True):
      if neighbor not in grid:
        continue

      if neighbor in flashed:
        continue

      grid[neighbor] += 1
      if grid[neighbor] > 9:
        flashing.put(neighbor)

  # Finally, any octopus that flashed has its energy level set to 0
  for position in flashed:
    grid[position] = 0

  return (grid, len(flashed))

def run_simulation(grid, steps):
  flashes = 0
  for _ in range(steps):
    (grid, flashes_in_step) = run_day(grid)
    flashes += flashes_in_step

  return (grid, flashes)

def run_simulation_until_sync(grid):
  index = 1
  (grid, flashes) = run_day(grid)
  while flashes != len(grid):
    index += 1
    (grid, flashes) = run_day(grid)

  return (grid, index)

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  parser.add_argument('-s', '--steps', default=100, type=int)
  args = parser.parse_args()

  grid = get_grid(args.filename)
  if args.part == 1:
    (grid, flashes) = run_simulation(grid, args.steps)
    print_grid(grid)
    print(f'Flashes: {flashes}')
  else:
    (grid, step) = run_simulation_until_sync(grid)
    print_grid(grid)
    print(f'Synchronized after {step} steps')

if __name__ == "__main__":
  main()
