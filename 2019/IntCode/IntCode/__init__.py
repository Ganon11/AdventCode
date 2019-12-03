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
    functions = {
      IntCodeProgram.ADD: IntCodeProgram.__add,
      IntCodeProgram.MUL: IntCodeProgram.__mul
    }
    while self.memory[self.ip] != IntCodeProgram.HALT:
      instruction = functions.get(self.memory[self.ip])
      instruction(self)

    return self.memory[0]
