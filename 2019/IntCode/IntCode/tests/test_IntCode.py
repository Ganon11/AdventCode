import IntCode

def test_no_noun_verb_constructor():
  values = [0, 1, 2, 0, 99]
  p = IntCode.IntCodeProgram(values, useNounVerb=False)
  assert p.ip == 0
  assert p.memory == values

  p2 = IntCode.IntCodeProgram(values, useNounVerb=False, noun=7, verb=3)
  assert p2.ip == 0
  assert p2.memory == values

def test_default_constructor():
  values = [0, 1, 2, 0, 99]
  p = IntCode.IntCodeProgram(values)
  assert p.ip == 0
  assert p.memory == [0, 12, 2, 0, 99]

def test_noun_verb_constructor():
  values = [0, 1, 2, 0, 99]
  p = IntCode.IntCodeProgram(values, noun=7, verb=3)
  assert p.ip == 0
  assert p.memory == [0, 7, 3, 0, 99]

def test_fromText():
  values = [0, 1, 2, 0, 99]

  p = IntCode.IntCodeProgram.fromText("0,1,2,0,99", useNounVerb=False)
  assert p.ip == 0
  assert p.memory == values

  p2 = IntCode.IntCodeProgram.fromText("0, 1, 2, 0, 99", useNounVerb=False)
  assert p2.ip == 0
  assert p2.memory == values

  p3 = IntCode.IntCodeProgram.fromText("  0, 1  , 2 ,    0,     99   ", useNounVerb=False)
  assert p3.ip == 0
  assert p3.memory == values

def test_execute_add():
  values = [1, 1, 2, 0, 99]
  p = IntCode.IntCodeProgram(values, useNounVerb=False)
  output = p.execute()
  assert output == 3
  assert p.ip == 4

def test_execute_mul():
  values = [2, 1, 2, 0, 99]
  p = IntCode.IntCodeProgram(values, useNounVerb=False)
  output = p.execute()
  assert output == 2
  assert p.ip == 4