import argparse
from my_collections import PriorityQueue
from position import Position # My own 2019 code!

def get_grid(filename):
  '''Gets the grid from the file.'''
  lines = []
  with open(filename, 'r') as fh:
    lines = list(map(lambda line: line.strip(), fh.readlines()))

  grid = dict()
  for y, line in enumerate(lines):
    for x, character in enumerate(line):
      grid[Position(x, y)] = int(character)

  return grid

def get_cost_of_best_path(grid):
  '''Finds the cost of the best path from 0,0 to the bottom-right.'''
  start = Position(0, 0)
  maxx = max(grid.keys(), key=lambda p: p.x).x
  maxy = max(grid.keys(), key=lambda p: p.y).y
  goal = Position(maxx, maxy)
  frontier = PriorityQueue()
  frontier.put(start, 0)
  came_from = dict()
  cost_so_far = dict()
  came_from[start] = None
  cost_so_far[start] = 0

  while not frontier.empty():
    current = frontier.get()

    if current == goal:
      break

    for neighbor in current.get_adjacent_positions():
      if neighbor not in grid:
        continue
      new_cost = cost_so_far[current] + grid[neighbor]
      if neighbor not in cost_so_far or new_cost < cost_so_far[neighbor]:
        cost_so_far[neighbor] = new_cost
        priority = new_cost + neighbor.distance(goal)
        frontier.put(neighbor, priority)
        came_from[neighbor] = current

  return cost_so_far[goal]

def expand_grid(grid):
  '''Expands the grid 5 times in each direction.'''
  maxx = max(grid.keys(), key=lambda p: p.x).x
  maxy = max(grid.keys(), key=lambda p: p.y).y
  for expand_y in range(5):
    for expand_x in range(5):
      if expand_y == 0 and expand_x == 0:
        continue

      risk_adjustment = expand_y + expand_x
      for original_y in range(maxy + 1):
        for original_x in range(maxx + 1):
          original_position = Position(original_x, original_y)
          new_risk = grid[original_position] + risk_adjustment
          if new_risk > 9:
            new_risk -= 9
          new_x = original_x + (expand_x * (maxx + 1))
          new_y = original_y + (expand_y * (maxy + 1))
          new_position = Position(new_x, new_y)
          grid[new_position] = new_risk

  return grid

def main():
  '''I can't think of anything clever.'''
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  grid = get_grid(args.filename)
  if args.part == 2:
    grid = expand_grid(grid)
  cost = get_cost_of_best_path(grid)
  print(f'Cost: {cost}')

if __name__ == "__main__":
  main()
