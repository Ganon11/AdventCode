import argparse
from collections import namedtuple
from my_collections import PriorityQueue

Position = namedtuple('Position', ['x', 'y'])

def get_neighbors(position):
  '''Gets neighbors of a position.'''
  return set([
    Position(position.x - 1, position.y),
    Position(position.x + 1, position.y),
    Position(position.x, position.y - 1),
    Position(position.x, position.y + 1)
  ])

def get_distance(p1, p2):
  '''Gets the Manhattan distance between two points.'''
  return abs(p1.x - p2.x) + abs(p1.y - p2.y)

def get_grid(filename):
  '''Gets the grid from the file.'''
  with open(filename, 'r') as fh:
    lines = fh.readlines()

  grid = dict()
  dimension_size = len(lines)
  for y, line in enumerate(lines):
    line = line.strip()
    for x, character in enumerate(line):
      grid[Position(x, y)] = int(character)

  return grid, dimension_size

def get_risk_of_position(grid, position, dimension_size):
  '''Gets the actual risk of a position.'''
  risk_adjustment = position.x // dimension_size
  actual_x = position.x % dimension_size
  risk_adjustment += position.y // dimension_size
  actual_y = position.y % dimension_size

  adjusted_position = Position(actual_x, actual_y)
  return ((grid[adjusted_position] + risk_adjustment - 1) % 9) + 1

def get_cost_of_best_path(grid, dimension_size, scale_factor=1):
  '''Finds the cost of the best path from 0,0 to the bottom-right.'''
  start = Position(x=0, y=0)
  max_position = (dimension_size * scale_factor) - 1
  goal = Position(x=max_position, y=max_position)

  frontier = PriorityQueue()
  frontier.put(start, 0)
  cost_so_far = { start: 0 }

  while not frontier.empty():
    current = frontier.get()

    if current == goal:
      break

    for neighbor in get_neighbors(current):
      # Check in bounds
      if (neighbor.x < 0 or max_position < neighbor.x or
          neighbor.y < 0 or max_position < neighbor.y):
        continue

      new_cost = cost_so_far[current] + get_risk_of_position(grid, neighbor, dimension_size)

      if neighbor not in cost_so_far or new_cost < cost_so_far[neighbor]:
        cost_so_far[neighbor] = new_cost
        priority = new_cost + get_distance(neighbor, goal)
        frontier.put(neighbor, priority)

  return cost_so_far[goal]

def main():
  '''I can't think of anything clever.'''
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  grid, dimension_size = get_grid(args.filename)
  cost = None
  if args.part == 1:
    cost = get_cost_of_best_path(grid, dimension_size)
  elif args.part == 2:
    cost = get_cost_of_best_path(grid, dimension_size, scale_factor=5)
  print(f'Cost: {cost}')

if __name__ == "__main__":
  main()