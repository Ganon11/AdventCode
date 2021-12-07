import argparse

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
    optimal_cost = None
    optimal_position = None
    for position in range(min(crabs), max(crabs) + 1):
      local_cost = get_real_cost(crabs, position)
      if optimal_cost is None or local_cost < optimal_cost:
        optimal_cost = local_cost
        optimal_position = position
    print(f'Best: {optimal_cost} to move to {optimal_position}')

if __name__ == "__main__":
  main()
