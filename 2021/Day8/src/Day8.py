import argparse
from collections import defaultdict

# Seven-segment-display:
#
#  aaaa
# b    c
# b    c
#  dddd
# e    f
# e    f
#  gggg

class Display:
  '''Represents a single jumbled display.'''
  _SEGMENTS = dict()
  _SEGMENT_FREQUENCY = defaultdict(int)
  _DIGIT_SCORES = dict()

  '''Represents the 10 unique signals and 4 output values'''
  def __init__(self, line):
    (signals_str, outputs_str) = line.strip().split(' | ')
    self.signals = list(map(set, signals_str.split(' ')))
    self.outputs = list(map(set, outputs_str.split(' ')))

  @classmethod
  def initialize_digit_scores(cls):
    '''Initializes some static information about seven-segment displays'''
    cls._SEGMENTS[0] = set("abcefg")
    cls._SEGMENTS[1] = set("cf")
    cls._SEGMENTS[2] = set("acdeg")
    cls._SEGMENTS[3] = set("acdfg")
    cls._SEGMENTS[4] = set("bcdf")
    cls._SEGMENTS[5] = set("abdfg")
    cls._SEGMENTS[6] = set("abdefg")
    cls._SEGMENTS[7] = set("acf")
    cls._SEGMENTS[8] = set("abcdefg")
    cls._SEGMENTS[9] = set("abcdfg")

    for digit, segments in cls._SEGMENTS.items():
      for segment in segments:
        cls._SEGMENT_FREQUENCY[segment] += 1

    for digit, segments in cls._SEGMENTS.items():
      score = 0
      for segment in segments:
        score += cls._SEGMENT_FREQUENCY[segment]
      cls._DIGIT_SCORES[digit] = score

  def get_count_simple_outputs(self):
    '''Count the outputs that are definitely a certain digit'''
    total = 0
    for output in self.outputs:
      if len(output) == 2 or len(output) == 3 or len(output) == 4 or len(output) == 7:
        total += 1
    return total

  def get_output_value(self):
    '''Decodes the wires and segments, and calculates the output value'''
    # First, create a local segment frequency map
    local_segment_frequency = defaultdict(int)
    for signal in self.signals:
      for segment in signal:
        local_segment_frequency[segment] += 1

    actual_digits = list()
    for output in self.outputs:
      output_score = 0
      for segment in output:
        output_score += local_segment_frequency[segment]

      actual_digit = -1
      for digit, score in Display._DIGIT_SCORES.items():
        if output_score == score:
          actual_digit = digit
          break

      if actual_digit == -1:
        print('Something went wrong')
      actual_digits.append(actual_digit)

    value = int(''.join(map(str, actual_digits)))
    return value

def get_displays(filename):
  '''Gets the list of displays from the given file'''
  lines = []
  with open(filename, 'r') as fh:
    lines = fh.readlines()

  displays = []
  for line in lines:
    displays.append(Display(line))
  return displays

def count_simple_outputs(displays):
  '''Count the outputs that are definitely a certain digit'''
  total = 0
  for display in displays:
    total += display.get_count_simple_outputs()
  return total

def main():
  '''I hope this isn't a ticking time bomb.'''
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample1.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  Display.initialize_digit_scores()
  displays = get_displays(args.filename)
  if args.part == 1:
    total = count_simple_outputs(displays)
    print(f'Count of simple outputs: {total}')
  else:
    total = 0
    for display in displays:
      subtotal = display.get_output_value()
      #print(f'{display.outputs} -> {subtotal}')
      total += subtotal
    print(f'Output values sum: {total}')

if __name__ == "__main__":
  main()
