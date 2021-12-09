import argparse
from collections import defaultdict

def get_diagnostics(filename):
  '''Gets the diagnostic codes from the file.'''
  diagnostics = []
  with open(filename, 'r') as fh:
    diagnostics = [line.strip() for line in fh.readlines()]

  return diagnostics

def get_bitcounts(diagnostics):
  '''Counts the number of on bits in each diagnostic.'''
  bitlen = len(diagnostics[0])
  bitcounts = defaultdict(int)
  for diagnostic in diagnostics:
    for index in range(bitlen):
      if diagnostic[index] == '1':
        bitcounts[index] += 1
  return bitcounts

def get_simple_rate(diagnostics, condition):
  '''Gets a rate following the naive rules.'''
  bitlen = len(diagnostics[0])
  bitcounts = get_bitcounts(diagnostics)

  bits = ['0' for x in range(bitlen)]
  count = len(diagnostics)
  for index in range(bitlen):
    if condition(bitcounts[index], count / 2):
      bits[index] = '1'
  return int(''.join(bits), 2)

def get_gamma_rate(diagnostics):
  '''Finds the gamma rate.'''
  return get_simple_rate(diagnostics, lambda a,b: a >= b)

def get_epsilon_rate(diagnostics):
  '''Finds the epsilon rate.'''
  return get_simple_rate(diagnostics, lambda a,b: a <= b)

def get_complex_rate(diagnostics, condition):
  '''Gets a rate following the naive rules.'''
  bitlen = len(diagnostics[0])
  candidates = diagnostics.copy()
  for index in range(bitlen):
    count = len(candidates)
    if count == 1:
      break

    bitcounts = get_bitcounts(candidates)
    new_bit = '1'
    if condition(bitcounts[index], count / 2):
      new_bit = '0'
    candidates = list(filter(lambda candidate, b = new_bit, i = index: candidate[i] == b, \
     candidates))

  return int(candidates[0], 2)

def get_oxygen_generator_rating(diagnostics):
  '''Gets the oxygen generator rating.'''
  return get_complex_rate(diagnostics, lambda a,b: a < b)

def get_co2_scrubber_rating(diagnostics):
  '''Gets the CO2 Scrubber rating.'''
  return get_complex_rate(diagnostics, lambda a,b: a >= b)

def main():
  '''All systems functional.'''
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
