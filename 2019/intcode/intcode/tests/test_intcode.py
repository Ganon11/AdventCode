import intcode

def test_default_constructor(): # pylint: disable=C0116
  values = [0, 1, 2, 0, 99]
  program = intcode.IntCodeProgram(values)
  assert program.instruction_pointer == 0
  assert program.memory == values

def test_noun_verb(): # pylint: disable=C0116
  values = [0, 1, 2, 0, 99]
  program = intcode.IntCodeProgram(values)
  assert program.instruction_pointer == 0
  assert program.memory == values

  program.set_noun(7)
  assert program.memory[1] == 7

  program.set_verb(3)
  assert program.memory[2] == 3

def test_from_text(): # pylint: disable=C0116
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

def test_execute_add(): # pylint: disable=C0116
  program = intcode.IntCodeProgram([1, 1, 2, 0, 99])
  output = program.execute()
  assert output == 3
  assert program.instruction_pointer == 4

def test_execute_mul(): # pylint: disable=C0116
  program = intcode.IntCodeProgram([2, 1, 2, 0, 99])
  output = program.execute()
  assert output == 2
  assert program.instruction_pointer == 4

def test_execute_input(): # pylint: disable=C0116
  values = [3, 0, 99]
  program = intcode.IntCodeProgram(values, user_input=[77])
  output = program.execute()
  assert program.instruction_pointer == 2
  assert program.memory[0] == 77
  assert output == 77

  program2 = intcode.IntCodeProgram(values, user_input=77)
  output2 = program2.execute()
  assert program2.instruction_pointer == 2
  assert program2.memory[0] == 77
  assert output2 == 77

def test_multiple_input(): # pylint: disable=C0116
  values = [3, 0, 3, 1, 99]
  program = intcode.IntCodeProgram(values, user_input=[1, 2])
  output = program.execute()
  assert program.instruction_pointer == 4
  assert program.memory[0] == 1
  assert program.memory[1] == 2
  assert output == 1

  program2 = intcode.IntCodeProgram(values, user_input=1)
  program2.provide_input(2)
  output2 = program2.execute()
  assert program2.instruction_pointer == 4
  assert program2.memory[0] == 1
  assert program2.memory[1] == 2
  assert output2 == 1

def test_execute_output(): # pylint: disable=C0116
  program = intcode.IntCodeProgram([4, 0, 99])
  output = program.execute()
  assert program.instruction_pointer == 2
  assert output == 4
  assert len(program.output) == 1
  assert 4 in program.output

def test_execute_output_immediate_mode(): # pylint: disable=C0116
  program = intcode.IntCodeProgram([104, 50, 99])
  output = program.execute()
  assert program.instruction_pointer == 2
  assert output == 104
  assert len(program.output) == 1
  assert 50 in program.output

def test_execute_multiple_output(): # pylint: disable=C0116
  program = intcode.IntCodeProgram([4, 0, 104, 50, 99])
  output = program.execute()
  assert program.instruction_pointer == 4
  assert output == 4
  assert len(program.output) == 2
  assert 4 in program.output
  assert 50 in program.output

def test_execute_add_immediate_mode(): # pylint: disable=C0116
  program = intcode.IntCodeProgram([1101, 50, 60, 0, 99])
  output = program.execute()
  assert program.instruction_pointer == 4
  assert output == 110

def test_execute_add_mixed_modes(): # pylint: disable=C0116
  program = intcode.IntCodeProgram([101, 50, 0, 0, 99])
  output = program.execute()
  assert program.instruction_pointer == 4
  assert output == 151

  program = intcode.IntCodeProgram([1001, 0, 50, 0, 99])
  output = program.execute()
  assert program.instruction_pointer == 4
  assert output == 1051

def test_execute_mul_immediate_mode(): # pylint: disable=C0116
  program = intcode.IntCodeProgram([1102, 5, 6, 0, 99])
  output = program.execute()
  assert program.instruction_pointer == 4
  assert output == 30

def test_execute_mul_mixed_modes(): # pylint: disable=C0116
  program = intcode.IntCodeProgram([102, 2, 0, 0, 99])
  output = program.execute()
  assert program.instruction_pointer == 4
  assert output == 204

  program = intcode.IntCodeProgram([1002, 0, 2, 0, 99])
  output = program.execute()
  assert program.instruction_pointer == 4
  assert output == 2004

def test_execute_jump_if_true(): # pylint: disable=C0116
  program = intcode.IntCodeProgram([1105, 1, 7, 1102, 0, 0, 0, 99])
  output = program.execute()
  assert program.instruction_pointer == 7
  assert output == 1105

  program = intcode.IntCodeProgram([1105, 0, 7, 1102, 0, 0, 0, 99])
  output = program.execute()
  assert program.instruction_pointer == 7
  assert output == 0

def test_execute_jump_if_false(): # pylint: disable=C0116
  program = intcode.IntCodeProgram([1106, 1, 7, 1102, 0, 0, 0, 99])
  output = program.execute()
  assert program.instruction_pointer == 7
  assert output == 0

  program = intcode.IntCodeProgram([1106, 0, 7, 1102, 0, 0, 0, 99])
  output = program.execute()
  assert program.instruction_pointer == 7
  assert output == 1106

def test_execute_less_than(): # pylint: disable=C0116
  program = intcode.IntCodeProgram([1107, 1, 2, 0, 99])
  output = program.execute()
  assert program.instruction_pointer == 4
  assert output == 1

  program = intcode.IntCodeProgram([1107, 2, 2, 0, 99])
  output = program.execute()
  assert program.instruction_pointer == 4
  assert output == 0

  program = intcode.IntCodeProgram([1107, 2, 1, 0, 99])
  output = program.execute()
  assert program.instruction_pointer == 4
  assert output == 0

def test_execute_equals(): # pylint: disable=C0116
  program = intcode.IntCodeProgram([1108, 1, 2, 0, 99])
  output = program.execute()
  assert program.instruction_pointer == 4
  assert output == 0

  program = intcode.IntCodeProgram([1108, 2, 2, 0, 99])
  output = program.execute()
  assert program.instruction_pointer == 4
  assert output == 1

  program = intcode.IntCodeProgram([1108, 2, 1, 0, 99])
  output = program.execute()
  assert program.instruction_pointer == 4
  assert output == 0

def test_step(): # pylint: disable=C0116
  program = intcode.IntCodeProgram([1108, 1, 2, 0, 1108, 2, 2, 0, 1108, 2, 1, 0, 99])

  program.step()
  assert not program.has_halted
  assert program.instruction_pointer == 4

  program.step()
  assert not program.has_halted
  assert program.instruction_pointer == 8

  program.step()
  assert not program.has_halted
  assert program.instruction_pointer == 12

  program.step()
  assert program.has_halted
  assert program.instruction_pointer == 12

def test_step_without_input_is_no_op(): # pylint: disable=C0116
  program = intcode.IntCodeProgram([3, 1, 99])

  program.step()
  assert program.instruction_pointer == 0
  assert not program.has_halted

  program.provide_input(103)
  program.step()
  assert program.instruction_pointer == 2
  assert not program.has_halted

  program.step()
  assert program.instruction_pointer == 2
  assert program.has_halted

def test_execute_will_return_early_if_waiting_for_input(): # pylint: disable=C0116
  program = intcode.IntCodeProgram([3, 1, 99])

  program.execute()
  assert not program.has_halted
  assert program.instruction_pointer == 0

  program.provide_input(103)
  program.execute()
  assert program.instruction_pointer == 2
  assert program.has_halted

def test_update_relative_base(): # pylint: disable=C0116
  program = intcode.IntCodeProgram([201, 2, 1, 17, 109, 17, 2201, 0, 0, 19, 99])
  program.execute()
  assert program.instruction_pointer == 10
  assert program.has_halted
  assert program._relative_base == 17 # pylint: disable=W0212

def test_increased_available_memory(): # pylint: disable=C0116
  program = intcode.IntCodeProgram([1101, 1, 2, 17, 99])
  program.execute()
  assert len(program.memory) == 18
  assert program.instruction_pointer == 4
  assert program.has_halted

def test_reddit(): # pylint: disable=C0116
  program = intcode.IntCodeProgram([109, 1, 203, 2, 204, 2, 99])
  program.provide_input(77)
  program.execute()
  print(program)
  print(program.output)

if __name__ == "__main__":
  test_reddit()
