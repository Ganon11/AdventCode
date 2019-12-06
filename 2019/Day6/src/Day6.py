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

  def count_orbits(self):
    if self.parent is None:
      return 0

    return 1 + self.parent.count_orbits()

  def get_children_set(self):
    children = set(map(lambda o: o.name, self.children))
    for child in self.children:
      children.update(child.get_children_set())

    return children

  def get_orbit_chain(self):
    if self.parent is None:
      return [self.name]

    return [self.name] + self.parent.get_orbit_chain()

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
  root = 'COM'

  q = objects[root].children
  seen = set([root])
  distances = 0
  level = 1
  while len(q) > 0:
    #print(f'doing level {level}, len(q): {len(q)}')
    q_next = []
    for node in q:
      #print(node)
      if node.name not in seen:
        distances += level
        neighbors = objects.get(node.name)
        q_next.extend(neighbors.children)
        seen.add(node.name)
    level +=1
    q = q_next
  return distances

def part2(objects):
  (you, santa) = ('YOU', 'SAN')
  current = objects['COM']
  while True:
    currentChildren = current.get_children_set()
    if you not in currentChildren or santa not in currentChildren:
      raise Exception('you dun goofd')

    childFound = False
    for child in current.children:
      childSet = child.get_children_set()
      if you in childSet and santa in childSet:
        childFound = True
        current = child
        break

    if not childFound:
      break

  print(f'lowest common ancestor is {current}')
  #print(f'distance from ancestor to YOU is {current.distance_to(you)}')
  #print(f'distance from ancestor to SAN is {current.distance_to(santa)}')
  print(current.distance_to(you) + current.distance_to(santa) - 2)

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample1.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  lines = None
  with open(args.filename, 'r') as f:
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

  if args.part == 1:
    print(part1(objects))
  else:
    part2(objects)

if __name__ == "__main__":
  main()