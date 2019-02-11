import argparse

from functools import reduce

def get_divisors(num):
  return set(reduce(list.__add__, ([i, num // i] for i in range(1, int(num ** 0.5) + 1) if num % i == 0)))

def sum_of_divisors(num):
  return sum(get_divisors(num))

def limited_sum_of_divisors(num):
  return sum(d for d in get_divisors(num) if num / d <= 50) * 11

def part_1(target):
  house = 1
  presents = sum_of_divisors(house) * 10
  while presents < target:
    house = house + 1
    presents = sum_of_divisors(house) * 10
  print("House %d got %d presents" % (house, presents))

def part_2(target):
  house = 1
  presents = limited_sum_of_divisors(house)
  while presents < target:
    house = house + 1
    presents = limited_sum_of_divisors(house)
  print("House %d got %d presents" % (house, presents))

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('-t', '--target', type=int, default=34000000)
  parser.add_argument('-n', '--number', type=int)
  args = parser.parse_args()

  if args.number:
    print(get_divisors(args.number))
  else:
    part_1(args.target)
    part_2(args.target)

if __name__ == "__main__":
  main()
