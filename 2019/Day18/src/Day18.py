import argparse
import itertools

import my_collections
from position import Position

class SquareGrid: # pylint: disable=C0115
  def __init__(self, width, height):
    self.width = width
    self.height = height
    self.walls = set()
    self.objective_locations = dict()
    self.objectives = dict()
    self.start = None
    self.distances = None
    self.doors = dict()

  def in_bounds(self, position): # pylint: disable=C0116
    return 0 <= position.x < self.width and 0 <= position.y < self.height

  def passable(self, position): # pylint: disable=C0116
    return position not in self.walls

  @staticmethod
  def no_z_component(position): # pylint: disable=C0116
    return position.z == 0

  # def have_key(self, position, keys): # pylint: disable=C0116
  #   if position not in self.doors:
  #     return True

  #   return self.doors[position].lower() in keys

  def neighbors(self, position): # pylint: disable=C0116
    results = position.get_adjacent_positions()
    results = filter(self.in_bounds, results)
    results = filter(self.passable, results)
    results = filter(SquareGrid.no_z_component, results)
    # results = filter(lambda p: self.have_key(p, keys), results)
    return results

  def add_wall(self, position): # pylint: disable=C0116
    self.walls.add(position)

  def add_objective(self, objective, position): # pylint: disable=C0116
    if objective == '@':
      self.start = position
    else:
      self.objective_locations[objective] = position
      self.objectives[position] = objective

  def add_door(self, door, position): # pylint: disable=C0116
    self.doors[position] = door

  @staticmethod
  def heuristic(position1, position2): # pylint: disable=C0116
    return position1.distance(position2)

  def calculate_distance(self, start, end): # pylint: disable=C0116
    frontier = my_collections.PriorityQueue()
    frontier.put(start, 0)
    came_from = dict()
    cost_so_far = dict()
    came_from[start] = None
    cost_so_far[start] = 0

    while not frontier.empty():
      current = frontier.get()

      if current == end:
        break

      for n in self.neighbors(current):
        new_cost = cost_so_far[current] + 1
        if n not in cost_so_far or new_cost < cost_so_far[n]:
          cost_so_far[n] = new_cost
          priority = new_cost + SquareGrid.heuristic(end, n)
          frontier.put(n, priority)
          came_from[n] = current

    # Walk the path backwards to find any doors encountered
    doors = set()
    current = end
    while current != start:
      if current in self.doors:
        doors.add(self.doors[current])
      current = came_from[current]

    return (cost_so_far[end], doors)

  def calculate_paths(self): # pylint: disable=C0116
    self.distances = dict()

    # Add distances from start to objectives
    for objective in self.objective_locations:
      target = self.objective_locations[objective]
      (distance, doors_encountered) = self.calculate_distance(self.start, target)
      self.distances[(self.start, target)] = (distance, doors_encountered)
      self.distances[(target, self.start)] = (distance, doors_encountered)

    # Add distances between objectives
    for combination in itertools.combinations(self.objective_locations.keys(), 2):
      start = self.objective_locations[combination[0]]
      end = self.objective_locations[combination[1]]
      (distance, doors_encountered) = self.calculate_distance(start, end)
      self.distances[(start, end)] = (distance, doors_encountered)
      self.distances[(end, start)] = (distance, doors_encountered)

  @staticmethod
  def _have_keys_for_doors(keys, doors):
    for door in doors:
      if door.lower() not in keys:
        return False

    return True

  def find_best_permutation(self, return_to_start=False): # pylint: disable=C0116
    self.calculate_paths()

    mincost = None
    minperm = None
    for permutation in itertools.permutations(self.objective_locations.keys()):
      keys = set()
      first_objective = self.objective_locations[permutation[0]]
      (total, doors_encountered) = self.distances[(self.start, first_objective)]
      # Cannot reach this objective first
      if len(doors_encountered) > 1:
        continue

      keys.add(self.objectives[first_objective])
      bad_path = False
      for i in range(len(permutation) - 1):
        start = self.objective_locations[permutation[i]]
        end = self.objective_locations[permutation[i + 1]]
        (subtotal, doors_encountered) = self.distances[(start, end)]
        if not SquareGrid._have_keys_for_doors(keys, doors_encountered):
          bad_path = True
          break
        keys.add(self.objectives[end])
        total += subtotal

      if bad_path:
        continue

      if return_to_start:
        last_objective = self.objective_locations[permutation[-1]]
        (subtotal, _) = self.distances[(last_objective, self.start)]
        total += subtotal

      if mincost is None or total < mincost:
        mincost = total
        minperm = permutation

    return (minperm, mincost)

  def __str__(self):
    s = ''
    for y in range(self.height):
      for x in range(self.width):
        position = Position(x, y)
        if position in self.walls:
          s += '#'
        elif position in self.objectives:
          s += self.objectives[position]
        elif position in self.doors:
          s += self.doors[position]
        elif position == self.start:
          s += '@'
        else:
          s += '.'
      s += '\n'
    return s

def get_grid(filename): # pylint: disable=C0116
  lines = None
  with open(filename, 'r') as file:
    lines = file.readlines()

  height = len(lines)
  width = len(lines[0].rstrip())
  grid = SquareGrid(width, height)

  y = 0
  for line in lines:
    x = 0
    for ch in line.rstrip():
      position = Position(x, y)
      if ch == '#':
        grid.add_wall(position)
      elif ch != '.':
        if ch.isupper():
          grid.add_door(ch, position)
        else:
          grid.add_objective(ch, position)
      x += 1
    y += 1
  return grid

def main(): # pylint: disable=C0116
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample1.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  grid = get_grid(args.filename)
  print(grid)
  if args.part == 1:
    (perm, cost) = grid.find_best_permutation()
    print(f'Best path is {perm}, cost {cost}')

if __name__ == "__main__":
  main()
