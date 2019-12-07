import intcode

def test_default_constructor():
  values = [0, 1, 2, 0, 99]
  program = intcode.IntCodeProgram(values)
  assert program.instruction_pointer == 0
  assert program.memory == values

def test_noun_verb():
  values = [0, 1, 2, 0, 99]
  program = intcode.IntCodeProgram(values)
  assert program.instruction_pointer == 0
  assert program.memory == values

  program.set_noun(7)
  assert program.memory[1] == 7

  program.set_verb(3)
  assert program.memory[2] == 3

def test_from_text():
  values = [0, 1, 2, 0, 99]

  program = intcode.IntCodeProgram.from_text("0,1,2,0,99")
  assert program.instruction_pointer == 0
  assert program.memory == values

  program2 = intcode.IntCodeProgram.from_text("0, 1, 2, 0, 99")
  assert program2.instruction_pointer == 0
  assert program2.memory == values

  program3 = intcode.IntCodeProgram.from_text("  0, 1  , 2 ,    0,     99   ")
  assert program3.instruction_pointer == 0
  assert program3.memory == values

def test_execute_add():
  values = [1, 1, 2, 0, 99]
  program = intcode.IntCodeProgram(values)
  output = program.execute()
  assert output == 3
  assert program.instruction_pointer == 4

def test_execute_mul():
  values = [2, 1, 2, 0, 99]
  program = intcode.IntCodeProgram(values)
  output = program.execute()
  assert output == 2
  assert program.instruction_pointer == 4

def test_execute_default_input():
  values = [3, 0, 99]
  program = intcode.IntCodeProgram(values)
  output = program.execute()
  assert program.instruction_pointer == 2
  assert program.memory[0] == 1
  assert output == 1

def test_execute_input():
  values = [3, 0, 99]
  program = intcode.IntCodeProgram(values, user_input=77)
  output = program.execute()
  assert program.instruction_pointer == 2
  assert program.memory[0] == 77
  assert output == 77

def test_execute_output():
  values = [4, 0, 99]
  program = intcode.IntCodeProgram(values)
  output = program.execute()
  assert program.instruction_pointer == 2
  assert output == 4

def test_execute_output_immediate_mode():
  values = [104, 50, 99]
  program = intcode.IntCodeProgram(values)
  output = program.execute()
  assert program.instruction_pointer == 2
  assert output == 104

def test_execute_add_immediate_mode():
  values = [1101, 50, 60, 0, 99]
  program = intcode.IntCodeProgram(values)
  output = program.execute()
  assert program.instruction_pointer == 4
  assert output == 110

def test_execute_add_mixed_modes():
  values = [101, 50, 0, 0, 99]
  program = intcode.IntCodeProgram(values)
  output = program.execute()
  assert program.instruction_pointer == 4
  assert output == 151

  values = [1001, 0, 50, 0, 99]
  program = intcode.IntCodeProgram(values)
  output = program.execute()
  assert program.instruction_pointer == 4
  assert output == 1051

def test_execute_mul_immediate_mode():
  values = [1102, 5, 6, 0, 99]
  program = intcode.IntCodeProgram(values)
  output = program.execute()
  assert program.instruction_pointer == 4
  assert output == 30

def test_execute_mul_mixed_modes():
  values = [102, 2, 0, 0, 99]
  program = intcode.IntCodeProgram(values)
  output = program.execute()
  assert program.instruction_pointer == 4
  assert output == 204

  values = [1002, 0, 2, 0, 99]
  program = intcode.IntCodeProgram(values)
  output = program.execute()
  assert program.instruction_pointer == 4
  assert output == 2004

def test_execute_jump_if_true():
  values = [1105, 1, 7, 1102, 0, 0, 0, 99]
  program = intcode.IntCodeProgram(values)
  output = program.execute()
  assert program.instruction_pointer == 7
  assert output == 1105

  values = [1105, 0, 7, 1102, 0, 0, 0, 99]
  program = intcode.IntCodeProgram(values)
  output = program.execute()
  assert program.instruction_pointer == 7
  assert output == 0

def test_execute_jump_if_false():
  values = [1106, 1, 7, 1102, 0, 0, 0, 99]
  program = intcode.IntCodeProgram(values)
  output = program.execute()
  assert program.instruction_pointer == 7
  assert output == 0

  values = [1106, 0, 7, 1102, 0, 0, 0, 99]
  program = intcode.IntCodeProgram(values)
  output = program.execute()
  assert program.instruction_pointer == 7
  assert output == 1106

def test_execute_less_than():
  values = [1107, 1, 2, 0, 99]
  program = intcode.IntCodeProgram(values)
  output = program.execute()
  assert program.instruction_pointer == 4
  assert output == 1

  values = [1107, 2, 2, 0, 99]
  program = intcode.IntCodeProgram(values)
  output = program.execute()
  assert program.instruction_pointer == 4
  assert output == 0

  values = [1107, 2, 1, 0, 99]
  program = intcode.IntCodeProgram(values)
  output = program.execute()
  assert program.instruction_pointer == 4
  assert output == 0

def test_execute_equals():
  values = [1108, 1, 2, 0, 99]
  program = intcode.IntCodeProgram(values)
  output = program.execute()
  assert program.instruction_pointer == 4
  assert output == 0

  values = [1108, 2, 2, 0, 99]
  program = intcode.IntCodeProgram(values)
  output = program.execute()
  assert program.instruction_pointer == 4
  assert output == 1

  values = [1108, 2, 1, 0, 99]
  program = intcode.IntCodeProgram(values)
  output = program.execute()
  assert program.instruction_pointer == 4
  assert output == 0
