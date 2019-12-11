import argparse
from collections import defaultdict

import intcode
from position import Position

class HullPaintingRobot: # pylint: disable=C0115
  def __init__(self, filename, part):
    program = None
    with open(filename, 'r') as file:
      program = [int(value) for value in file.read().split(',')]
    self._program = intcode.IntCodeProgram(program)
    self._field = defaultdict(lambda: False)
    self._position = Position(0, 0)
    self._direction = 'UP'
    self._painted_panels = set()
    if part == 2:
      self._field[self._position] = True

  def draw_field(self): # pylint: disable=C0116
    minx = min(self._field.keys(), key=lambda p: p.x).x
    maxx = max(self._field.keys(), key=lambda p: p.x).x
    miny = min(self._field.keys(), key=lambda p: p.y).y
    maxy = max(self._field.keys(), key=lambda p: p.y).y

    for y in range(miny, maxy + 1):
      for x in range(minx, maxx + 1):
        position = Position(x, y)
        if self._field[position]:
          print('#', end='')
        else:
          print(' ', end='')
      print('')

  def count_painted(self):
    return len(self._painted_panels)

  def _rotate(self, rotation):
    if rotation == 1:
      # Rotate clockwise
      if self._direction == 'UP':
        self._direction = 'RIGHT'
      elif self._direction == 'RIGHT':
        self._direction = 'DOWN'
      elif self._direction == 'DOWN':
        self._direction = 'LEFT'
      elif self._direction == 'LEFT':
        self._direction = 'UP'
    elif rotation == 0:
      # Rotate counter-clockwise
      if self._direction == 'UP':
        self._direction = 'LEFT'
      elif self._direction == 'LEFT':
        self._direction = 'DOWN'
      elif self._direction == 'DOWN':
        self._direction = 'RIGHT'
      elif self._direction == 'RIGHT':
        self._direction = 'UP'

  def _step(self):
    if self._direction == 'UP':
      self._position = Position(self._position.x, self._position.y - 1)
    elif self._direction == 'DOWN':
      self._position = Position(self._position.x, self._position.y + 1)
    elif self._direction == 'LEFT':
      self._position = Position(self._position.x - 1, self._position.y)
    elif self._direction == 'RIGHT':
      self._position = Position(self._position.x + 1, self._position.y)

  def run(self): # pylint: disable=C0116
    self._program.execute()
    while not self._program.has_halted:
      while bool(self._program.output):
        paint = self._program.output.pop(0)
        if paint == 1:
          self._painted_panels.add(self._position)
        self._field[self._position] = bool(paint == 1)
        self._rotate(self._program.output.pop(0))
        self._step()
      if self._field[self._position]:
        self._program.provide_input(1)
      else:
        self._program.provide_input(0)
      self._program.execute()

def main(): # pylint: disable=C0116
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample1.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  robot = HullPaintingRobot(args.filename, args.part)
  robot.run()
  robot.draw_field()
  print(f'Robot painted {robot.count_painted()} panels')

if __name__ == "__main__":
  main()
