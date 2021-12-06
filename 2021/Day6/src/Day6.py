import argparse
from collections import defaultdict

def read_fish(filename):
  '''Reads the initial state of fish from the given file.'''
  fish_strs = None
  with open(filename, 'r') as fh:
    fish_strs = fh.readline().strip().split(',')

  fish_dict = defaultdict(int)
  for fish in map(int, fish_strs):
    fish_dict[fish] += 1

  return fish_dict

def simulate_day(fish):
  '''Simulates a single day of fish population growth'''
  # fish can only be between 0 and 8
  fish_to_add = 0
  for number in sorted(fish):
    if number == 0:
      fish_to_add = fish[number]
    else:
      fish[number - 1] = fish[number]
    fish[number] = 0

  if fish_to_add > 0:
    fish[8] = fish_to_add
    fish[6] += fish_to_add

  return fish

def count_fish(fish):
  '''Counts the number of fish in the sea'''
  total = 0
  for number in sorted(fish):
    #print(f'\t{fish[number]} of {number}')
    total += fish[number]
  #print(f'Total fish: {total}')
  return total

def simulate_days(fish, days):
  '''Simulates growth of the fish for a number of days'''
  total = count_fish(fish)

  for _ in range(days):
    fish = simulate_day(fish)
    total = count_fish(fish)

  return total

def main():
  '''There's always another fish in the sea.'''
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample1.txt')
  parser.add_argument('-d', '--days', default=80, type=int)
  args = parser.parse_args()

  fish = read_fish(args.filename)
  total = simulate_days(fish, args.days)
  print(f'After {args.days} days, there are {total} fish')

if __name__ == "__main__":
  main()
