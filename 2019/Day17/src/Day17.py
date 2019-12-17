import argparse

import intcode
from position import Position

class Field:
  THE_VAST_EXPANSE_OF_SPACE = ord('.')
  A_PRECIPICE_ON_THE_EDGE_OF_DESPAIR = ord('#')
  A_ROBOT_FACING_UPWARD = ord('^')
  A_ROBOT_FACING_LEFTWARD = ord('<')
  A_ROBOT_FACING_RIGHTWARD = ord('>')
  A_ROBOT_FACING_DOWNWARD = ord('v')
  A_ROBOT_TUMBLING_UNCONTROLLABLY_THROUGH_SPACE = ord('X')
  A_NEW_ROW_OF_TERRIBLE_HORROR = 10 # Newline

  def __init__(self, characters):
    #print('__init__')
    self.field = dict()
    x = 0
    y = 0
    for character in characters:
      print(chr(character), end='')
      if character == Field.A_NEW_ROW_OF_TERRIBLE_HORROR:
        x += 1
        y = 0
        continue

      position = Position(x, y)
      self.field[position] = character

      y += 1

  def draw(self):
    print('draw')
    maxx = max(self.field.keys(), key=lambda p: p.x).x
    maxy = max(self.field.keys(), key=lambda p: p.y).y

    for x in range(maxx + 1):
      for y in range(maxy + 1):
        print(chr(self.field[Position(x, y)]), end='')
      print('', flush=True)
    #for y in range(maxy + 1):
    #  for x in range(maxx + 1):
    #    position = Position(x, y)
    #    #print(chr(self.field[Position(x, y)]), end='')
    #    print(f'{position}={chr(self.field[position])}', end='')
    #  print('', flush=True)

  def find_intersections(self):
    maxx = max(self.field.keys(), key=lambda p: p.x).x
    maxy = max(self.field.keys(), key=lambda p: p.y).y

    intersections = list()
    for x in range(1, maxx):
      for y in range(1, maxy):
        position = Position(x, y)
        north = position.north()
        south = position.south()
        east = position.east()
        west = position.west()

        if (self.field[position] == Field.A_PRECIPICE_ON_THE_EDGE_OF_DESPAIR and
            self.field[north] == Field.A_PRECIPICE_ON_THE_EDGE_OF_DESPAIR and
            self.field[south] == Field.A_PRECIPICE_ON_THE_EDGE_OF_DESPAIR and
            self.field[east] == Field.A_PRECIPICE_ON_THE_EDGE_OF_DESPAIR and
            self.field[west] == Field.A_PRECIPICE_ON_THE_EDGE_OF_DESPAIR):
          intersections.append(position)

    return intersections

  @staticmethod
  def calculate_alignment_parameters(position):
    return position.x * position.y

def translate_routine_to_ASCII(routine):
  ascii_list = list()
  for character in routine:
    ascii_list.append(ord(character))

  ascii_list.append(Field.A_NEW_ROW_OF_TERRIBLE_HORROR)
  return ascii_list

def draw_program_output(output):
  for character in output:
    try:
      draw = chr(character)
    except:
      draw = character
    print(draw, end='')
  print('', flush=True)

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample1.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  parser.add_argument('-i', '--interactive', action='store_true')
  args = parser.parse_args()

  program_text = None
  with open(args.filename, 'r') as file:
    program_text = file.read().rstrip()

  program = intcode.IntCodeProgram.from_text(program_text)
  if args.part == 1:
    program.execute()
    field = Field(program.output)
    field.draw()
    intersections = field.find_intersections()
    alignment_sum = sum(map(lambda p: Field.calculate_alignment_parameters(p), intersections))
    print(f'Alignment sum: {alignment_sum}')
  elif args.part == 2:
    program.memory[0] = 2 # Free play... ?
    program.execute()
    draw_program_output(program.output)
    program.output = list()

    # Main routine: A, B, A, B, C, C, B, A, B, C
    main_routine = ','.join(["A", "B", "A", "B", "C", "C", "B", "A", "B", "C"])
    main_ascii = translate_routine_to_ASCII(main_routine)
    program.provide_input(main_ascii)
    program.execute()
    draw_program_output(program.output)
    program.output = list()

    #  A: L, 10, R, 10, L, 10, L, 10
    a_routine = ','.join(["L", "10", "R", "10", "L", "10", "L", "10"])
    a_ascii = translate_routine_to_ASCII(a_routine)
    program.provide_input(a_ascii)
    program.execute()
    draw_program_output(program.output)
    program.output = list()

    #  B: R, 10, R, 12, L, 12
    b_routine = ','.join(["R", "10", "R", "12", "L", "12"])
    b_ascii = translate_routine_to_ASCII(b_routine)
    program.provide_input(b_ascii)
    program.execute()
    draw_program_output(program.output)
    program.output = list()

    #  C: R, 12, L, 12, R, 6
    c_routine = ','.join(["R", "12", "L", "12", "R", "6"])
    c_ascii = translate_routine_to_ASCII(c_routine)
    program.provide_input(c_ascii)
    program.execute()
    draw_program_output(program.output)
    program.output = list()

    # Continuous video feed?
    if args.interactive:
      program.provide_input(ord('y'))
    else:
      program.provide_input(ord('n'))
    program.provide_input(Field.A_NEW_ROW_OF_TERRIBLE_HORROR)
    program.execute()
    draw_program_output(program.output)

if __name__ == "__main__":
  main()
