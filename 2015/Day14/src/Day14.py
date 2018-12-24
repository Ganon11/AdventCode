import argparse
import re

class Reindeer:
  _REINDEER_PATTERN = re.compile('(\w+) can fly (\d+) km/s for (\d+) seconds, but then must rest for (\d+) seconds.', re.IGNORECASE)
  def __init__(self, line):
    match = Reindeer._REINDEER_PATTERN.match(line)
    self.name = match.group(1)
    self.speed = int(match.group(2))
    self.run_duration = int(match.group(3))
    self.rest_duration = int(match.group(4))
    self.state = 'run'
    self.timestamp = 0
    self.distance = 0
    self.points = 0

  def print(self):
    print("%s has traveled %d km. State: %s (since time %d). Score %d" % (self.name, self.distance, self.state, self.timestamp, self.points))

  def step(self, time):
    if self.state == 'run' and self.run_duration == (time - self.timestamp):
      self.state = 'rest'
      self.timestamp = time
    elif self.state == 'rest' and self.rest_duration == (time - self.timestamp):
      self.state = 'run'
      self.timestamp = time

    if self.state == 'run':
      self.distance = self.distance + self.speed

def get_reindeer(filename):
  lines = None
  with open(filename, 'r') as fp:
    lines = fp.readlines()

  reindeer = list()
  for line in lines:
    reindeer.append(Reindeer(line.rstrip()))

  return reindeer

def part_1(filename, steps):
  all_reindeer = get_reindeer(filename)
  for t in range(steps):
    for r in all_reindeer:
      r.step(t)

  all_reindeer.sort(key=lambda x: x.distance, reverse=True)
  all_reindeer[0].print()

def part_2(filename, steps):
  all_reindeer = get_reindeer(filename)
  for t in range(steps):
    for r in all_reindeer:
      r.step(t)

    all_reindeer.sort(key=lambda x: x.distance, reverse=True)
    lead = all_reindeer[0].distance
    for r in all_reindeer:
      if lead == r.distance:
        r.points = r.points + 1
      else:
        break

  all_reindeer.sort(key=lambda x: x.points, reverse=True)
  all_reindeer[0].print()

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('filename')
  parser.add_argument('-s', '--steps', type=int)
  args = parser.parse_args()

  filename = '../input/Sample.txt'
  if args.filename:
    filename = args.filename

  steps = 2503
  if args.steps:
    steps = int(args.steps)

  part_1(filename, steps)
  part_2(filename, steps)

if __name__ == "__main__":
  main()
