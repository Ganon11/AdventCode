import argparse
import intcode

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample1.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  parser.add_argument('-r', '--replace', action='store_true')
  args = parser.parse_args()

  text = None
  with open(args.filename, 'r') as file:
    text = file.read()
  values = [int(n) for n in text.split(',')]

  if args.part == 1:
    program = intcode.IntCodeProgram(values)
    if args.replace:
      program.set_noun(12)
      program.set_verb(2)
    print(program.execute())
  elif args.part == 2:
    for noun in range(0, 99):
      for verb in range(0, 99):
        program = intcode.IntCodeProgram(values)
        program.set_noun(noun)
        program.set_verb(verb)
        if program.execute() == 19690720:
          print(f'100 * {noun} + {verb} == {100 * noun + verb}')


if __name__ == "__main__":
  main()
