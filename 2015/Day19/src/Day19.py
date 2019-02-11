import argparse
import sys

def parse_input(filename):
  rules = dict()
  molecule = ''
  with open(filename, 'r') as fp:
    for line in fp.readlines():
      if line.find('=>') != -1:
        parts = line.split()
        if not parts[0] in rules:
          rules[parts[0]] = list()
        rules[parts[0]].append(parts[2])
      else:
        molecule = line
  return (rules, molecule)

def parse_reverse_input(filename):
  rules = dict()
  molecule = ''
  with open(filename, 'r') as fp:
    for line in fp.readlines():
      if line.find('=>') != -1:
        parts = line.split()
        if not parts[2] in rules:
          rules[parts[2]] = list()
        rules[parts[2]].append(parts[0])
      else:
        molecule = line
  return (rules, molecule)

def part_1(filename):
  (rules, molecule) = parse_input(filename)
  values = dict()
  for key in rules:
    for newVal in rules[key]:
      length = len(key)
      index = molecule.find(key)
      while index != -1:
        newMol = molecule[:index] + newVal + molecule[index + length:]
        values[newMol] = 1
        index = molecule.find(key, index + length)
  print("Found %d unique values" % len(values))

def part_2(filename):
  (rules, molecule) = parse_reverse_input(filename)
  frontier = dict()
  seen = dict()
  gens_seen = dict()
  frontier[molecule] = 0

  while 0 < len(frontier):
    current = min(frontier, key=frontier.get)
    gen = frontier[current]

    if not gen in gens_seen:
      print("Now checking gen %d" % gen)
      gens_seen[gen] = 1

    frontier.pop(current)
    if current in seen:
      next

    seen[current] = 1
    if current == 'e':
      print("Got target in %d generations" % gen)
      break

    for key in rules:
      for newVal in rules[key]:
        length = len(key)
        index = current.find(key)
        while index != -1:
          newMol = current[:index] + newVal + current[index + length:]
          frontier[newMol] = gen + 1
          index = current.find(key, index + length)

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('filename')
  args = parser.parse_args()

  part_1(args.filename)
  part_2(args.filename)

if __name__ == "__main__":
  main()
