import argparse
import heapq
import itertools
import sys

class PriorityQueue:
  def __init__(self):
    self.elements = []

  def empty(self):
    return len(self.elements) == 0

  def put(self, item, priority):
    heapq.heappush(self.elements, (priority, item))

  def get(self):
    return heapq.heappop(self.elements)[1]

class SquareGrid:
  def __init__(self, width, height):
    self.width = width
    self.height = height
    self.walls = set()
    self.objective_locations = dict()
    self.objectives = dict()
    self.start = ()

  def in_bounds(self, id):
    (x, y) = id
    return 0 <= x < self.width and 0 <= y < self.height

  def passable(self, id):
    return id not in self.walls

  def neighbors(self, id):
    #print(id)
    (x, y) = id
    results = [(x + 1, y), (x, y - 1), (x - 1, y), (x, y + 1)]
    results = filter(self.in_bounds, results)
    results = filter(self.passable, results)
    return results

  def add_wall(self, id):
    self.walls.add(id)

  def add_objective(self, objective, id):
    if objective == 0:
      self.start = id
    else:
      self.objective_locations[objective] = id
      self.objectives[id] = objective

  def heuristic(self, a, b):
    (x1, y1) = a
    (x2, y2) = b
    return abs(x1 - x2) + abs(y1 - y2)

  def calculate_distance(self, start, end):
    frontier = PriorityQueue()
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
          priority = new_cost + self.heuristic(end, n)
          frontier.put(n, priority)
          came_from[n] = current

    return cost_so_far[end]

  def calculate_paths(self):
    self.distances = dict()

    # Add distances from 0 to objectives
    for o in self.objective_locations.keys():
      target = self.objective_locations[o]
      distance = self.calculate_distance(self.start, target)
      self.distances[(self.start, target)] = distance
      self.distances[(target, self.start)] = distance

    # Add distances between objectives
    for combination in itertools.combinations(self.objective_locations.keys(), 2):
      distance = self.calculate_distance(self.objective_locations[combination[0]], self.objective_locations[combination[1]])
      self.distances[(self.objective_locations[combination[0]], self.objective_locations[combination[1]])] = distance
      self.distances[(self.objective_locations[combination[1]], self.objective_locations[combination[0]])] = distance

  def find_best_permutation(self, return_to_start=False):
    self.calculate_paths()

    mincost = None
    minperm = None
    for permutation in itertools.permutations(self.objective_locations.keys()):
      total = self.distances[(self.start, self.objective_locations[permutation[0]])]
      for i in range(len(permutation) - 1):
        total += self.distances[(self.objective_locations[permutation[i]], self.objective_locations[permutation[i + 1]])]

      if return_to_start:
        total += self.distances[(self.objective_locations[permutation[-1]], self.start)]

      if mincost is None or total < mincost:
        mincost = total
        minperm = permutation

    return minperm, mincost

  def __str__(self):
    s = ''
    for y in range(self.height):
      for x in range(self.width):
        id = (x, y)
        if id in self.walls:
          s += '#'
        elif id in self.objectives:
          s += str(self.objectives[id])
        elif id == self.start:
          s += 'S'
        else:
          s += '.'
      s += "\n"
    return s

def get_grid(filename):
  grid = None
  with open(filename, 'r') as fp:
    lines = fp.readlines()
    h = len(lines)
    w = len(lines[0].rstrip())
    grid = SquareGrid(w, h)
    y = 0
    for line in lines:
      x = 0
      for ch in line.rstrip():
        id = (x, y)
        if ch == '#':
          grid.add_wall(id)
        elif ch != '.':
          objective = int(ch)
          grid.add_objective(objective, id)
        x += 1
      y += 1
  return grid

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', type=str, default='sample.txt')
  args = parser.parse_args()

  grid = get_grid(args.filename or 'sample.txt')
  print(grid)
  (path, cost) = grid.find_best_permutation()
  print("Path: %s, cost %d" % (path, cost))
  (path, cost) = grid.find_best_permutation(return_to_start=True)
  print("Path: %s, cost %d" % (path, cost))

if __name__ == "__main__":
  main()