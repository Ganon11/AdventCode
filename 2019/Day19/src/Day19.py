import argparse

import intcode
from position import Position

def print_field(field): # pylint: disable=C0116
  positions = sorted(field.keys())

  last_position = None
  for position in positions:
    if last_position is not None and last_position.y != position.y:
      print('', flush=True)
      print(' ' * position.x, end='')

    if field[position] == 1:
      print('#', end='')
    else:
      print(' ', end='')

    last_position = position

  # One last newline for the kids
  print('', flush=True)

def find_beam_in_square(program_text, square_size): # pylint: disable=C0116
  field = dict()
  for row in range(square_size):
    for col in range(square_size):
      position = Position(col, row)
      program = intcode.IntCodeProgram.from_text(program_text)
      program.provide_input([col, row])
      program.execute()
      field[position] = program.output.pop(0)
  count = sum(field.values())
  return count

def find_square_in_beam(field, size): # pylint: disable=C0116
  minx = min(field.keys(), key=lambda p: p.x).x
  maxx = max(field.keys(), key=lambda p: p.x).x
  miny = min(field.keys(), key=lambda p: p.y).y
  maxy = max(field.keys(), key=lambda p: p.y).y

  for x in range(minx, maxx + 1):
    for y in range(miny, maxy + 1):
      if x + size - 1 > maxx or y + size - 1 > maxy:
        return None # Didn't find it

      top_left_corner = Position(x, y)
      if field[top_left_corner] != 1:
        continue # Can't start a box on an empty place

      valid_square = True
      for additional_x in range(size):
        if field[Position(top_left_corner.x + additional_x, top_left_corner.y)] != 0:
          valid_square = False
          break

      if not valid_square:
        continue # A square must have all 1s horizontally

      for additional_y in range(size):
        if field[Position(top_left_corner.x, top_left_corner.y + additional_y)] != 0:
          valid_square = False
          break

      if not valid_square:
        continue # A square must have all 1s vertically

      # This is the start of a valid square!
      return top_left_corner

  # Nothing found in field
  return None


def main(): # pylint: disable=C0116
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample1.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  parser.add_argument('-s', '--size', type=int)
  args = parser.parse_args()

  program_text = None
  with open(args.filename, 'r') as file:
    program_text = file.read().rstrip()

  if args.part == 1:
    count = find_beam_in_square(program_text, args.size)
    print(f'{count} positions affected by tractor beam')
  elif args.part == 2:
    # Shamelessly stealing this approach from reddit. It's clever. Sue me.

    # Starting at a particular column, we walk downward to find the first row where the beam hits.
    # Because we need to check for a box of size 100, we start at column 101
    # Even if the beam's size exploded at a 1-to-1 rate (i.e. a box of size Z can be found at point
    # (Z, Z)), the first place a 100-size box could be is at column 100.
    # Also we need to do a subtraction of 99, so this makes us not go negative.
    x = args.size - 1

    # Start at row 0.
    y = 0
    while True:
      while True:
        # Check the current (x, y)
        program = intcode.IntCodeProgram.from_text(program_text)
        program.provide_input([x, y])
        program.execute()

        if program.output.pop(0) == 1:
          # We've found the top edge of the beam!
          break

        # We haven't found the beam yet - keep descending.
        y += 1

      # In order for this to be a square, the current (x, y) must be the top-right corner of that
      # square. The bottom-left corner of the square is then at (x-99, y+99).
      program = intcode.IntCodeProgram.from_text(program_text)
      program.provide_input([x - (args.size - 1), y + (args.size - 1)])
      program.execute()

      #If this position also is in the beam, we've found our square.
      if program.output.pop(0) == 1:
        print(f'Found the box at {x - (args.size - 1)}, {y}')
        print(f'x * 10000 + y = {(x - (args.size - 1)) * 10000 + y}')
        break

      # We haven't gotten wide enough yet - increase the column and descend further as needed.
      x += 1

if __name__ == "__main__":
  main()
