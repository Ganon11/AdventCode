import argparse
import intcode

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample1.txt')
  parser.add_argument('-i', '--input', type=int, required=True)
  args = parser.parse_args()

  values = []
  with open(args.filename, 'r') as file:
    values = [int(value) for value in file.read().split(',')]

  program = intcode.IntCodeProgram(values, user_input=[args.input])
  program.execute()
  print(f'Program Output: {program.output}')

if __name__ == "__main__":
  main()
