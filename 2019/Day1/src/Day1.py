import argparse

def mass_fuel(mass):
  return max((mass // 3) - 2, 0)

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample1.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  masses = list()
  with open(args.filename, 'r') as file:
    masses = [int(x) for x in file.readlines()]

  fuels = list(map(mass_fuel, masses))
  total_fuel = sum(fuels)
  if args.part == 1:
    print(total_fuel)
  elif args.part == 2:
    partial = total_fuel
    while partial != 0:
      new_fuels = list(map(mass_fuel, fuels))
      partial = sum(new_fuels)
      total_fuel += partial
      fuels = new_fuels
    print(total_fuel)

if __name__ == "__main__":
  main()
