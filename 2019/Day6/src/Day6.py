import argparse
import os

class SpaceObject:
  def __init__(self, name, parent=None):
    self.name = name
    self.parent = parent
    if self.parent is not None:
      self.parent.add_child(self)
    self.children = list()

  def add_child(self, child):
    self.children.append(child)

  def get_children_set(self):
    children = set(map(lambda o: o.name, self.children))
    for child in self.children:
      children.update(child.get_children_set())

    return children

  def get_path_to(self, target):
    if target in map(lambda o: o.name, self.children):
      return [self.name, target]

    for child in self.children:
      path = child.get_path_to(target)
      if path is not None:
        return [self.name] + path

    return None

  def __repr__(self):
    return str(self)

  def __str__(self):
    children_len = len(self.children)
    children_names = list(map(lambda o: o.name, self.children))
    return f'Object {self.name} has {children_len} children {children_names}'

def part1(objects):
  queue = objects['COM'].children
  seen = set(['COM'])
  total = 0
  level = 1
  while len(queue) > 0:
    next_queue = []
    for node in queue:
      if node.name not in seen:
        total += level
        next_queue.extend(node.children)
        seen.add(node.name)
    level += 1
    queue = next_queue
  print(total)

def part2(objects):
  path_to_you = objects['COM'].get_path_to('YOU')
  path_to_santa = objects['COM'].get_path_to('SAN')
  path = list(set(path_to_you) ^ set(path_to_santa))
  print(len(path) - 2)

def get_space_objects(filename):
  lines = None
  with open(filename, 'r') as file:
    lines = file.readlines()

  objects = dict()
  for line in lines:
    (parent, child) = line.strip().split(')')
    parent_object = None
    if parent in objects:
      parent_object = objects[parent]
    else:
      parent_object = SpaceObject(parent)
      objects[parent] = parent_object

    child_object = None
    if child in objects:
      child_object = objects[child]
      parent_object.add_child(child_object)
    else:
      child_object = SpaceObject(child, parent_object)
      objects[child] = child_object

  return objects

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('-s', '--use-sample', action='store_true')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  filename = os.path.join(os.path.dirname(os.path.dirname(__file__)), 'input')
  if args.use_sample:
    filename = os.path.join(filename, 'sample.txt')
  else:
    filename = os.path.join(filename, 'input.txt')

  objects = get_space_objects(filename)

  if args.part == 1:
    part1(objects)
  else:
    if args.use_sample:
      objects['YOU'] = SpaceObject('YOU', objects['K'])
      objects['SAN'] = SpaceObject('SAN', objects['I'])
    part2(objects)

if __name__ == "__main__":
  main()
