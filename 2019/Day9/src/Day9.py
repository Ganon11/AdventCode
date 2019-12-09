import argparse

import intcode

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample1.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  program_memory = None
  with open(args.filename, 'r') as file:
    program_memory = file.read()
  program = intcode.IntCodeProgram.from_text(program_memory)
  program.provide_input(args.part)
  program.execute()
  print(program.output)

if __name__ == "__main__":
  main()
