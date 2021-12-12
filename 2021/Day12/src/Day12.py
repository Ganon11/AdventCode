import argparse

class Cave:
  '''A cave in a massive subterranean subsystem.'''
  def __init__(self, name):
    self.name = name
    self.is_small = name.lower() == name
    self.neighbors = set()
    self.visited = False

  def add_neighbor(self, cave):
    '''Denotes that this cave is adjacent to another cave'''
    self.neighbors.add(cave)

  def visit(self):
    '''Mark the cave as visited.'''
    self.visited = True

  def unvisit(self):
    '''Mark the cave as unvisited.'''
    self.visited = False

  def __str__(self):
    mystr = self.name
    if self.is_small:
      mystr += ' (small)'
    mystr += '\nNeighbors:\n'
    for neighbor in self.neighbors:
      mystr += f'\t{neighbor}\n'
    return mystr

def get_cave_system(filename):
  '''Gets a graph of the cave system from the file.'''
  lines = None
  with open(filename, 'r') as fh:
    lines = list(map(lambda line: line.strip(), fh.readlines()))

  caves = dict()
  for line in lines:
    (a, b) = line.split('-')
    if a not in caves:
      caves[a] = Cave(a)

    if b not in caves:
      caves[b] = Cave(b)

    if b != 'start':
      caves[a].add_neighbor(b)
    if a != 'start':
      caves[b].add_neighbor(a)

  return caves

def traverse(caves, current, extra_visit_available=False, skip_remove=False):
  '''
  Searches depth-first through the cave system for all possible paths to end.
  '''
  paths = 0

  if current.is_small:
    current.visit()

  for neighbor_name in current.neighbors:
    neighbor = caves[neighbor_name]
    if neighbor_name == 'end':
      paths += 1
    elif not neighbor.visited:
      paths += traverse(caves, neighbor, extra_visit_available)
    elif neighbor.is_small and extra_visit_available:
      paths += traverse(caves, neighbor, extra_visit_available=False, skip_remove=True)

  if current.is_small and not skip_remove:
    current.unvisit()

  return paths

def find_all_paths(caves, part):
  '''Searches for all possible paths from start to end.'''
  if part == 1:
    return traverse(caves, caves['start'])
  elif part == 2:
    return traverse(caves, caves['start'], extra_visit_available=True)

def main():
  '''
  Submarine's subterranean subsystems subsisting suboptimally support a silly
  solution.
  '''
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample1.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  caves = get_cave_system(args.filename)
  path_count = find_all_paths(caves, args.part)
  print(f'Number of paths: {path_count}')

if __name__ == "__main__":
  main()
