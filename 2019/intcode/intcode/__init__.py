"""
# IntCode

This is an interpreter for [Advent of Code](https://adventofcode.com/) 2019's IntCode.

IntCode was first introduced in [Day 2](https://adventofcode.com/2019/day/2).
This first iteration required **ADD**, **MULTIPLY**, and **HALT** commands.

IntCode was expanded in [Day 5](https://adventofcode.com/2019/day/5). In this
iteration, the concept of *parameter modes* was introduced, and six more
instructions were added: **INPUT**, **OUTPUT**, **JUMP-IF-TRUE**,
**JUMP-IF-FALSE**, **LESS-THAN**, and **EQUALS**.

In [Day 7](https://adventofcode.com/2019/day/7), I changed how **INPUT** and
**OUTPUT** worked by enqueueing input and appending output to a list. This was
needed to implement message passing between multiple IntCode programs running in
parallel. I also added the **step** function to execute an IntCode program one
step at a time. As further enhancements, I allowed IntCode programs to execute
until blocking for input, then be resumed later (either via **execute** or
**step**).

In [Day 9](https://adventofcode.com/2019/day/9), IntCode was completed by adding
the **RELATIVE** *parameter mode*, introducing the concept of a *relative base*,
adding the **UPDATE-RELATIVE-BASE** opcode, and allowing memory accesses to new
memory to expand the RAM and retrieve a 0 value.
"""

import argparse
import warnings

from defaultlist import defaultlist

class IntCodeProgram:
  """
  An intcode program, represented as a list of values/instructions (memory) and
  an instruction pointer.
  """
  ADD = 1
  MUL = 2
  INPUT = 3
  OUTPUT = 4
  JUMP_IF_TRUE = 5
  JUMP_IF_FALSE = 6
  LESS_THAN = 7
  EQUALS = 8
  ADJUST_RELATIVE_BASE = 9
  HALT = 99

  POSITION_MODE = 0
  IMMEDIATE_MODE = 1
  RELATIVE_MODE = 2

  def __init__(self, values, user_input=None):
    self.memory = defaultlist(lambda: 0)
    self.memory.extend(values)
    self.instruction_pointer = 0
    self.input = list()
    if user_input is not None:
      if isinstance(user_input, list):
        self.input.extend(user_input)
      elif isinstance(user_input, int):
        self.input.append(user_input)
      else:
        raise Exception("Unacceptable user input type")
    self.output = list()
    self.has_halted = False
    self._relative_base = 0

  def set_noun(self, noun):
    """Sets the noun (memory value 1)"""
    self.memory[1] = noun

  def set_verb(self, verb):
    """Sets the verb (memory value 2)"""
    self.memory[2] = verb

  @classmethod
  def from_text(cls, text, user_input=None):
    """
    Creates an IntCodeProgram from text.

    The given string should be a comma-separated list of integers.
    """
    return cls([int(n) for n in text.split(',')], user_input)

  def link_output(self, other):
    """
    Link's this program's output to other's input
    """
    if isinstance(other, IntCodeProgram):
      self.output = other.input

  def _get_modes(self, number_of_modes=3):
    modes_str = str(self.memory[self.instruction_pointer] // 100).zfill(number_of_modes)[::-1]

    modes = list()
    for index in range(0, number_of_modes):
      modes.append(int(modes_str[index]))
    return modes

  def _get_r_values(self, number_of_values):
    modes = self._get_modes(number_of_modes=number_of_values)
    values = list()
    for index in range(0, number_of_values):
      value = self.memory[self.instruction_pointer + index + 1]
      if modes[index] == IntCodeProgram.POSITION_MODE:
        value = self.memory[value]
      elif modes[index] == IntCodeProgram.RELATIVE_MODE:
        value = self.memory[self._relative_base + value]

      values.append(value)

    return values

  def _get_l_value(self, relative_position):
    mode = self._get_modes(relative_position)[-1]
    value = self.memory[self.instruction_pointer + relative_position]
    if mode == IntCodeProgram.RELATIVE_MODE:
      value += self._relative_base

    return value

  def _add(self):
    values = self._get_r_values(2)
    address = self._get_l_value(relative_position=3)

    self.memory[address] = values[0] + values[1]
    self.instruction_pointer += 4

  def _mul(self):
    values = self._get_r_values(2)
    address = self._get_l_value(relative_position=3)

    self.memory[address] = values[0] * values[1]
    self.instruction_pointer += 4

  def _input(self):
    if len(self.input) == 0:
      return True
    address = self._get_l_value(relative_position=1)
    self.memory[address] = self.input.pop(0)

    self.instruction_pointer += 2
    return False

  def _output(self):
    values = self._get_r_values(number_of_values=1)
    self.output.append(values[0])
    self.instruction_pointer += 2

  def _jump_if_true(self):
    values = self._get_r_values(2)

    if values[0] != 0:
      self.instruction_pointer = values[1]
    else:
      self.instruction_pointer += 3

  def _jump_if_false(self):
    values = self._get_r_values(2)

    if values[0] == 0:
      self.instruction_pointer = values[1]
    else:
      self.instruction_pointer += 3

  def _less_than(self):
    values = self._get_r_values(2)
    address = self._get_l_value(relative_position=3)

    if values[0] < values[1]:
      self.memory[address] = 1
    else:
      self.memory[address] = 0
    self.instruction_pointer += 4

  def _equals(self):
    values = self._get_r_values(2)
    address = self._get_l_value(relative_position=3)

    if values[0] == values[1]:
      self.memory[address] = 1
    else:
      self.memory[address] = 0
    self.instruction_pointer += 4

  def _adjust_relative_base(self):
    values = self._get_r_values(1)
    self._relative_base += values[0]
    self.instruction_pointer += 2

  def step(self):
    """
    Executes the program one step at a time.

    If an input instruction is processed without any available input, True is returned.

    Otherwise, False is returned.
    """
    instruction = self.memory[self.instruction_pointer]
    opcode = instruction % 100
    if opcode == IntCodeProgram.ADD:
      self._add()
    elif opcode == IntCodeProgram.MUL:
      self._mul()
    elif opcode == IntCodeProgram.INPUT:
      if self._input():
        return True
    elif opcode == IntCodeProgram.OUTPUT:
      self._output()
    elif opcode == IntCodeProgram.JUMP_IF_TRUE:
      self._jump_if_true()
    elif opcode == IntCodeProgram.JUMP_IF_FALSE:
      self._jump_if_false()
    elif opcode == IntCodeProgram.LESS_THAN:
      self._less_than()
    elif opcode == IntCodeProgram.EQUALS:
      self._equals()
    elif opcode == IntCodeProgram.ADJUST_RELATIVE_BASE:
      self._adjust_relative_base()
    elif opcode == IntCodeProgram.HALT:
      self.has_halted = True
    else:
      raise Exception(f'Unrecognized opcode {opcode}')

    return False

  def execute(self):
    """
    Executes the program.

    The 'output' (memory[0]) of the program will be returned.
    """
    if self.has_halted:
      warnings.warn("Program has already executed")

    while not self.has_halted:
      should_return_early = self.step()
      if should_return_early:
        return self.memory[0]

    return self.memory[0]

  def provide_input(self, new_input):
    """
    Give additional input to the program.

    This input can either be a single integer value, or a list of integer values.
    """
    if isinstance(new_input, list):
      self.input.extend(new_input)
    elif isinstance(new_input, int):
      self.input.append(new_input)
    else:
      raise Exception("Unacceptable input type")

  @staticmethod
  def _is_valid_operand(other):
    return hasattr(other, "memory") and hasattr(other, "ip")

  def __str__(self):
    return f'Memory: {self.memory}, IP: {self.instruction_pointer}, RB: {self._relative_base}'

  def __hash__(self):
    return hash(str(self))

  def __repr__(self):
    return str(self)

  def __eq__(self, other):
    if not IntCodeProgram._is_valid_operand(other):
      return NotImplemented

    return self.memory == other.memory and self.instruction_pointer == other.ip

  def __ne__(self, other):
    if not IntCodeProgram._is_valid_operand(other):
      return NotImplemented

    return self.memory != other.memory or self.instruction_pointer != other.ip

def main():
  """
  Runs a sample IntCode program.
  """
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', required=True)
  args = parser.parse_args()
  values = []
  with open(args.filename, 'r') as file:
    values = [int(value) for value in file.read().split(',')]

  program = IntCodeProgram(values)
  program.execute()
  print(f'Program Output: {program.output}')

if __name__ == '__main__':
  main()
