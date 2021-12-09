import argparse
from my_collections import Queue # My own 2019 code!
from position import Position # My own 2019 code!

def get_grid(filename):
  '''Fetches and parses a map/grid from the file'''
  lines = None
  with open(filename, 'r') as fh:
    lines = fh.readlines()

  grid = dict()
  for y, line in enumerate(lines):
    for x, depth in enumerate(line.strip()):
      grid[Position(x, y)] = int(depth)

  return grid

def find_low_points(grid):
  '''Finds positions that are local minimums in the grid'''
  return [p for p,d in grid.items() \
   if all(n not in grid or grid[n] > d for n in p.get_adjacent_positions()) ]

def find_size_of_basin(grid, start):
  '''Returns the size of the basin around start'''
  # Each low_point forms a basin.
  # Each basin can be fully explored by BFS from the low point.
  frontier = Queue()
  frontier.put(start)
  explored = set()
  explored.add(start)
  while not frontier.empty():
    point = frontier.get()
    for neighbor in point.get_adjacent_positions():
      if neighbor in explored:
        continue
      if neighbor not in grid:
        continue
      if grid[neighbor] == 9:
        continue
      explored.add(neighbor)
      frontier.put(neighbor)
  return len(explored)

def get_basins(grid, low_points):
  '''Returns the size of the basins of each low point'''
  basins = dict()
  for point in low_points:
    basins[point] = find_size_of_basin(grid, point)

  return basins

def main():
  '''Down, down, to Santa's town'''
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  grid = get_grid(args.filename)
  low_points = find_low_points(grid)
  if args.part == 1:
    print(f'Total risk level: {sum(map(lambda p: grid[p] + 1, low_points))}')
  else:
    basins = get_basins(grid, low_points)
    sizes = sorted(list(basins.values()))
    print(f'Basin product: {sizes[-1] * sizes[-2] * sizes[-3]}')

if __name__ == "__main__":
  main()
