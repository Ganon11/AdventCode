import argparse
import os
import sys

def execute(program):
  ip = 0

  while program[ip] != 99:
    a = program[ip + 1]
    b = program[ip + 2]
    dest = program[ip + 3]
    if program[ip] == 1:
      program[dest] = program[a] + program[b]
    elif program[ip] == 2:
      program[dest] = program[a] * program[b]

    ip += 4

  return program[0]

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='sample1.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  parser.add_argument('-r', '--replace', action='store_true')
  args = parser.parse_args()

  f = open(args.filename, 'r')
  text = f.read()
  program = [int(n) for n in text.split(',')]

  if args.part == 1:
    if (args.replace):
      program[1] = 12
      program[2] = 2
    print(execute(program))
  elif args.part == 2:
    for noun in range(0, 99):
      for verb in range(0, 99):
        programCopy = program.copy()
        programCopy[1] = noun
        programCopy[2] = verb
        if execute(programCopy) == 19690720:
          print(f'100 * {noun} + {verb} == {100 * noun + verb}')


if __name__ == "__main__":
  main()