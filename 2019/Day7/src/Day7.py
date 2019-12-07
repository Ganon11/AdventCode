import argparse
from itertools import permutations

import intcode

def part_1(program_text):
  phase_setting_sequences = permutations(range(0, 5))

  max_thruster_signal = 0
  phase_sequence = None
  for phase_setting_sequence in phase_setting_sequences:
    input_1 = [phase_setting_sequence[0], 0]
    program_1 = intcode.IntCodeProgram.from_text(program_text, user_input=input_1)
    program_1.execute()

    input_2 = [phase_setting_sequence[1], program_1.output[0]]
    program_2 = intcode.IntCodeProgram.from_text(program_text, user_input=input_2)
    program_2.execute()

    input_3 = [phase_setting_sequence[2], program_2.output[0]]
    program_3 = intcode.IntCodeProgram.from_text(program_text, user_input=input_3)
    program_3.execute()

    input_4 = [phase_setting_sequence[3], program_3.output[0]]
    program_4 = intcode.IntCodeProgram.from_text(program_text, user_input=input_4)
    program_4.execute()

    input_5 = [phase_setting_sequence[4], program_4.output[0]]
    program_5 = intcode.IntCodeProgram.from_text(program_text, user_input=input_5)
    program_5.execute()

    if program_5.output[0] > max_thruster_signal:
      max_thruster_signal = program_5.output[0]
      phase_sequence = str(phase_setting_sequence)

  print(f'Max Thruster Signal: {max_thruster_signal} with phase sequence {phase_sequence}')

def part_2(program_text):
  phase_setting_sequences = permutations(range(5, 10))
  max_thruster_signal = 0
  phase_sequence = None
  for phase_setting_sequence in phase_setting_sequences:
    programs = list()
    halted = set()
    for index in range(0, 5):
      program_input = [phase_setting_sequence[index]]
      programs.append(intcode.IntCodeProgram.from_text(program_text, user_input=program_input))
    programs[0].provide_input([0])

    program_5_output = list()
    while len(halted) != len(programs):
      for index in range(0, 5):
        if index in halted:
          pass
        if not programs[index].step():
          halted.add(index)

        next_program_index = index + 1
        if next_program_index == 5:
          next_program_index = 0
        if len(programs[index].output) > 0:
          programs[next_program_index].provide_input(programs[index].output)
          if index == 4:
            program_5_output.extend(programs[index].output)
          programs[index].output = list()

    if program_5_output[-1] > max_thruster_signal:
      max_thruster_signal = program_5_output[-1]
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
