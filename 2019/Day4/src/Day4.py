import argparse
from collections import Counter

def double_digits(value, part):
  value_str = str(value)
  digit_counter = Counter(value_str)
  if part == 1:
    return any(v >= 2 for v in digit_counter.values())

  if part == 2:
    return 2 in digit_counter.values()

  return None

def increasing_digits(value):
  value_str = str(value)
  digit = value_str[0]
  for i in range(1, 6):
    if value_str[i] < digit:
      return False
    digit = value_str[i]

  return True

def check(value, part):
  if not increasing_digits(value):
    return False

  if not double_digits(value, part):
    return False

  return True

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename')
  parser.add_argument('-c', '--check')
  parser.add_argument('-s', '--start', default=111111)
  parser.add_argument('-e', '--end', default=999999)
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  if args.check:
    print(check(args.check, args.part))
    return

  if args.filename:
    # read start/end from file
    with open(args.filename, 'r') as file:
      line = file.read()
      args.start = int(line[0:6])
      args.end = int(line[7:])

  total = 0
  for i in range(args.start, args.end + 1):
    if check(i, args.part):
      total += 1

  print(f'{total} passwords pass')


if __name__ == "__main__":
  main()
