import re
import sys

def get_arrangement_rules(filename):
  lines = None
  with open(filename, 'r') as fp:
    lines = fp.readlines()

  people = list()
  edges = dict()
  pattern = re.compile('(\w+) would (gain|lose) (\d+) happiness units by sitting next to (\w+).', re.IGNORECASE)
  lose_pattern = re.compile('lose', re.IGNORECASE)
  for line in lines:
    match = pattern.match(line)
    a = match.group(1)
    pref = match.group(2)
    amount = int(match.group(3))
    b = match.group(4)

    if lose_pattern.match(pref):
      amount = -1 * amount

    if a not in people:
      people.append(a)
    if b not in people:
      people.append(b)

    edges[','.join([a, b])] = amount

  return people, edges

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

def cost_of_arrangement(arrangement, edges):
  total = 0
  for index in range(len(arrangement)):
    person = arrangement[index]
    neighbor1 = arrangement[index - 1]
    nextindex = index + 1
    if nextindex == len(arrangement):
      nextindex = 0
    neighbor2 = arrangement[nextindex]

    total = total + edges[','.join([person, neighbor1])] + edges[','.join([person, neighbor2])]

  return total

def find_optimal_seating_arrangement(people, edges):
  if len(people) < 2:
    print("Less than 2 people! Figure it out!")
    return

  pathcosts = dict()
  for path in permutations(people):
    pathcosts[' -> '.join(path)] = cost_of_arrangement(path, edges)

  bestpath = max(pathcosts, key=pathcosts.get)
  print("Optimal seating arrangement found: %s = %d" % (bestpath, pathcosts[bestpath]))

def main():
  filename = 'sample.txt'
  if len(sys.argv) > 1:
    filename = sys.argv[1]

  people, edges = get_arrangement_rules(filename)
  find_optimal_seating_arrangement(people, edges)

  me = 'Ganon11'
  for person in people:
    edges[','.join([me, person])] = 0
    edges[','.join([person, me])] = 0
  people.append(me)
  find_optimal_seating_arrangement(people, edges)

if __name__ == "__main__":
  main()
