import argparse
import re

class SueCandidate:
  _ID_PATTERN = re.compile('Sue (\d+)', re.IGNORECASE)
  _CHILDREN_PATTERN = re.compile('children: (\d+)', re.IGNORECASE)
  _CATS_PATTERN = re.compile('cats: (\d+)', re.IGNORECASE)
  _SAMOYEDS_PATTERN = re.compile('samoyeds: (\d+)', re.IGNORECASE)
  _POMERANIANS_PATTERN = re.compile('pomeranians: (\d+)', re.IGNORECASE)
  _AKITAS_PATTERN = re.compile('akitas: (\d+)', re.IGNORECASE)
  _VIZSLAS_PATTERN = re.compile('vizslas: (\d+)', re.IGNORECASE)
  _GOLDFISH_PATTERN = re.compile('goldfish: (\d+)', re.IGNORECASE)
  _TREES_PATTERN = re.compile('trees: (\d+)', re.IGNORECASE)
  _CARS_PATTERN = re.compile('cars: (\d+)', re.IGNORECASE)
  _PERFUMES_PATTERN = re.compile('perfumes: (\d+)', re.IGNORECASE)

  def __init__(self, line):
    id_match = SueCandidate._ID_PATTERN.search(line)
    if not id_match:
      raise 'Invalid format'
    self.id = int(id_match.group(1))

    children_match = SueCandidate._CHILDREN_PATTERN.search(line)
    if children_match:
      self.children = int(children_match.group(1))
    else:
      self.children = None

    cats_match = SueCandidate._CATS_PATTERN.search(line)
    if cats_match:
      self.cats = int(cats_match.group(1))
    else:
      self.cats = None

    samoyeds_match = SueCandidate._SAMOYEDS_PATTERN.search(line)
    if samoyeds_match:
      self.samoyeds = int(samoyeds_match.group(1))
    else:
      self.samoyeds = None

    pomeranians_match = SueCandidate._POMERANIANS_PATTERN.search(line)
    if pomeranians_match:
      self.pomeranians = int(pomeranians_match.group(1))
    else:
      self.pomeranians = None

    akitas_match = SueCandidate._AKITAS_PATTERN.search(line)
    if akitas_match:
      self.akitas = int(akitas_match.group(1))
    else:
      self.akitas = None

    vizslas_match = SueCandidate._VIZSLAS_PATTERN.search(line)
    if vizslas_match:
      self.vizslas = int(vizslas_match.group(1))
    else:
      self.vizslas = None

    goldfish_match = SueCandidate._GOLDFISH_PATTERN.search(line)
    if goldfish_match:
      self.goldfish = int(goldfish_match.group(1))
    else:
      self.goldfish = None

    trees_match = SueCandidate._TREES_PATTERN.search(line)
    if trees_match:
      self.trees = int(trees_match.group(1))
    else:
      self.trees = None

    cars_match = SueCandidate._CARS_PATTERN.search(line)
    if cars_match:
      self.cars = int(cars_match.group(1))
    else:
      self.cars = None

    perfumes_match = SueCandidate._PERFUMES_PATTERN.search(line)
    if perfumes_match:
      self.perfumes = int(perfumes_match.group(1))
    else:
      self.perfumes = None

  def matches_children(self, children):
    if self.children is None:
      return True

    return self.children == children

  def matches_cats(self, cats):
    if self.cats is None:
      return True

    return self.cats == cats

  def matches_samoyeds(self, samoyeds):
    if self.samoyeds is None:
      return True

    return self.samoyeds == samoyeds

  def matches_pomeranians(self, pomeranians):
    if self.pomeranians is None:
      return True

    return self.pomeranians == pomeranians

  def matches_akitas(self, akitas):
    if self.akitas is None:
      return True

    return self.akitas == akitas

  def matches_vizslas(self, vizslas):
    if self.vizslas is None:
      return True

    return self.vizslas == vizslas

  def matches_goldfish(self, goldfish):
    if self.goldfish is None:
      return True

    return self.goldfish == goldfish

  def matches_trees(self, trees):
    if self.trees is None:
      return True

    return self.trees == trees

  def matches_cars(self, cars):
    if self.cars is None:
      return True

    return self.cars == cars

  def matches_perfumes(self, perfumes):
    if self.perfumes is None:
      return True

    return self.perfumes == perfumes

  def greater_cats(self, cats):
    if self.cats is None:
      return True

    return self.cats > cats

  def greater_trees(self, trees):
    if self.trees is None:
      return True

    return self.trees > trees

  def less_pomeranians(self, pomeranians):
    if self.pomeranians is None:
      return True

    return self.pomeranians < pomeranians

  def less_goldfish(self, goldfish):
    if self.goldfish is None:
      return True

    return self.goldfish < goldfish

def get_sues(filename):
  sues = list()
  with open(filename, 'r') as fp:
    for line in fp.readlines():
      sues.append(SueCandidate(line))

  return sues

def part_1(filename):
  sues = get_sues(filename)

  sues = list(filter(lambda x: x.matches_children(3), sues))
  sues = list(filter(lambda x: x.matches_cats(7), sues))
  sues = list(filter(lambda x: x.matches_samoyeds(2), sues))
  sues = list(filter(lambda x: x.matches_pomeranians(3), sues))
  sues = list(filter(lambda x: x.matches_akitas(0), sues))
  sues = list(filter(lambda x: x.matches_vizslas(0), sues))
  sues = list(filter(lambda x: x.matches_goldfish(5), sues))
  sues = list(filter(lambda x: x.matches_trees(3), sues))
  sues = list(filter(lambda x: x.matches_cars(3), sues))
  sues = list(filter(lambda x: x.matches_perfumes(3), sues))

  if 1 == len(sues):
    print("Found target Sue: %d" % sues[0].id)

def part_2(filename):
  sues = get_sues(filename)

  sues = list(filter(lambda x: x.matches_children(3), sues))
  sues = list(filter(lambda x: x.matches_samoyeds(2), sues))
  sues = list(filter(lambda x: x.matches_akitas(0), sues))
  sues = list(filter(lambda x: x.matches_vizslas(0), sues))
  sues = list(filter(lambda x: x.matches_cars(3), sues))
  sues = list(filter(lambda x: x.matches_perfumes(3), sues))

  print("Starting with %d Sues" % len(sues))
  sues = list(filter(lambda x: x.greater_cats(7), sues))
  print("Filtered cats to %d Sues" % len(sues))
  sues = list(filter(lambda x: x.less_pomeranians(3), sues))
  print("Filtered pomeranians to %d Sues" % len(sues))
  sues = list(filter(lambda x: x.less_goldfish(5), sues))
  print("Filtered goldfish to %d Sues" % len(sues))
  sues = list(filter(lambda x: x.greater_trees(3), sues))
  print("Filtered trees to %d Sues" % len(sues))

  if 1 == len(sues):
    print("Found target Sue: %d" % sues[0].id)

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('filename')
  args = parser.parse_args()

  part_1(args.filename)
  part_2(args.filename)

if __name__ == "__main__":
  main()
