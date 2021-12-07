import argparse
import collections

def read_fish(filename):
  '''Reads the initial state of fish from the given file.'''
  fish_strs = None
  with open(filename, 'r') as fh:
    fish_strs = fh.readline().strip().split(',')

  return collections.Counter(map(int, fish_strs))

def simulate_day(fish):
  '''Simulates a single day of fish population growth'''
  fish_to_add = 0
  new_fish = collections.Counter()
  for index, fish_count in fish.items():
    if index == 0:
      fish_to_add = fish_count
    else:
      new_fish[index - 1] = fish_count

  new_fish[8] = fish_to_add
  new_fish[6] += fish_to_add
  return new_fish

def simulate_days(fish, days):
  '''Simulates growth of the fish for a number of days'''
  for _ in range(days):
    fish = simulate_day(fish)

  return sum([value for _, value in fish.items()])

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
