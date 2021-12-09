import argparse
from my_collections import Queue # My own 2019 code!
from position import Position # My own 2019 code!

def get_grid(filename):
  '''Fetches and parses a map/grid from the file'''
  lines = None
  with open(filename, 'r') as fh:
    lines = fh.readlines()

  grid = dict()
  row = 0
  for line in lines:
    col = 0
    for depth in line.strip():
      grid[Position(col, row)] = int(depth)
      col += 1
    row += 1

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

def find_low_points(grid):
  '''Finds positions that are local minimums in the grid'''
  low_points = list()
  for position, depth in grid.items():
    is_low_point = True
    for neighbor in position.get_adjacent_positions():
      if neighbor in grid and grid[neighbor] <= depth:
        is_low_point = False
        break

    if is_low_point:
      low_points.append(position)

  return low_points

def get_basins(grid, low_points=None):
  '''Returns the size of the basins of each low point'''
  if low_points is None:
    low_points = find_low_points(grid)

  basins = dict()
  for point in low_points:
    # Each low_point forms a basin.
    # Each basin can be fully explored by BFS from the low point.
    frontier = Queue()
    frontier.put(point)
    explored = set()
    explored.add(point)
    while not frontier.empty():
      p = frontier.get()
      for neighbor in p.get_adjacent_positions():
        if neighbor in explored:
          continue
        if neighbor not in grid:
          continue
        if grid[neighbor] == 9:
          continue
        explored.add(neighbor)
        frontier.put(neighbor)
    basins[point] = len(explored)

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
    sizes = list(basins.values())
    sizes.sort()
    if len(sizes) < 3:
      print('Not enough basins found!')
    else:
      product = sizes[-1] * sizes[-2] * sizes[-3]
      print(f'Basin product: {product}')

if __name__ == "__main__":
  main()
