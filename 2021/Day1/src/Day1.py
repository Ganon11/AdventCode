import argparse

def get_depths(filename):
  '''Gets the list of depths from the file.'''
  depths = list()
  with open(filename, 'r') as file:
    depths = [int(x) for x in file.readlines()]

  return depths

def check_sliding_window(depths, window=1):
  '''Checks for decreasing depth with an optional window for moving averages.'''
  num_depths = len(depths)

  if num_depths <= window:
    return 0

  old_sum = sum(depths[:window])
  index = window
  total = 0
  while index < num_depths:
    start = index - window + 1
    end = index + 1
    new_sum = sum(depths[start:end])
    if new_sum > old_sum:
      total = total + 1

    old_sum = new_sum
    index = index + 1

  return total

def main():
  '''Where are we even going, anyway?'''
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  parser.add_argument('-w', '--window', default=3, type=int)
  args = parser.parse_args()

  depths = get_depths(args.filename)
  if args.part == 1:
    deeper = check_sliding_window(depths)
  else:
    deeper = check_sliding_window(depths, args.window)

  print(f'Got deeper {deeper} times.')

if __name__ == "__main__":
  main()
