"""
# IntCode

This is an interpreter for [Advent of Code](https://adventofcode.com/) 2019's IntCode.

IntCode was first introduced in [Day 2](https://adventofcode.com/2019/day/2). This first
iteration required **ADD**, **MULTIPLY**, and **HALT** commands.

IntCode was expanded in [Day 5](https://adventofcode.com/2019/day/5). In this iteration,
the concept of *parameter modes* was introduced, and six more instructions were added:
**INPUT**, **OUTPUT**, **JUMP-IF-TRUE**, **JUMP-IF-FALSE**, **LESS-THAN**, and **EQUALS**.
"""

import warnings

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
  HALT = 99

  OPCODES = (ADD, MUL, INPUT, OUTPUT, JUMP_IF_TRUE, JUMP_IF_FALSE, LESS_THAN, EQUALS, HALT)

  POSITION_MODE = 0
  IMMEDIATE_MODE = 1

  def __init__(self, values, user_input=1):
    self.memory = values.copy()
    self.instruction_pointer = 0
    self._user_input = user_input

  def set_noun(self, noun):
    """Sets the noun (memory value 1)"""
    self.memory[1] = noun

  def set_verb(self, verb):
    """Sets the verb (memory value 2)"""
    self.memory[2] = verb

  @classmethod
  def from_text(cls, text):
    """
    Creates an IntCodeProgram from text.

    The given string should be a comma-separated list of integers.
    """
    return cls([int(n) for n in text.split(',')])

  @staticmethod
  def _get_modes(value, number_of_modes=3):
    modes = str(value // 100)
    modes_tuple = None
    if number_of_modes == 0:
      modes_tuple = ()

    elif number_of_modes == 1:
      modes_tuple = (int(modes[0]), )

    elif number_of_modes == 2:
      if len(modes) == 1:
        modes_tuple = (int(modes[0]), IntCodeProgram.POSITION_MODE)
      elif len(modes) == 2:
        modes_tuple = (int(modes[1]), int(modes[0]))

    elif number_of_modes == 3:
      if len(modes) == 1:
        modes_tuple = (int(modes[0]), IntCodeProgram.POSITION_MODE, IntCodeProgram.POSITION_MODE)
      elif len(modes) == 2:
        modes_tuple = (int(modes[1]), int(modes[0]), IntCodeProgram.POSITION_MODE)
      elif len(modes) == 3:
        modes_tuple = (int(modes[2]), int(modes[1]), int(modes[0]))

    else:
      raise Exception(f'Unrecognized number of modes {number_of_modes}')

    return modes_tuple

  def _add(self):
    modes = IntCodeProgram._get_modes(self.memory[self.instruction_pointer], number_of_modes=3)
    if modes[2] == IntCodeProgram.IMMEDIATE_MODE:
      raise Exception("Cannot write to an immediate mode value")

    value_a = self.memory[self.instruction_pointer + 1]
    if modes[0] == IntCodeProgram.POSITION_MODE:
      value_a = self.memory[value_a]

    value_b = self.memory[self.instruction_pointer + 2]
    if modes[1] == IntCodeProgram.POSITION_MODE:
      value_b = self.memory[value_b]

    destination = self.memory[self.instruction_pointer + 3]

    self.memory[destination] = value_a + value_b
    self.instruction_pointer += 4

  def _mul(self):
    modes = IntCodeProgram._get_modes(self.memory[self.instruction_pointer], number_of_modes=3)
    if modes[2] == IntCodeProgram.IMMEDIATE_MODE:
      raise Exception("Cannot write to an immediate mode value")

    value_a = self.memory[self.instruction_pointer + 1]
    if modes[0] == IntCodeProgram.POSITION_MODE:
      value_a = self.memory[value_a]

    value_b = self.memory[self.instruction_pointer + 2]
    if modes[1] == IntCodeProgram.POSITION_MODE:
      value_b = self.memory[value_b]

    destination = self.memory[self.instruction_pointer + 3]

    self.memory[destination] = value_a * value_b
    self.instruction_pointer += 4

  def _input(self):
    address = self.memory[self.instruction_pointer + 1]
    self.memory[address] = self._user_input

    self.instruction_pointer += 2

  def _output(self):
    modes = IntCodeProgram._get_modes(self.memory[self.instruction_pointer], number_of_modes=1)
    value_a = self.memory[self.instruction_pointer + 1]
    if modes[0] == IntCodeProgram.POSITION_MODE:
      value_a = self.memory[value_a]
    print(f"{value_a}")

    self.instruction_pointer += 2

  def _jump_if_true(self):
    modes = IntCodeProgram._get_modes(self.memory[self.instruction_pointer], number_of_modes=2)

    value_a = self.memory[self.instruction_pointer + 1]
    if modes[0] == IntCodeProgram.POSITION_MODE:
      value_a = self.memory[value_a]

    value_b = self.memory[self.instruction_pointer + 2]
    if modes[1] == IntCodeProgram.POSITION_MODE:
      value_b = self.memory[value_b]

    if value_a != 0:
      self.instruction_pointer = value_b
    else:
      self.instruction_pointer += 3

  def _jump_if_false(self):
    modes = IntCodeProgram._get_modes(self.memory[self.instruction_pointer], number_of_modes=2)

    value_a = self.memory[self.instruction_pointer + 1]
    if modes[0] == IntCodeProgram.POSITION_MODE:
      value_a = self.memory[value_a]

    value_b = self.memory[self.instruction_pointer + 2]
    if modes[1] == IntCodeProgram.POSITION_MODE:
      value_b = self.memory[value_b]

    if value_a == 0:
      self.instruction_pointer = value_b
    else:
      self.instruction_pointer += 3

  def _less_than(self):
    modes = IntCodeProgram._get_modes(self.memory[self.instruction_pointer], number_of_modes=3)
    if modes[2] == IntCodeProgram.IMMEDIATE_MODE:
      raise Exception("Cannot write to an immediate mode value")

    value_a = self.memory[self.instruction_pointer + 1]
    if modes[0] == IntCodeProgram.POSITION_MODE:
      value_a = self.memory[value_a]

    value_b = self.memory[self.instruction_pointer + 2]
    if modes[1] == IntCodeProgram.POSITION_MODE:
      value_b = self.memory[value_b]

    destination = self.memory[self.instruction_pointer + 3]

    if value_a < value_b:
      self.memory[destination] = 1
    else:
      self.memory[destination] = 0
    self.instruction_pointer += 4

  def _equals(self):
    modes = IntCodeProgram._get_modes(self.memory[self.instruction_pointer], number_of_modes=3)
    if modes[2] == IntCodeProgram.IMMEDIATE_MODE:
      raise Exception("Cannot write to an immediate mode value")

    value_a = self.memory[self.instruction_pointer + 1]
    if modes[0] == IntCodeProgram.POSITION_MODE:
      value_a = self.memory[value_a]

    value_b = self.memory[self.instruction_pointer + 2]
    if modes[1] == IntCodeProgram.POSITION_MODE:
      value_b = self.memory[value_b]

    destination = self.memory[self.instruction_pointer + 3]

    if value_a == value_b:
      self.memory[destination] = 1
    else:
      self.memory[destination] = 0
    self.instruction_pointer += 4

  def execute(self):
    """
    Executes the program.

    The 'output' (memory[0]) of the program will be returned.
    """
    if self.instruction_pointer != 0:
      warnings.warn("Program has already executed")
      return self.memory[0]

    while True:
      instruction = self.memory[self.instruction_pointer]
      opcode = instruction % 100
      if opcode == IntCodeProgram.ADD:
        self._add()
      elif opcode == IntCodeProgram.MUL:
        self._mul()
      elif opcode == IntCodeProgram.INPUT:
        self._input()
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
      elif opcode == IntCodeProgram.HALT:
        break
      else:
        raise Exception(f'Unrecognized opcode {opcode}')

    return self.memory[0]

  @staticmethod
  def _is_valid_operand(other):
    return hasattr(other, "memory") and hasattr(other, "ip")

  def __str__(self):
    return f'Memory: {self.memory}, IP: {self.instruction_pointer}'

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
