import argparse

def get_instructions(filename):
  '''Gets a list of submarine instructions from the file.'''
  instructions = []
  with open(filename, 'r') as file:
    instructions = [line.strip().split(' ') for line in file.readlines()]

  return instructions

def follow_naive_instructions(instructions):
  '''Follows the instructions without aim.'''
  horizontal_position = 0
  depth = 0

  for instruction in instructions:
    value = int(instruction[1])
    if instruction[0] == 'forward':
      horizontal_position += value
    elif instruction[0] == 'down':
      depth += value
    elif instruction[0] == 'up':
      depth -= value
    else:
      raise ValueError(f'Unknown instruction: {instruction[0]}')

  return (horizontal_position, depth)

def follow_complicated_instructions(instructions):
  '''Follows the instructions with aim.'''
  horizontal_position = 0
  depth = 0
  aim = 0

  for instruction in instructions:
    value = int(instruction[1])
    if instruction[0] == 'forward':
      horizontal_position += value
      depth += value * aim
    elif instruction[0] == 'down':
      aim += value
    elif instruction[0] == 'up':
      aim -= value
    else:
      raise ValueError(f'Unknown instruction: {instruction[0]}')

  return (horizontal_position, depth)

def main():
  '''Deeper we must go!'''
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  instructions = get_instructions(args.filename)
  if args.part == 1:
    (h, d) = follow_naive_instructions(instructions)
  else:
    (h, d) = follow_complicated_instructions(instructions)
  print(f'Position Product: {h * d}')

if __name__ == "__main__":
  main()
