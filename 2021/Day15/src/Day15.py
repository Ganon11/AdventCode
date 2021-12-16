import argparse
from my_collections import PriorityQueue

def get_grid(filename):
  '''Gets the grid from the file.'''
  with open(filename, 'r') as fh:
    lines = fh.readlines()

  grid = dict()
  dimension_size = len(lines)
  for y, line in enumerate(lines):
    line = line.strip()
    for x, character in enumerate(line):
      grid[(x, y)] = int(character)

  return grid, dimension_size

def get_risk_of_position(grid, position, dimension_size):
  '''Gets the actual risk of a position.'''
  risk_adjustment = position[0] // dimension_size
  actual_x = position[0] % dimension_size
  risk_adjustment += position[1] // dimension_size
  actual_y = position[1] % dimension_size

  return ((grid[(actual_x, actual_y)] + risk_adjustment - 1) % 9) + 1

def get_cost_of_best_path(grid, dimension_size, scale_factor=1):
  '''Finds the cost of the best path from 0,0 to the bottom-right.'''
  start = (0, 0)
  max_position = (dimension_size * scale_factor) - 1
  goal = (max_position, max_position)

  frontier = PriorityQueue()
  frontier.put(start, 0)
  cost_so_far = { start: 0 }

  while not frontier.empty():
    current = frontier.get()

    if current == goal:
      break

    for neighbor in [(current[0] - 1, current[1]), (current[0] + 1, current[1]),
                     (current[0], current[1] - 1), (current[0], current[1] + 1)]:
      # Check in bounds
      if (neighbor[0] < 0 or max_position < neighbor[0] or
          neighbor[1] < 0 or max_position < neighbor[1]):
        continue

      new_cost = cost_so_far[current] + get_risk_of_position(grid, neighbor, dimension_size)

      if neighbor not in cost_so_far or new_cost < cost_so_far[neighbor]:
        cost_so_far[neighbor] = new_cost
        priority = new_cost + abs(neighbor[0] - goal[0]) + abs(neighbor[1] - goal[1])
        frontier.put(neighbor, priority)

  return cost_so_far[goal]

def main():
  '''I can't think of anything clever.'''
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  grid, dimension_size = get_grid(args.filename)
  if args.part == 1:
    cost = get_cost_of_best_path(grid, dimension_size)
  elif args.part == 2:
    cost = get_cost_of_best_path(grid, dimension_size, scale_factor=5)
  print(f'Cost: {cost}')

if __name__ == "__main__":
  main()
