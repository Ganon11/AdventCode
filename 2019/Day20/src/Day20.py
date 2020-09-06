import argparse
from collections import defaultdict
import itertools

import my_collections
from position import Position

class Teleporter: #pylint: disable=C0115
  def __init__(self, name, loc1, loc2):
    self.name = name
    self.loc1 = loc1
    self.loc2 = loc2

  def __str__(self):
    return f'{self.name} connects {self.loc1} and {self.loc2}'

  def __repr__(self):
    return str(self)

  def __eq__(self, other):
    return self.name == other.name

class Maze: # pylint: disable=C0115
  HALL = 0
  WALL = 1
  SPACE = 2
  TELEPORTER = 3

  _CHARMAP = {
    HALL: '.',
    WALL: '#',
    SPACE: ' ',
    TELEPORTER: '?'
  }

  _REVERSE_CHARMAP = {
    '.': HALL,
    '#': WALL,
    ' ': SPACE
         # Nothing for teleporters, they are special
  }

  def __init__(self, width, height):
    self.width = width
    self.height = height
    self.teleporters = set()
    self.maze = defaultdict(lambda: Maze.SPACE)

  def in_bounds(self, position): # pylint: disable=C0116
    return 0 <= position.x < self.width and 0 <= position.y < self.height

  def passable(self, position): # pylint: disable=C0116
    return self.maze[position] != Maze.WALL and self.maze[position] != Maze.SPACE

  @staticmethod
  def no_z_component(position): # pylint: disable=C0116
    return position.z == 0

  def is_teleporter(self, position): # pylint: disable=C0116
    if any(position in [t.loc1, t.loc2] for t in self.teleporters):
      return True
    if self.maze[position] not in Maze._CHARMAP:
      return True
    return False

  def neighbors(self, position): # pylint: disable=C0116
    results = position.get_adjacent_positions()
    results = filter(self.in_bounds, results)
    results = filter(self.passable, results)
    results = filter(Maze.no_z_component, results)

    # Replace teleporter entrances with exits, vice versa
    return results

  def add_tile_type(self, position, tile): # pylint: disable=C0116
    if tile in Maze._REVERSE_CHARMAP:
      self.maze[position] = Maze._REVERSE_CHARMAP[tile]
    else: # It's a teleporter!
      self.maze[position] = tile

  def _hook_up_teleporters(self):
    possible_locations = [key for key, value in self.maze if value == Maze.TELEPORTER]
    names = dict()
    for location in possible_locations:
      letter = self.maze[location]
      other_letter = None
      if self.maze[location.north()] == Maze.TELEPORTER:
        pass

      name = f'{letter}{other_letter}'
      reversed_name = f'{letter}{other_letter}'
      if name in names:
        pass
      elif reversed_name in names:
        pass
      else:
        names[name] = location

def main(): # pylint: disable=C0116
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample1.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

if __name__ == "__main__":
  main()
