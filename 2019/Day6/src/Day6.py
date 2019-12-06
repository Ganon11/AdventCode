import argparse
from functools import reduce
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

  def distance_to(self, target):
    if target in map(lambda o: o.name, self.children):
      return 1

    for child in self.children:
      child_distance = child.distance_to(target)
      if child_distance is not None:
        return child_distance + 1

    return None

  def __repr__(self):
    return str(self)

  def __str__(self):
    return f'Object {self.name} has {len(self.children)} children'

def part1(objects):
  total = 0
  root = objects['COM']
  for child in root.get_children_set():
    total += root.distance_to(child)

  print(total)

def part2(objects):
  (you, santa) = ('YOU', 'SAN')
  current = objects['COM']
  while True:
    currentChildren = current.get_children_set()
    childFound = False
    for child in current.children:
      childSet = child.get_children_set()
      if you in childSet and santa in childSet:
        childFound = True
        current = child
        break
    if not childFound:
      break

  print(current.distance_to(you) + current.distance_to(santa) - 2)

def get_space_objects(filename):
  lines = None
  with open(filename, 'r') as f:
    lines = f.readlines()

  objects = dict()
  for line in lines:
    (parent, child) = line.strip().split(')')
    parentObject = None
    if parent in objects:
      parentObject = objects[parent]
    else:
      parentObject = SpaceObject(parent)
      objects[parent] = parentObject

    childObject = None
    if child in objects:
      childObject = objects[child]
      parentObject.add_child(childObject)
    else:
      childObject = SpaceObject(child, parentObject)
      objects[child] = childObject

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