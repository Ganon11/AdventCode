class IntCodeProgram:
  ADD = 1
  MUL = 2
  HALT = 99

  OPCODES = [ADD, MUL, HALT]

  def __init__(self, values):
    self.memory = values.copy()
    self.ip = 0

  def add(self):
    operand1 = self.memory[self.ip + 1]
    operand2 = self.memory[self.ip + 2]
    destination = self.memory[self.ip + 3]

    self.memory[destination] = self.memory[operand1] + self.memory[operand2]
    self.ip += 4

  def mul(self):
    operand1 = self.memory[self.ip + 1]
    operand2 = self.memory[self.ip + 2]
    destination = self.memory[self.ip + 3]

    self.memory[destination] = self.memory[operand1] * self.memory[operand2]
    self.ip += 4

  def execute(self):
    functions = {
      IntCodeProgram.ADD: IntCodeProgram.add,
      IntCodeProgram.MUL: IntCodeProgram.mul
    }
    while self.memory[self.ip] != IntCodeProgram.HALT:
      instruction = functions.get(self.memory[self.ip])
      instruction(self)

    return self.memory[0]
