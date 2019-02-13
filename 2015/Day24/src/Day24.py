import argparse
import functools
import itertools
import operator
import sys

def get_packages(filename):
  packages = set()
  with open(filename, 'r') as fp:
    for line in fp.readlines():
      packages.add(int(line.rstrip()))
  return packages

def divide_packages(packages, count):
  target_weight = sum(packages) // count
  print(target_weight)
  for num_packages in range(len(packages)):
    qes = [functools.reduce(operator.mul, c) for c in itertools.combinations(packages, num_packages) if sum(c) == target_weight]
    if qes:
      print("Minimum qes %d" % min(qes))
      return

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('filename')
  args = parser.parse_args()

  packages = get_packages(args.filename)

  print(packages)
  divide_packages(packages, 3)
  divide_packages(packages, 4)

if __name__ == "__main__":
  main()
