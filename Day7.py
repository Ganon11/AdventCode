import os
import re

def read_directions(directions):
  circuit = dict()
  direction_pattern = re.compile(r'^(.*) -> ([a-z]+)$', re.IGNORECASE)

  for d in directions:
    m = direction_pattern.match(d)
    input_calc = m.group(1)
    result_wire = m.group(2)
    circuit[result_wire] = input_calc

  return circuit

number_pattern = re.compile(r'^(\d+|[a-z]+)$', re.IGNORECASE)
unary_pattern = re.compile(r'^NOT ([a-z]+)$', re.IGNORECASE)
binary_pattern = re.compile(r'^(\d+|[a-z]+) (AND|OR) (\d+|[a-z]+)$', re.IGNORECASE)
shift_pattern = re.compile(r'^([a-z]+) (LSHIFT|RSHIFT) (\d+)$', re.IGNORECASE)

def evaluate_circuit(circuit, wire):
  result = 0
  calc = circuit[wire]
  try:
    # Has it already been evaluated?
    result = int(calc)
    return result
  except:
    # There was a problem: we need to evaluate
    pass

  number_match = number_pattern.match(calc)
  unary_match = unary_pattern.match(calc)
  binary_match = binary_pattern.match(calc)
  shift_match = shift_pattern.match(calc)

  if number_match:
    try:
      result = int(calc)
    except:
      result = evaluate_circuit(circuit, calc)

  elif unary_match:
    input_wire = unary_match.group(1)
    result = ~evaluate_circuit(circuit, input_wire)

  elif binary_match:
    wire1 = binary_match.group(1)
    gate = binary_match.group(2)
    wire2 = binary_match.group(3)

    val1 = 0
    try:
      val1 = int(wire1)
    except:
      val1 = evaluate_circuit(circuit, wire1)

    val2 = 0
    try:
      val2 = int(wire2)
    except:
      val2 = evaluate_circuit(circuit, wire2)

    if gate == 'AND':
      result = val1 & val2
    else:
      result = val1 | val2

  elif shift_match:
    wire1 = shift_match.group(1)
    shifter = shift_match.group(2)
    amt = int(shift_match.group(3))

    val = evaluate_circuit(circuit, wire1)
    if shifter == 'LSHIFT':
      result = val << amt
    else:
      result = val >> amt

  circuit[wire] = int(result)
  return circuit[wire] 

def build_circuit1(directions):
  circuit = read_directions(directions)

  old_direction = circuit['a']
  a_val = evaluate_circuit(circuit, 'a')
  print("Part 1: Wire a has value %d" % a_val)

  circuit = read_directions(directions)
  circuit['b'] = a_val
  a_val = evaluate_circuit(circuit, 'a')
  print("Part 2: Wire a has value %d" % a_val)

def main():
  f = open('Day7Input.txt', 'r')
  directions = [l.rstrip() for l in f.readlines()]
  build_circuit1(directions)

if __name__ == "__main__":
  main()