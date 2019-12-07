import argparse
from itertools import permutations

import intcode

def part_1(program_text):
  max_thruster_signal = 0
  phase_sequence = None
  for phase_setting_sequence in permutations(range(0, 5)):
    output = 0
    for index in range(0, 5):
      program = intcode.IntCodeProgram.from_text(program_text)
      program.provide_input([phase_setting_sequence[index], output])
      program.execute()
      output = program.output[0]

    if output > max_thruster_signal:
      max_thruster_signal = output
      phase_sequence = str(phase_setting_sequence)

  print(f'Max Thruster Signal: {max_thruster_signal} with phase sequence {phase_sequence}')

def part_2(program_text):
  max_thruster_signal = 0
  phase_sequence = None
  for phase_setting_sequence in permutations(range(5, 10)):
    programs = list()
    for index in range(0, 5):
      program_input = phase_setting_sequence[index]
      programs.append(intcode.IntCodeProgram.from_text(program_text, user_input=program_input))
      if index > 0:
        programs[index - 1].link_output(programs[index])

    programs[-1].link_output(programs[0])
    programs[0].provide_input(0)

    halted = set()
    while len(halted) != len(programs):
      for index in range(0, 5):
        if index in halted:
          pass
        if not programs[index].step():
          halted.add(index)

    if programs[4].output[-1] > max_thruster_signal:
      max_thruster_signal = programs[4].output[-1]
      phase_sequence = str(phase_setting_sequence)

  print(f'Max Thruster Signal: {max_thruster_signal} with phase sequence {phase_sequence}')

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample1.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  program_text = None
  with open(args.filename, 'r') as file:
    program_text = file.read()

  if args.part == 1:
    part_1(program_text)
  else:
    part_2(program_text)

if __name__ == "__main__":
  main()
