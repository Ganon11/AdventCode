import argparse
import os
from collections import defaultdict

def double_digits(value):
  value_str = str(value)
  for i in range(0, 5):
    if value_str[i] == value_str[i + 1]:
      return True
  return False

def increasing_digits(value):
  value_str = str(value)
  digit = value_str[0]
  for i in range(1, 6):
    if value_str[i] < digit:
      return False
    digit = value_str[i]

  return True

def double_is_not_part_of_triple(value):
  value_str = str(value)
  digits = defaultdict(int)
  for i in range(0, 6):
    digits[value_str[i]] += 1

  return any(digits[key] == 2 for key in digits)

def check(value, part):
  if not double_digits(value):
    return False

  if not increasing_digits(value):
    return False

  if part == 2 and not double_is_not_part_of_triple(value):
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
    with open(args.filename, 'r') as f:
      line = f.read()
      args.start = int(line[0:6])
      args.end = int(line[7:])

  total = 0
  for i in range(args.start, args.end + 1):
    if check(i, args.part):
      total += 1

  print(f'{total} passwords pass')


if __name__ == "__main__":
  main()