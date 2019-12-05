import intcode

def test_no_noun_verb_constructor():
  values = [0, 1, 2, 0, 99]
  p = intcode.IntCodeProgram(values, useNounVerb=False)
  assert p.ip == 0
  assert p.memory == values

  p2 = intcode.IntCodeProgram(values, useNounVerb=False, noun=7, verb=3)
  assert p2.ip == 0
  assert p2.memory == values

def test_default_constructor():
  values = [0, 1, 2, 0, 99]
  p = intcode.IntCodeProgram(values)
  assert p.ip == 0
  assert p.memory == [0, 12, 2, 0, 99]

def test_noun_verb_constructor():
  values = [0, 1, 2, 0, 99]
  p = intcode.IntCodeProgram(values, noun=7, verb=3)
  assert p.ip == 0
  assert p.memory == [0, 7, 3, 0, 99]

def test_from_text():
  values = [0, 1, 2, 0, 99]

  p = intcode.IntCodeProgram.from_text("0,1,2,0,99", useNounVerb=False)
  assert p.ip == 0
  assert p.memory == values

  p2 = intcode.IntCodeProgram.from_text("0, 1, 2, 0, 99", useNounVerb=False)
  assert p2.ip == 0
  assert p2.memory == values

  p3 = intcode.IntCodeProgram.from_text("  0, 1  , 2 ,    0,     99   ", useNounVerb=False)
  assert p3.ip == 0
  assert p3.memory == values

def test_execute_add():
  values = [1, 1, 2, 0, 99]
  p = intcode.IntCodeProgram(values, useNounVerb=False)
  output = p.execute()
  assert output == 3
  assert p.ip == 4

def test_execute_mul():
  values = [2, 1, 2, 0, 99]
  p = intcode.IntCodeProgram(values, useNounVerb=False)
  output = p.execute()
  assert output == 2
  assert p.ip == 4

def test_execute_default_input():
  values = [3, 0, 99]
  p = intcode.IntCodeProgram(values, useNounVerb=False)
  output = p.execute()
  assert p.ip == 2
  assert p.memory[0] == 1
  assert output == 1

def test_execute_input():
  values = [3, 0, 99]
  p = intcode.IntCodeProgram(values, useNounVerb=False, userInput=77)
  output = p.execute()
  assert p.ip == 2
  assert p.memory[0] == 77
  assert output == 77

def test_execute_output():
  values = [4, 0, 99]
  p = intcode.IntCodeProgram(values, useNounVerb=False)
  output = p.execute()
  assert p.ip == 2
  assert output == 4

def test_execute_add_immediate_mode():
  values = [1101, 50, 60, 0, 99]
  p = intcode.IntCodeProgram(values, useNounVerb=False)
  output = p.execute()
  assert p.ip == 4
  assert output == 110

def test_execute_add_mixed_modes():
  values = [101, 50, 0, 0, 99]
  p = intcode.IntCodeProgram(values, useNounVerb=False)
  output = p.execute()
  assert p.ip == 4
  assert output == 151

  values = [1001, 0, 50, 0, 99]
  p = intcode.IntCodeProgram(values, useNounVerb=False)
  output = p.execute()
  assert p.ip == 4
  assert output == 1051

def test_execute_mul_immediate_mode():
  values = [1102, 5, 6, 0, 99]
  p = intcode.IntCodeProgram(values, useNounVerb=False)
  output = p.execute()
  assert p.ip == 4
  assert output == 30

def test_execute_mul_mixed_modes():
  values = [102, 2, 0, 0, 99]
  p = intcode.IntCodeProgram(values, useNounVerb=False)
  output = p.execute()
  assert p.ip == 4
  assert output == 204

  values = [1002, 0, 2, 0, 99]
  p = intcode.IntCodeProgram(values, useNounVerb=False)
  output = p.execute()
  assert p.ip == 4
  assert output == 2004

def test_execute_jump_if_true():
  values = [1105, 1, 7, 1102, 0, 0, 0, 99]
  p = intcode.IntCodeProgram(values, useNounVerb=False)
  output = p.execute()
  assert p.ip == 7
  assert output == 1105

  values = [1105, 0, 7, 1102, 0, 0, 0, 99]
  p = intcode.IntCodeProgram(values, useNounVerb=False)
  output = p.execute()
  assert p.ip == 7
  assert output == 0

def test_execute_jump_if_false():
  values = [1106, 1, 7, 1102, 0, 0, 0, 99]
  p = intcode.IntCodeProgram(values, useNounVerb=False)
  output = p.execute()
  assert p.ip == 7
  assert output == 0

  values = [1106, 0, 7, 1102, 0, 0, 0, 99]
  p = intcode.IntCodeProgram(values, useNounVerb=False)
  output = p.execute()
  assert p.ip == 7
  assert output == 1106

def test_execute_less_than():
  values = [1107, 1, 2, 0, 99]
  p = intcode.IntCodeProgram(values, useNounVerb=False)
  output = p.execute()
  assert p.ip == 4
  assert output == 1

  values = [1107, 2, 2, 0, 99]
  p = intcode.IntCodeProgram(values, useNounVerb=False)
  output = p.execute()
  assert p.ip == 4
  assert output == 0

  values = [1107, 2, 1, 0, 99]
  p = intcode.IntCodeProgram(values, useNounVerb=False)
  output = p.execute()
  assert p.ip == 4
  assert output == 0

def test_execute_equals():
  values = [1108, 1, 2, 0, 99]
  p = intcode.IntCodeProgram(values, useNounVerb=False)
  output = p.execute()
  assert p.ip == 4
  assert output == 0

  values = [1108, 2, 2, 0, 99]
  p = intcode.IntCodeProgram(values, useNounVerb=False)
  output = p.execute()
  assert p.ip == 4
  assert output == 1

  values = [1108, 2, 1, 0, 99]
  p = intcode.IntCodeProgram(values, useNounVerb=False)
  output = p.execute()
  assert p.ip == 4
  assert output == 0