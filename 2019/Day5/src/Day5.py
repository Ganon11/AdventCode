import argparse
import os
import intcode

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample1.txt')
  parser.add_argument('-i', '--input', type=int, required=True)
  args = parser.parse_args()

  values = []
  with open(args.filename, 'r') as f:
    values = [int (n) for n in f.read().split(',')]

  program = intcode.IntCodeProgram(values, useNounVerb=False, userInput=args.input)
  output = program.execute()

if __name__ == "__main__":
  main()