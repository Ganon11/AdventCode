import argparse

def read_fish(filename):
  '''Reads the initial state of fish from the given file.'''
  fish_strs = None
  with open(filename, 'r') as fh:
    fish_strs = fh.readline().strip().split(',')

  fish_list = [0, 0, 0, 0, 0, 0, 0, 0, 0]
  for fish in map(int, fish_strs):
    fish_list[fish] += 1

  return fish_list

def simulate_day(fish):
  '''Simulates a single day of fish population growth'''
  # fish can only be between 0 and 8
  fish_to_add = 0
  for index, fish_count in enumerate(fish):
    if index == 0:
      fish_to_add = fish_count
    else:
      fish[index - 1] = fish[index]
    fish[index] = 0

  if fish_to_add > 0:
    fish[8] = fish_to_add
    fish[6] += fish_to_add

  return fish

def simulate_days(fish, days):
  '''Simulates growth of the fish for a number of days'''
  for _ in range(days):
    fish = simulate_day(fish)

  return sum(fish)

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
