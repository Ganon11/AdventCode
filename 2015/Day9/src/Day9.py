import os
import re
import sys

def get_routes(filename):
  f = open(filename, 'r')
  routes = f.readlines()
  pattern = re.compile('(\w+) to (\w+) = (\d+)', re.IGNORECASE)
  destinations = list()
  edges = dict()

  for route in routes:
    match = pattern.match(route)

    a = match.group(1)
    b = match.group(2)
    cost = int(match.group(3))

    if a not in destinations:
      destinations.append(a)
    if b not in destinations:
      destinations.append(b)

    edges[','.join([a, b])] = cost
    edges[','.join([b, a])] = cost

  return destinations, edges

def permutations(values):
  if len(values) == 0:
    return []

  if len(values) == 1:
    return [values]

  perms = []
  for index in range(len(values)):
    item = values[index]
    remaining = values[:index] + values[index + 1:]
    for p in permutations(remaining):
      perms.append([item] + p)

  return perms

def cost_of_path(path, edges):
  a = path[0]
  b = path[1]
  edge = ','.join([a, b])

  cost = edges[edge]
  for i in range(2, len(path)):
    a = b
    b = path[i]
    edge = ','.join([a, b])

    cost += edges[edge]

  return cost

def main(filename):
  destinations, edges = get_routes(filename)

  if len(destinations) < 2:
    print("Less than 2 destinations! Figure it out!")
    return

  pathcosts = dict()
  for path in permutations(destinations):
    cost = cost_of_path(path, edges)

    pathcosts[' -> '.join(path)] = cost

  bestpath = min(pathcosts, key=pathcosts.get)
  worstpath = max(pathcosts, key=pathcosts.get)

  print("Best path found: %s = %d" % (bestpath, pathcosts[bestpath]))
  print("Worst path found: %s = %d" % (worstpath, pathcosts[worstpath]))

if __name__ == "__main__":
  filename = '../input/Input.txt'
  if len(sys.argv) > 1:
    filename = sys.argv[1]
  main(filename)