import argparse
import math

def get_crabs(filename):
  '''Gets the initial position of the crabs from the file'''
  crabs = None
  with open(filename, 'r') as fh:
    crabs = list(map(int, fh.readline().strip().split(',')))
  return crabs

def main():
  '''Insert crab rave here.'''
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  crabs = get_crabs(args.filename)
  if args.part == 1:
    crabs.sort()
    median_position = crabs[len(crabs) // 2]
    cost = sum(abs(crab - median_position) for crab in crabs)
    print(f'Best: {cost} to move to {median_position}')
  else:
    mean = sum(crabs) / len(crabs)
    lower = math.floor(mean)
    upper = math.ceil(mean)
    lower_cost = sum((abs(crab - lower) * (abs(crab - lower) + 1)) // 2 for crab in crabs)
    upper_cost = sum((abs(crab - upper) * (abs(crab - upper) + 1)) // 2 for crab in crabs)
    if lower_cost < upper_cost:
      print(f'Best: {lower_cost} to move to {lower}')
    else:
      print(f'Best: {upper_cost} to move to {upper}')

if __name__ == "__main__":
  main()
