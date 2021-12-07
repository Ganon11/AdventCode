import argparse
import math

def get_crabs(filename):
  '''Gets the initial position of the crabs from the file'''
  crabs = []
  with open(filename, 'r') as fh:
    crabs = list(map(int, fh.readline().strip().split(',')))
  return crabs

def get_cost(crabs, position):
  '''
  Determines the amount of fuel required for all crabs to align to position.
  '''
  total = 0
  for crab in crabs:
    fuel = abs(crab - position)
    total += fuel
  return total

def get_real_cost(crabs, position):
  '''
  Determines the real amount of fuel required for all crabs to align to
  position.
  '''
  total = 0
  for crab in crabs:
    # To move 1 space costs 1 fuel
    # To move 2 spaces costs 1 + 2 = 3 fuel
    # To move 3 spaces costs 1 + 2 + 3 = 6 fuel
    # To move n spaces costs 1 + 2 + 3 ... + n = n(n+1)/2 fuel
    distance = abs(crab - position)
    fuel = (distance * (distance + 1)) // 2
    total += fuel
  return total

def main():
  '''Insert crab rave here.'''
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  crabs = get_crabs(args.filename)
  if args.part == 1:
    crabs.sort()
    num_crabs = len(crabs)
    print(get_cost(crabs, crabs[num_crabs // 2]))
  else:
    mean = sum(crabs) / len(crabs)
    lower = math.floor(mean)
    upper = math.ceil(mean)
    lower_cost = get_real_cost(crabs, lower)
    upper_cost = get_real_cost(crabs, upper)
    if lower_cost < upper_cost:
      print(f'Best: {lower_cost} to move to {lower}')
    else:
      print(f'Best: {upper_cost} to move to {upper}')

if __name__ == "__main__":
  main()
