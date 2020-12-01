import argparse

def get_expenses(filename):
  expenses = list()
  with open(filename, 'r') as file:
    expenses = [int(x) for x in file.readlines()]

  return expenses

def part1(expenses):
  for index in range(0, len(expenses) - 1):
    expense1 = expenses[index]
    for index2 in range(index + 1, len(expenses)):
      expense2 = expenses[index2]

      if expense1 + expense2 == 2020:
        return expense1 * expense2

  return -1

def part2(expenses):
  for index in range(0, len(expenses) - 2):
    expense1 = expenses[index]
    for index2 in range(index + 1, len(expenses) - 1):
      expense2 = expenses[index2]

      for index3 in range(index2 + 1, len(expenses)):
        expense3 = expenses[index3]

        if expense1 + expense2 + expense3 == 2020:
          return expense1 * expense2 * expense3

  return -1

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample1.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  expenses = get_expenses(args.filename)
  checksum = -1
  if args.part == 1:
    checksum = part1(expenses)
  else:
    checksum = part2(expenses)

  if checksum == -1:
    print('No matching expenses found!')
  else:
    print(f'Expense checksum: {checksum}')

if __name__ == "__main__":
  main()
