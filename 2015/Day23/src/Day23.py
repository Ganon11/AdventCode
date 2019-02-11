import argparse
import sys

def get_instructions(filename):
  instructions = list()
  with open(filename, 'r') as fp:
    for line in fp.readlines():
      instructions.append(line.rstrip())

  return instructions

def run_instructions(instructions, registers):
  ip = 0

  while ip >= 0 and ip < len(instructions):
    instruction = instructions[ip]
    parts = instruction.split()
    command = parts[0]
    if command == 'hlf':
      register = parts[1]
      registers[register] = int(registers[register] / 2)
      ip = ip + 1
    elif command == 'tpl':
      register = parts[1]
      registers[register] = registers[register] * 3
      ip = ip + 1
    elif command == 'inc':
      register = parts[1]
      registers[register] = registers[register] + 1
      ip = ip + 1
    elif command == 'jmp':
      offset = int(parts[1])
      ip = ip + offset
    elif command == 'jie':
      register = parts[1][0]
      offset = int(parts[2])
      if registers[register] % 2 == 0:
        ip = ip + offset
      else:
        ip = ip + 1
    elif command == 'jio':
      register = parts[1][0]
      offset = int(parts[2])
      if registers[register] == 1:
        ip = ip + offset
      else:
        ip = ip + 1
    else:
      print("Bad command %s" % command)
      break

  return registers

def part_1(instructions):
  registers = dict()
  registers['a'] = 0
  registers['b'] = 0

  run_instructions(instructions, registers)

  print("Program terminated: register a = %d, register b = %d" % (registers['a'], registers['b']))

def part_2(instructions):
  registers = dict()
  registers['a'] = 1
  registers['b'] = 0

  run_instructions(instructions, registers)

  print("Program terminated: register a = %d, register b = %d" % (registers['a'], registers['b']))

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('filename')
  args = parser.parse_args()

  instructions = get_instructions(args.filename)

  part_1(instructions)
  part_2(instructions)

if __name__ == "__main__":
  main()
