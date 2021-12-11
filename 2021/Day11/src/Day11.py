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

def run_step(grid):
  '''Simulates a step of the flashing-octopus field.'''
  # First, the energy level of each octopus increases by 1
  flashing = Queue()
  for position in grid.keys():
    grid[position] += 1
    if grid[position] > 9:
      flashing.put(position)

  # Then, any octopus with an energy level greater than 9 flashes
  # An octopus can only flash at most once per step.
  flashed = set()
  while not flashing.empty():
    current = flashing.get()
    if current in flashed:
      continue

    flashed.add(current)
    # A flashing octopus increases the energy level of all adjacent octopuses by
    # 1, which can cause cascading flashes.
    for neighbor in current.get_adjacent_positions(include_diagonals=True):
      # The grid doesn't grow
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
  '''Simulates the flashing octopus field for a certain number of steps.'''
  flashes = 0
  for _ in range(steps):
    (grid, flashes_in_step) = run_step(grid)
    flashes += flashes_in_step

  return flashes

def run_simulation_until_sync(grid):
  '''Simulates the flashing octopus field until the octopuses synchronize.'''
  index = 1
  (grid, flashes) = run_step(grid)
  while flashes != len(grid):
    index += 1
    (grid, flashes) = run_step(grid)

  return index

def main():
  '''I've heard of Strawberry Fields, but this is ridiculous.'''
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  parser.add_argument('-s', '--steps', default=100, type=int)
  args = parser.parse_args()

  grid = get_grid(args.filename)
  if args.part == 1:
    print(f'Flashes: {run_simulation(grid, args.steps)}')
  else:
    print(f'Synchronized after {run_simulation_until_sync(grid)} steps')

if __name__ == "__main__":
  main()
