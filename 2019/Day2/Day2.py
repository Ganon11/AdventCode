import argparse
import os
import sys

sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from IntCode import IntCodeProgram

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='sample1.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  parser.add_argument('-r', '--replace', action='store_true')
  args = parser.parse_args()

  f = open(args.filename, 'r')
  text = f.read()
  values = [int(n) for n in text.split(',')]

  if args.part == 1:
    program = IntCodeProgram(values)
    if (args.replace):
      program.memory[1] = 12
      program.memory[2] = 2
    print(program.execute())
  elif args.part == 2:
    for noun in range(0, 99):
      for verb in range(0, 99):
        program = IntCodeProgram(values)
        program.memory[1] = noun
        program.memory[2] = verb
        if program.execute() == 19690720:
          print(f'100 * {noun} + {verb} == {100 * noun + verb}')


if __name__ == "__main__":
  main()