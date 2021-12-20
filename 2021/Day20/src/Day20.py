import argparse

def get_trench_map(filename):
  '''Get the image enhancement string and grid from the file.'''
  image_enhancement_algorithm = None
  grid = dict()
  max_x = 0
  max_y = 0
  with open(filename, 'r') as fh:
    image_enhancement_algorithm = fh.readline().strip()
    fh.readline()
    line = fh.readline().strip()
    max_x = len(line)
    while line:
      for x, ch in enumerate(line):
        if ch == '#':
          value = '#'
        else:
          value = ' '
        grid[(x, max_y)] = value

      line = fh.readline().strip()
      max_y += 1

  return (image_enhancement_algorithm, grid, max_x, max_y)

def get_index(x, y, grid, infinite_value):
  '''Converts an x, y point into an index on the IEA.'''
  digits = ''
  neighbors = None
  if (x, y) in get_index.PRECOMPUTED_NEIGHBORS:
    neighbors = get_index.PRECOMPUTED_NEIGHBORS[(x, y)]
  else:
    neighbors = [
      (x - 1, y - 1), (x, y - 1), (x + 1, y - 1),
      (x - 1, y),     (x, y),     (x + 1, y),
      (x - 1, y + 1), (x, y + 1), (x + 1, y + 1)
    ]
    get_index.PRECOMPUTED_NEIGHBORS[(x, y)] = neighbors

  for neighbor in neighbors:
    if neighbor not in grid:
      digits += infinite_value
    elif grid[neighbor] == '#':
      digits += '1'
    else:
      digits += '0'

  return int(digits, 2)

get_index.PRECOMPUTED_NEIGHBORS = dict()

def count_pixels(grid):
  '''Counts the number of "on" pixels in the image.'''
  return sum(1 for p in grid.keys() if grid[p] == '#')

def do_step(grid, step, iea, dimensions):
  '''Enhances the image according to the IEA.'''
  new_grid = dict()
  minx = dimensions[0] - 1
  miny = dimensions[1] - 1
  maxx = dimensions[2] + 1
  maxy = dimensions[3] + 1

  if iea[0] == '.':
    infinite_value = '0'
  elif step % 2 == 0:
    infinite_value = '1'
  else:
    infinite_value = '0'

  for y in range(miny, maxy + 1):
    for x in range(minx, maxx + 1):
      new_grid[(x, y)] = iea[get_index(x, y, grid, infinite_value)]

  return (new_grid, (minx, miny, maxx, maxy))

def main():
  '''Zoom and enhance!'''
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  (image_enhancement_algorithm, grid, maxx, maxy) = get_trench_map(args.filename)

  if args.part == 1:
    steps = 2
  elif args.part == 2:
    steps = 50

  dimensions = (0, 0, maxx, maxy)

  for step in range(steps):
    (grid, dimensions) = do_step(grid, step + 1, image_enhancement_algorithm, dimensions)

  total = count_pixels(grid)
  print(f'After {steps} steps, there are {total} pixels on.')

if __name__ == "__main__":
  main()
