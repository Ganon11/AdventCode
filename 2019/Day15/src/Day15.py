import argparse
from collections import defaultdict
import os

import intcode
import my_collections
from position import Position

class Maze: # pylint: disable=C0115
  CLEAR_SCREEN = lambda: os.system('cls' if os.name == 'nt' else 'clear')

  UNKNOWN = 0
  OPEN = 1
  WALL = 2
  OXYGEN_SYSTEM = 3

  TILES = {
    UNKNOWN: ' ',
    OPEN: '.',
    WALL: '#',
    OXYGEN_SYSTEM: 'O'
  }

  def __init__(self, program_text):
    self.program = intcode.IntCodeProgram.from_text(program_text)
    self.maze = defaultdict(lambda: Maze.UNKNOWN)
    self.drone = Position(0, 0)
    self.maze[self.drone] = Maze.OPEN
    self.oxygen_system = None

  def draw(self): # pylint: disable=C0116
    minx = min(self.maze.keys(), key=lambda p: p.x).x
    maxx = max(self.maze.keys(), key=lambda p: p.x).x
    miny = min(self.maze.keys(), key=lambda p: p.y).y
    maxy = max(self.maze.keys(), key=lambda p: p.y).y

    Maze.CLEAR_SCREEN()
    for y in reversed(range(miny, maxy + 1)):
      for x in range(minx, maxx + 1):
        position = Position(x, y)
        tile = None
        if self.drone == position:
          tile = 'D'
        else:
          tile = Maze.TILES[self.maze[position]]
        print(tile, end='')
      print('', flush=True)

    if self.oxygen_system is not None:
      print(f'Oxygen System at {self.oxygen_system}')

  @staticmethod
  def _get_user_input(): # pylint: disable=C0116
    while True:
      direction = input('Enter a direction (North: 1, South: 2, West: 3, East: 4) or quit (Q): ')
      if direction in ['Q', 'q']:
        return -1

      try:
        int_direction = int(direction)
      except ValueError:
        print('Enter a valid integer')
        continue

      if not 1 <= int_direction <= 4:
        print('Enter an integer between 1 and 4')
        continue

      return int_direction

  def _get_new_position(self, direction): # pylint: disable=C0116
    if direction == 1:
      return self.drone.north()

    if direction == 2:
      return self.drone.south()

    if direction == 3:
      return self.drone.west()

    return self.drone.east()

  def interactive_explore(self): # pylint: disable=C0116
    while not self.program.has_halted:
      self.draw()
      int_direction = Maze._get_user_input()
      if int_direction == -1:
        print('Quitting...')
        break

      self.program.provide_input(int_direction)
      self.program.execute()
      if len(self.program.output) != 1:
        raise Exception('No output!')
      output = self.program.output.pop(0)

      new_position = self._get_new_position(int_direction)

      if output == 0:
        self.maze[new_position] = Maze.WALL
      elif output == 1:
        self.maze[new_position] = Maze.OPEN
        self.drone = new_position
      elif output == 2:
        system_position = new_position
        self.maze[new_position] = Maze.OXYGEN_SYSTEM
        self.drone = new_position

  def _handle_program_output(self, output, new_position):
    if output == 0: # Hit a wall - position has not changed
      self.maze[new_position] = Maze.WALL
      return False

    if output == 1: # Moved into an open space
      self.maze[new_position] = Maze.OPEN
      self.drone = new_position
    elif output == 2: # Found the OXYGEN_SYSTEM and moved into the space
      self.oxygen_system = new_position
      self.maze[new_position] = Maze.OXYGEN_SYSTEM
      self.drone = new_position

    return True

  @staticmethod
  def _get_opposing_movement_direction(direction):
    if direction == 1:
      return 2

    if direction == 2:
      return 1

    if direction == 3:
      return 4

    return 3

  def _move(self, direction):
    new_position = self._get_new_position(direction)
    self.program.provide_input(direction)
    self.program.execute()
    output = self.program.output.pop(0)
    return self._handle_program_output(output, new_position)

  def _fill_in_blanks(self): # pylint: disable=C0116
    assert self.oxygen_system is not None
    minx = min(self.maze.keys(), key=lambda p: p.x).x
    maxx = max(self.maze.keys(), key=lambda p: p.x).x
    miny = min(self.maze.keys(), key=lambda p: p.y).y
    maxy = max(self.maze.keys(), key=lambda p: p.y).y

    for x in range(minx, maxx + 1):
      for y in range(miny, maxy + 1):
        position = Position(x, y)
        if self.maze[position] == Maze.UNKNOWN:
          self.maze[position] = Maze.WALL

  def _explore(self, debug_print):
    if debug_print:
      self.draw()

    for direction in range(1, 5):
      new_position = self._get_new_position(direction)
      if self.maze[new_position] == Maze.UNKNOWN:
        if self._move(direction):
          self._explore(debug_print)
          self._move(Maze._get_opposing_movement_direction(direction))

  def explore(self, debug_print): # pylint: disable=C0116
    self._explore(debug_print)
    self._fill_in_blanks()

  def calculate_shortest_path(self): # pylint: disable=C0116
    assert self.oxygen_system is not None

    start = Position(0, 0)
    frontier = my_collections.PriorityQueue()
    frontier.put(start, 0)
    came_from = {}
    cost_so_far = {}
    came_from[start] = None
    cost_so_far[start] = 0

    while not frontier.empty():
      current = frontier.get()

      if current == self.oxygen_system:
        break

      possible_neighbors = list()
      possible_neighbors.append(current.north())
      possible_neighbors.append(current.south())
      possible_neighbors.append(current.west())
      possible_neighbors.append(current.east())

      for neighbor in possible_neighbors:
        if self.maze[neighbor] == Maze.WALL:
          continue

        new_cost = cost_so_far[current] + 1
        if neighbor not in cost_so_far or new_cost < cost_so_far[neighbor]:
          cost_so_far[neighbor] = new_cost
          priority = new_cost + self.oxygen_system.distance(neighbor)
          frontier.put(neighbor, priority)
          came_from[neighbor] = current

    return cost_so_far[self.oxygen_system]

  def calculate_longest_path(self): # pylint: disable=C0116
    assert self.oxygen_system is not None

    frontier = my_collections.Queue()
    frontier.put(self.oxygen_system)
    cost_so_far = {}
    cost_so_far[self.oxygen_system] = 0

    while not frontier.empty():
      current = frontier.get()

      possible_neighbors = list()
      possible_neighbors.append(current.north())
      possible_neighbors.append(current.south())
      possible_neighbors.append(current.west())
      possible_neighbors.append(current.east())

      for neighbor in possible_neighbors:
        if self.maze[neighbor] == Maze.WALL:
          continue

        if neighbor not in cost_so_far:
          frontier.put(neighbor)
          cost_so_far[neighbor] = cost_so_far[current] + 1

    furthest_node = max(cost_so_far, key=cost_so_far.get)
    return cost_so_far[furthest_node]

def main(): # pylint: disable=C0116,R0912
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample1.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  parser.add_argument('-i', '--interactive', action='store_true')
  parser.add_argument('-d', '--debug', action='store_true')
  args = parser.parse_args()

  program_text = None
  with open(args.filename, 'r') as file:
    program_text = file.read().rstrip()

  maze = Maze(program_text)
  if args.interactive:
    maze.interactive_explore()
  else:
    maze.explore(args.debug)
    maze.draw()
    if args.part == 1:
      steps = maze.calculate_shortest_path()
      print(f'Steps from origin to oxygen system: {steps}')
    elif args.part == 2:
      steps = maze.calculate_longest_path()
      print(f'Oxygen filled the system in {steps} minutes')

if __name__ == "__main__":
  main()
