import argparse
import os
import intcode

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample1.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  values = []
  with open(args.filename, 'r') as f:
    values = [int (n) for n in f.read().split(',')]

  user_input = None
  if args.part == 1:
    user_input = 1
  elif args.part == 2:
    user_input = 5

  program = intcode.IntCodeProgram(values, useNounVerb=False, userInput=user_input)
  output = program.execute()

if __name__ == "__main__":
  main()