import argparse
from collections import defaultdict
import os
from time import sleep

import intcode
from position import Position

CLEAR_SCREEN = lambda: os.system('cls' if os.name == 'nt' else 'clear')
EMPTY = ' '
WALL = '|'
BLOCK = '#'
PADDLE = '_'
BALL = 'o'

def get_tile(tile_type): # pylint: disable=C0116
  if tile_type == 0:
    return EMPTY

  if tile_type == 1:
    return WALL

  if tile_type == 2:
    return BLOCK

  if tile_type == 3:
    return PADDLE

  if tile_type == 4:
    return BALL

  return None

def build_screen(values, field=None, score=None, paddle_position=None, ball_position=None): # pylint: disable=C0116
  if field is None:
    field = defaultdict(lambda: EMPTY)

  index = 0
  while index < len(values):
    x = values[index]
    y = values[index + 1]
    tile_type = values[index + 2]

    if x == -1 and y == 0:
      score = tile_type
    else:
      tile = get_tile(tile_type)
      position = Position(x, y)
      field[position] = tile
      if tile == PADDLE:
        paddle_position = position
      elif tile == BALL:
        ball_position = position

    index += 3

  return (field, score, paddle_position, ball_position)

def print_screen(field, score): # pylint: disable=C0116
  minx = min(field.keys(), key=lambda p: p.x).x
  maxx = max(field.keys(), key=lambda p: p.x).x
  miny = min(field.keys(), key=lambda p: p.y).y
  maxy = max(field.keys(), key=lambda p: p.y).y

  CLEAR_SCREEN()
  for y in range(miny, maxy + 1):
    for x in range(minx, maxx + 1):
      position = Position(x, y)
      print(field[position], end='')
    print('', flush=True)

  print(f'Current Score: {score}')

def count_blocks(field): # pylint: disable=C0116
  count = 0
  for p in field:
    if field[p] == BLOCK:
      count += 1
  return count

def play_game(program, debug=False): # pylint: disable=C0116
  program.execute()
  (field, score, paddle_position, ball_position) = build_screen(program.output)
  while not program.has_halted:
    if debug:
      sleep(0.0166666) # 60 FPS baby
      print_screen(field, score)

    if paddle_position.x < ball_position.x:
      program.provide_input(1)
    elif paddle_position.x == ball_position.x:
      program.provide_input(0)
    else:
      program.provide_input(-1)

    program.output = list() # Clear the output to save time building screen
    program.execute()
    (field, score, paddle_position, ball_position) = build_screen(program.output, field, score, paddle_position, ball_position) # pylint: disable=C0301

  print_screen(field, score)

def main(): # pylint: disable=C0116
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample1.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  parser.add_argument('-d', '--debug', action='store_true')
  args = parser.parse_args()

  program_text = None
  with open(args.filename, 'r') as file:
    program_text = file.read().rstrip()

  program = intcode.IntCodeProgram.from_text(program_text)
  if args.part == 1:
    program.execute()
    (field, _, _, _) = build_screen(program.output)
    print(f'Block count: {count_blocks(field)}')
  elif args.part == 2:
    program.memory[0] = 2 # Free play!
    play_game(program, args.debug)

if __name__ == "__main__":
  main()
