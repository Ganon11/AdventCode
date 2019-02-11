import argparse

def sum_of_divisors(num):
  return sum([i for i in range(1, num) if num % i == 0])

def part_1(target):
  house = 0
  factor = 100000
  presents = sum_of_divisors(house) * 10
  while True:
    #print("Checking %d presents vs target %d" % (presents, target))
    if presents < target:
      #print("Adding")
      house = house + factor
    else:
      print("Subtracting and reducing factor from %d to %d" % (factor, int(factor / 10)))
      house = house - factor
      factor = int(factor / 10)

    presents = sum_of_divisors(house) * 10
    if factor == 1 and presents >= target:
      break

  print("House %d got %d presents" % (house, presents))

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('-t', '--target', type=int, default=34000000)
  args = parser.parse_args()

  part_1(args.target)

if __name__ == "__main__":
  main()
