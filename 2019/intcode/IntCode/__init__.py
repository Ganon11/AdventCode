import warnings

class IntCodeProgram:
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

  def __init__(self, values, useNounVerb=True, noun=12, verb=2, userInput=1):
    self.memory = values.copy()
    if useNounVerb:
      self.memory[1] = noun
      self.memory[2] = verb
    self.ip = 0
    self._user_input = userInput

  @classmethod
  def from_text(cls, text, useNounVerb=True, noun=12, verb=2):
    return cls([int(n) for n in text.split(',')], useNounVerb, noun, verb)

  def __get_modes(self, value, numberOfModes=3):
    #print(f'get_modes({value}, numberOfModes={numberOfModes})')
    modes = str(value // 100)
    if numberOfModes == 0:
      return ()

    elif numberOfModes == 1:
      return (int(modes[0]))

    elif numberOfModes == 2:
      if len(modes) == 1:
        return (int(modes[0]), IntCodeProgram.POSITION_MODE)
      elif len(modes) == 2:
        return (int(modes[1]), int(modes[0]))

    elif numberOfModes == 3:
      if len(modes) == 1:
        return (int(modes[0]), IntCodeProgram.POSITION_MODE, IntCodeProgram.POSITION_MODE)
      elif len(modes) == 2:
        return (int(modes[1]), int(modes[0]), IntCodeProgram.POSITION_MODE)
      elif len(modes) == 3:
        return (int(modes[2]), int(modes[1]), int(modes[0]))

    else:
      raise Exception(f'Unrecognized number of modes {numberOfModes}')

  def __add(self):
    modes = self.__get_modes(self.memory[self.ip], numberOfModes=3)
    if modes[2] == IntCodeProgram.IMMEDIATE_MODE:
      raise Exception("Cannot write to an immediate mode value")

    A = self.memory[self.ip + 1]
    if modes[0] == IntCodeProgram.POSITION_MODE:
      A = self.memory[A]

    B = self.memory[self.ip + 2]
    if modes[1] == IntCodeProgram.POSITION_MODE:
      B = self.memory[B]

    destination = self.memory[self.ip + 3]

    self.memory[destination] = A + B
    self.ip += 4

  def __mul(self):
    modes = self.__get_modes(self.memory[self.ip], numberOfModes=3)
    if modes[2] == IntCodeProgram.IMMEDIATE_MODE:
      raise Exception("Cannot write to an immediate mode value")

    A = self.memory[self.ip + 1]
    if modes[0] == IntCodeProgram.POSITION_MODE:
      A = self.memory[A]

    B = self.memory[self.ip + 2]
    if modes[1] == IntCodeProgram.POSITION_MODE:
      B = self.memory[B]

    destination = self.memory[self.ip + 3]

    self.memory[destination] = A * B
    self.ip += 4

  def __input(self):
    address = self.memory[self.ip + 1]
    self.memory[address] = self._user_input

    self.ip += 2

  def __output(self):
    address = self.memory[self.ip + 1]
    print(f"memory[{address}] = {self.memory[address]}")

    self.ip += 2

  def __jump_if_true(self):
    #print("__jump_if_true: " + str(self))
    modes = self.__get_modes(self.memory[self.ip], numberOfModes=2)

    A = self.memory[self.ip + 1]
    if modes[0] == IntCodeProgram.POSITION_MODE:
      A = self.memory[A]

    B = self.memory[self.ip + 2]
    if modes[1] == IntCodeProgram.POSITION_MODE:
      B = self.memory[B]

    #print(f'  checking if {A} != 0 (and jumping to {B}')
    if A != 0:
      self.ip = B
    else:
      self.ip += 3

  def __jump_if_false(self):
    modes = self.__get_modes(self.memory[self.ip], numberOfModes=2)

    A = self.memory[self.ip + 1]
    if modes[0] == IntCodeProgram.POSITION_MODE:
      A = self.memory[A]

    B = self.memory[self.ip + 2]
    if modes[1] == IntCodeProgram.POSITION_MODE:
      B = self.memory[B]

    if A == 0:
      self.ip = B
    else:
      self.ip += 3

  def __less_than(self):
    #print("__less_than: " + str(self))
    modes = self.__get_modes(self.memory[self.ip], numberOfModes=3)
    if modes[2] == IntCodeProgram.IMMEDIATE_MODE:
      raise Exception("Cannot write to an immediate mode value")

    A = self.memory[self.ip + 1]
    if modes[0] == IntCodeProgram.POSITION_MODE:
      A = self.memory[A]

    B = self.memory[self.ip + 2]
    if modes[1] == IntCodeProgram.POSITION_MODE:
      B = self.memory[B]

    destination = self.memory[self.ip + 3]

    #print(f'  comparing {A} < {B}, writing to {destination}')
    if A < B:
      self.memory[destination] = 1
    else:
      self.memory[destination] = 0
    self.ip += 4

  def __equals(self):
    modes = self.__get_modes(self.memory[self.ip], numberOfModes=3)
    if modes[2] == IntCodeProgram.IMMEDIATE_MODE:
      raise Exception("Cannot write to an immediate mode value")

    A = self.memory[self.ip + 1]
    if modes[0] == IntCodeProgram.POSITION_MODE:
      A = self.memory[A]

    B = self.memory[self.ip + 2]
    if modes[1] == IntCodeProgram.POSITION_MODE:
      B = self.memory[B]

    destination = self.memory[self.ip + 3]

    if A == B:
      self.memory[destination] = 1
    else:
      self.memory[destination] = 0
    self.ip += 4

  def execute(self):
    if self.ip != 0:
      warnings.warn("Program has already executed")
      return self.memory[0]

    while True:
      instruction = self.memory[self.ip]
      opcode = instruction % 100
      if opcode == IntCodeProgram.ADD:
        self.__add()
      elif opcode == IntCodeProgram.MUL:
        self.__mul()
      elif opcode == IntCodeProgram.INPUT:
        self.__input()
      elif opcode == IntCodeProgram.OUTPUT:
        self.__output()
      elif opcode == IntCodeProgram.JUMP_IF_TRUE:
        self.__jump_if_true()
      elif opcode == IntCodeProgram.JUMP_IF_FALSE:
        self.__jump_if_false()
      elif opcode == IntCodeProgram.LESS_THAN:
        self.__less_than()
      elif opcode == IntCodeProgram.EQUALS:
        self.__equals()
      elif opcode == IntCodeProgram.HALT:
        break
      else:
        raise Exception(f'Unrecognized opcode {opcode}')

    return self.memory[0]

  def _is_valid_operand(self, other):
    return hasattr(other, "memory") and hasattr(other, "ip")

  def __str__(self):
    return f'Memory: {self.memory}, IP: {self.ip}'

  def __hash__(self):
    return hash(str(self))

  def __repr__(self):
    return str(self)

  def __eq__(self, other):
    if not self._is_valid_operand(other):
      return NotImplemented

    return self.memory == other.memory and self.ip == other.ip

  def __ne__(self, other):
    if not self._is_valid_operand(other):
      return NotImplemented

    return self.memory != other.memory or self.ip != other.ip