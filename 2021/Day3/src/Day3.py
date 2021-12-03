import argparse
from collections import defaultdict

def get_diagnostics(filename):
  diagnostics = []
  with open(filename, 'r') as fh:
    diagnostics = [line.strip() for line in fh.readlines()]

  return diagnostics

def get_bitcounts(diagnostics):
  bitlen = len(diagnostics[0])
  bitcounts = defaultdict(int)
  for diagnostic in diagnostics:
    for index in range(bitlen):
      if diagnostic[index] == '1':
        bitcounts[index] += 1
  return bitcounts

def get_simple_rate(diagnostics, condition):
  bitlen = len(diagnostics[0])
  bitcounts = get_bitcounts(diagnostics)

  bits = ['0' for x in range(bitlen)]
  count = len(diagnostics)
  for index in range(bitlen):
    if condition(bitcounts[index], count / 2):
      bits[index] = '1'
  return int(''.join(bits), 2)

def get_gamma_rate(diagnostics):
  return get_simple_rate(diagnostics, lambda a,b: a >= b)

def get_epsilon_rate(diagnostics):
  return get_simple_rate(diagnostics, lambda a,b: a <= b)

def get_complex_rate(diagnostics, defaultbit, otherbit):
  bitlen = len(diagnostics[0])
  candidates = diagnostics.copy()
  for index in range(bitlen):
    count = len(candidates)
    if count == 1:
      return int(candidates[0], 2)

    bitcounts = get_bitcounts(candidates)
    new_bit = defaultbit
    if bitcounts[index] < (count / 2):
      new_bit = otherbit
    candidates = list(filter(lambda candidate: candidate[index] == new_bit, candidates))

  if len(candidates) == 1:
    return int(candidates[0], 2)

  return 0

def get_oxygen_generator_rating(diagnostics):
  return get_complex_rate(diagnostics, '1', '0')

def get_co2_scrubber_rating(diagnostics):
  return get_complex_rate(diagnostics, '0', '1')

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  diagnostics = get_diagnostics(args.filename)
  if args.part == 1:
    gamma = get_gamma_rate(diagnostics)
    epsilon = get_epsilon_rate(diagnostics)
    print(f'Power consumption: {gamma * epsilon}')
  else:
    o2 = get_oxygen_generator_rating(diagnostics)
    co2 = get_co2_scrubber_rating(diagnostics)
    print(f'Life support: {o2 * co2}')

if __name__ == "__main__":
  main()
