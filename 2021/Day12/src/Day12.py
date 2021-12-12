import argparse

class Cave:
  '''A cave in a massive subterranean subsystem.'''
  def __init__(self, name):
    self.name = name
    self.is_small = name.lower() == name
    self.neighbors = set()

  def add_neighbor(self, cave):
    '''Denotes that this cave is adjacent to another cave'''
    self.neighbors.add(cave)

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

def traverse(caves, current_name, extra_visit_available=False, visited=set(), skip_remove=False):
  '''
  Searches depth-first through the cave system for all possible paths to end.
  '''
  paths = 0

  current = caves[current_name]
  if current.is_small:
    visited.add(current_name)

  for neighbor in current.neighbors:
    if neighbor == 'end':
      paths += 1
    elif neighbor not in visited:
      paths += traverse(caves, neighbor, extra_visit_available, visited)
    elif caves[neighbor].is_small and extra_visit_available:
      paths += traverse(caves, neighbor, extra_visit_available=False, visited=visited, skip_remove=True)

  if current.is_small and not skip_remove:
    visited.remove(current_name)

  return paths

def find_all_paths(caves, part):
  '''Searches for all possible paths from start to end.'''
  if part == 1:
    return traverse(caves, 'start')
  elif part == 2:
    return traverse(caves, 'start', extra_visit_available=True)

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
