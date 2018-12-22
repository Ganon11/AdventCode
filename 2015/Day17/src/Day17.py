import argparse
import functools
import itertools

class Container:
  def __init__(self, capacity):
    self.capacity = capacity
    self.in_use = False

def get_containers(filename):
  containers = list()
  with open(filename, 'r') as fp:
    for line in fp.readlines():
      containers.append(Container(int(line.rstrip())))

  return containers

def combinations(containers, goal):
  answers = list()
  for count in range(1, len(containers) + 1):
    all_combinations = itertools.combinations(containers, count)
    for comb in all_combinations:
      if goal == functools.reduce((lambda c1, c2: c1 + c2), map(lambda c: c.capacity, comb)):
        answers.append(comb)

  return answers

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('filename')
  parser.add_argument('-l', '--liters', type=int, default=25)
  args = parser.parse_args()

  containers = get_containers(args.filename)
  answers = combinations(containers, args.liters)

  print("Number of combinations: %d" % len(answers))
  # for answer in answers:
  #   print(','.join(map(lambda c: str(c.capacity), answer)))

  answers.sort(key=lambda c: len(c))
  min_len = len(answers[0])
  short_answers = list(filter(lambda x: min_len == len(x), answers))
  print("Number of small combinations: %d" % len(short_answers))

if __name__ == "__main__":
  main()
