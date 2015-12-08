import os
import re

def do_assign(circuit, wire_in, wire_out):
  val = 0
  try:
    val = int(wire_in)
  except:
    if wire_in not in circuit:
      circuit[wire_in] = 0
    val = circuit[wire_in]

  circuit[wire_out] = val

def do_not(circuit, wire_in, wire_out):
  if wire_in not in circuit:
    circuit[wire_in] = 0
  circuit[wire_out] = (~circuit[wire_in]) % (65536)

def do_binary(circuit, gate, wire1, wire2, wire_out):
  val1 = 0
  try:
    val1 = int(wire1)
  except:
    if wire1 not in circuit:
      circuit[wire1] = 0
    val1 = circuit[wire1]

  val2 = 0
  try:
    val1 = int(wire2)
  except:
    if wire2 not in circuit:
      circuit[wire2] = 0
    val1 = circuit[wire2]
  
  val = 0
  if gate == 'AND':
    val = val1 & val2
  elif gate == 'OR':
    val = val1 | val2

  circuit[wire_out] = val

def do_shift(circuit, wire_in, shifter, amount, wire_out):
  if wire_in not in circuit:
    circuit[wire_in] = 0

  val = 0
  if shifter == 'LSHIFT':
    val = circuit[wire_in] << amount
  elif shifter == 'RSHIFT':
    val = circuit[wire_in] >> amount

  circuit[wire_out] = val

def build_circuit1(directions):
  circuit = dict()
  number_pattern = re.compile(r'^(\d+|[a-z]+) -> ([a-z]+)$', re.IGNORECASE)
  unary_pattern = re.compile(r'^NOT ([a-z]+) -> ([a-z]+)$', re.IGNORECASE)
  binary_pattern = re.compile(r'^(\d+|[a-z]+) (AND|OR) (\d+|[a-z]+) -> ([a-z]+)$', re.IGNORECASE)
  shift_pattern = re.compile(r'^([a-z]+) (LSHIFT|RSHIFT) (\d+) -> ([a-z]+)$', re.IGNORECASE)

  counter = 0

  for d in directions:
    if counter == 5:
      break
    counter = counter + 1
    line = d.rstrip()
    number_match = number_pattern.match(line)
    unary_match = unary_pattern.match(line)
    binary_match = binary_pattern.match(line)
    shift_match = shift_pattern.match(line)

    print("%s, %s %s %s %s" % (line, number_match, unary_match, binary_match, shift_match))

    if number_match:
      wire_in = number_match.group(1)
      wire_out = number_match.group(2)

      do_assign(circuit, wire_in, wire_out)

    elif unary_match:
      wire_in = unary_match.group(1)
      wire_out = unary_match.group(2)

      do_not(circuit, wire_in, wire_out)

    elif binary_match:
      wire1 = binary_match.group(1)
      gate = binary_match.group(2)
      wire2 = binary_match.group(3)
      wire_out = binary_match.group(4)

      do_binary(circuit, gate, wire1, wire2, wire_out)

    elif shift_match:
      wire_in = shift_match.group(1)
      shifter = shift_match.group(2)
      amount = int(shift_match.group(3))
      wire_out = shift_match.group(4)

      do_shift(circuit, wire_in, shifter, amount, wire_out)

    else:
      print("Bad command found: '%s'" % line)

  #print("Circuit emulation complete: wire a has value %d" % circuit['a'])
  for c in circuit:
    print("\t%s: %d" % (c, circuit[c]))

def main():
  f = open('Day7Input.txt', 'r')
  directions = f.readlines()
  build_circuit1(directions)

if __name__ == "__main__":
  main()