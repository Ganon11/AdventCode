import argparse
import re

def get_reboot_instructions(filename):
  '''Gets the list of reboot instructions.'''
  with open(filename, 'r') as fh:
    lines = fh.readlines()

  pattern = re.compile(r'(on|off) x=(-?\d+)\.\.(-?\d+),y=(-?\d+)\.\.(-?\d+),z=(-?\d+)\.\.(-?\d+)')
  tokens = []
  for line in lines:
    line = line.strip()
    match = pattern.match(line)
    instruction = match.group(1)
    x1 = int(match.group(2))
    x2 = int(match.group(3))
    y1 = int(match.group(4))
    y2 = int(match.group(5))
    z1 = int(match.group(6))
    z2 = int(match.group(7))
    tokens.append((instruction, x1, x2, y1, y2, z1, z2))

  return tokens

def instruction_in_reboot_range(instruction):
  '''Checks if this is an initialization step.'''
  min_x = instruction[1]
  max_x = instruction[2]
  min_y = instruction[3]
  max_y = instruction[4]
  min_z = instruction[5]
  max_z = instruction[6]

  if min_x < -50 or max_x > 50 or \
    min_y < -50 or max_y > 50 or \
    min_z < -50 or max_z > 50:
    return False

  return True

def follow_instruction(grid, instruction):
  '''Yessir yessir.'''
  min_x = instruction[1]
  max_x = instruction[2]
  min_y = instruction[3]
  max_y = instruction[4]
  min_z = instruction[5]
  max_z = instruction[6]

  for x in range(min_x, max_x + 1):
    for y in range(min_y, max_y + 1):
      for z in range(min_z, max_z + 1):
        point = (x, y, z)
        if instruction[0] == 'on':
          grid[point] = True
        elif point in grid:
          del grid[point]

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample1.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  instructions = get_reboot_instructions(args.filename)
  grid = dict()
  for instruction in instructions:
    if args.part == 1 and not instruction_in_reboot_range(instruction):
      continue

    follow_instruction(grid, instruction)
  print(len(grid))

if __name__ == "__main__":
  main()
