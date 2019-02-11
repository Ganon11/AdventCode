import argparse
import copy
import sys

def get_grid_from_file(filename):
  grid = list()
  with open(filename, 'r') as fp:
    for line in fp.readlines():
      row = list()
      for ch in list(line.rstrip()):
        if '#' == ch:
          row.append(True)
        else:
          row.append(False)
      grid.append(row)

  return grid

def print_grid(grid):
  for line in grid:
    for ch in line:
      if ch:
        print("#", end = " ")
      else:
        print(".", end = " ")
    print("")

def count_grid(grid):
  sum = 0
  for line in grid:
    for ch in line:
      if ch:
        sum = sum + 1

  return sum

def count_neighbors(grid, row, column):
  sum = 0
  if row > 0:
    if column > 0 and grid[row - 1][column - 1]:
      sum = sum + 1
    if grid[row - 1][column]:
      sum = sum + 1
    if column < (len(grid[row - 1]) - 1) and grid[row - 1][column + 1]:
      sum = sum + 1

  if column > 0 and grid[row][column - 1]:
    sum = sum + 1

  if column < (len(grid[row]) - 1) and grid[row][column + 1]:
    sum = sum + 1

  if row < (len(grid) - 1):
    if column > 0 and grid[row + 1][column - 1]:
      sum = sum + 1
    if grid[row + 1][column]:
      sum = sum + 1
    if column < (len(grid[row + 1]) - 1) and grid[row + 1][column + 1]:
      sum = sum + 1

  return sum

def generation(grid):
  newgrid = copy.deepcopy(grid)
  for row in range(len(grid)):
    for col in range(len(grid[row])):
      neighbors = count_neighbors(grid, row, col)
      if grid[row][col] and neighbors != 2 and neighbors != 3:
        newgrid[row][col] = False
      elif not grid[row][col] and neighbors == 3:
        newgrid[row][col] = True

  return newgrid

def grid_to_string(grid):
  return ''.join(map(lambda x: '#' if x == 'True' else '.', [str(item) for row in grid for item in row]))

def part_1(filename, generations):
  grid = get_grid_from_file(filename)

  lookup = dict()

  for t in range(generations):
    grid_string = grid_to_string(grid)
    if grid_string in lookup:
      grid = lookup[grid_string]
    else:
      newgrid = generation(grid)
      lookup[grid_string] = newgrid
      grid = newgrid

  #print_grid(grid)
  print("Part 1: Lights on: %d" % count_grid(grid))

def set_corners(grid):
  side_length = len(grid)
  grid[0][0] = True
  grid[0][side_length - 1] = True
  grid[side_length - 1][0] = True
  grid[side_length - 1][side_length - 1] = True

def part_2(filename, generations):
  grid = get_grid_from_file(filename)
  set_corners(grid)

  lookup = dict()

  for t in range(generations):
    grid_string = grid_to_string(grid)
    if grid_string in lookup:
      grid = lookup[grid_string]
    else:
      newgrid = generation(grid)
      set_corners(newgrid)
      lookup[grid_string] = newgrid
      grid = newgrid

  #print_grid(grid)
  print("Part 2: Lights on: %d" % count_grid(grid))

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('filename')
  parser.add_argument('-g', '--generations', type=int, default=100)
  args = parser.parse_args()

  part_1(args.filename, args.generations)
  part_2(args.filename, args.generations)

if __name__ == "__main__":
  main()
