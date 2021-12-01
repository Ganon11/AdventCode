import argparse

def get_depths(filename):
  depths = list()
  with open(filename, 'r') as file:
    depths = [int(x) for x in file.readlines()]

  return depths

def check_increments(depths):
  num_depths = len(depths)

  if num_depths <= 1:
    return 0

  index = 1
  total = 0
  while index < num_depths:
    if depths[index] > depths[index - 1]:
      total = total + 1

    index = index + 1

  return total

def check_sliding_window(depths):
  num_depths = len(depths)

  if num_depths <= 3:
    return 0

  old_sum = depths[0] + depths[1] + depths[2]
  index = 3
  total = 0
  while index < num_depths:
    new_sum = depths[index - 2] + depths[index - 1] + depths[index]
    if new_sum > old_sum:
      total = total + 1

    old_sum = new_sum
    index = index + 1

  return total

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  depths = get_depths(args.filename)
  if args.part == 1:
    deeper = check_increments(depths)
  else:
    deeper = check_sliding_window(depths)

  print(f'Got deeper {deeper} times.')

if __name__ == "__main__":
  main()
