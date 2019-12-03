import warnings

class IntCodeProgram:
  ADD = 1
  MUL = 2
  HALT = 99

  OPCODES = [ADD, MUL, HALT]

  def __init__(self, values, useNounVerb=True, noun=12, verb=2):
    self.memory = values.copy()
    if useNounVerb:
      self.memory[1] = noun
      self.memory[2] = verb
    self.ip = 0

  @classmethod
  def fromText(cls, text, useNounVerb=True, noun=12, verb=2):
    return cls([int(n) for n in text.split(',')], useNounVerb, noun, verb)

  def __add(self):
    address1 = self.memory[self.ip + 1]
    address2 = self.memory[self.ip + 2]
    destination = self.memory[self.ip + 3]

    self.memory[destination] = self.memory[address1] + self.memory[address2]
    self.ip += 4

  def __mul(self):
    address1 = self.memory[self.ip + 1]
    address2 = self.memory[self.ip + 2]
    destination = self.memory[self.ip + 3]

    self.memory[destination] = self.memory[address1] * self.memory[address2]
    self.ip += 4

  def execute(self):
    if self.ip != 0:
      warnings.warn("Program has already executed")
      return self.memory[0]

    while self.memory[self.ip] != IntCodeProgram.HALT:
      opcode = self.memory[self.ip]
      if opcode == IntCodeProgram.ADD:
        self.__add()
      elif opcode == IntCodeProgram.MUL:
        self.__mul()
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