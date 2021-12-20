import argparse

def get_trench_map(filename):
  '''Get the image enhancement string and grid from the file.'''
  image_enhancement_algorithm = None
  grid = dict()
  with open(filename, 'r') as fh:
    image_enhancement_algorithm = fh.readline().strip()
    fh.readline()
    line = fh.readline().strip()
    y = 0
    while line:
      for x, ch in enumerate(line):
        if ch == '#':
          value = '#'
        else:
          value = ' '
        grid[(x, y)] = value

      line = fh.readline().strip()
      y += 1

  return (image_enhancement_algorithm, grid)

def print_grid(grid):
  '''Prints a grid.'''
  minx = min(grid.keys(), key=lambda p: p[0])[0]
  maxx = max(grid.keys(), key=lambda p: p[0])[0]
  miny = min(grid.keys(), key=lambda p: p[1])[1]
  maxy = max(grid.keys(), key=lambda p: p[1])[1]

  for y in range(miny, maxy + 1):
    for x in range(minx, maxx + 1):
      print(grid[(x, y)], end='')
    print('', flush=True)

def get_ordered_neighbors(x, y):
  return [
    (x - 1, y - 1), (x, y - 1), (x + 1, y - 1),
    (x - 1, y), (x, y), (x + 1, y),
    (x - 1, y + 1), (x, y + 1), (x + 1, y + 1)
  ]

def get_index(x, y, grid, infinite_value):
  minx = min(grid.keys(), key=lambda p: p[0])[0] - 1
  maxx = max(grid.keys(), key=lambda p: p[0])[0] + 1
  miny = min(grid.keys(), key=lambda p: p[1])[1] - 1
  maxy = max(grid.keys(), key=lambda p: p[1])[1] + 1
  digits = []
  for neighbor in get_ordered_neighbors(x, y):
    if neighbor not in grid:
      digits.append(infinite_value)
    else:
      if grid[neighbor] == '#':
        digits.append('1')
      else:
        digits.append('0')

  return int(''.join(digits), 2)

def count_pixels(grid):
  minx = min(grid.keys(), key=lambda p: p[0])[0]
  maxx = max(grid.keys(), key=lambda p: p[0])[0]
  miny = min(grid.keys(), key=lambda p: p[1])[1]
  maxy = max(grid.keys(), key=lambda p: p[1])[1]
  total = 0
  for y in range(miny, maxy + 1):
    for x in range(minx, maxx + 1):
      if grid[(x, y)] == '#':
        total += 1

  return total

def do_step(grid, step, iea):
  new_grid = dict()
  minx = min(grid.keys(), key=lambda p: p[0])[0] - 1
  maxx = max(grid.keys(), key=lambda p: p[0])[0] + 1
  miny = min(grid.keys(), key=lambda p: p[1])[1] - 1
  maxy = max(grid.keys(), key=lambda p: p[1])[1] + 1

  if iea[0] == '.':
    infinite_value = '0'
  elif step % 2 == 0:
    infinite_value = '1'
  else:
    infinite_value = '0'

  print(f'Doing step {step}, the infinite value is {infinite_value}')

  for y in range(miny, maxy + 1):
    for x in range(minx, maxx + 1):
      index = get_index(x, y, grid, infinite_value)
      new_grid[(x, y)] = iea[index]

  return new_grid

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  (image_enhancement_algorithm, grid) = get_trench_map(args.filename)

  if args.part == 1:
    steps = 2
  elif args.part == 2:
    steps = 50

  for step in range(steps):
    grid = do_step(grid, step + 1, image_enhancement_algorithm)

  #print_grid(grid)
  total = count_pixels(grid)
  print(f'There are {total} pixels on.')

if __name__ == "__main__":
  main()
