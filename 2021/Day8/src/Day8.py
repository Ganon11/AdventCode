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

    # These were manually calculated by inspecting the seven-segment display.
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

    # Now determine how often each segment is used
    for digit, segments in cls._SEGMENTS.items():
      for segment in segments:
        cls._SEGMENT_FREQUENCY[segment] += 1

    # Finally, calculate a "score" for each digit by summing the frequency of
    # its segments
    for digit, segments in cls._SEGMENTS.items():
      cls._DIGIT_SCORES[digit] = sum(cls._SEGMENT_FREQUENCY[segment] for segment in segments)

  def get_count_simple_outputs(self):
    '''Count the outputs that are definitely a certain digit'''
    simple_output_lens = [2, 3, 4, 7]
    return sum(len(output) in simple_output_lens for output in self.outputs)

  def get_output_value(self):
    '''Decodes the wires and segments, and calculates the output value'''

    # First, create a local segment frequency map
    local_segment_frequency = defaultdict(int)
    for signal in self.signals:
      for segment in signal:
        local_segment_frequency[segment] += 1

    # Now determine what digits our outputs are, based on their "score"
    actual_digits = list()
    for output in self.outputs:
      output_score = sum(local_segment_frequency[segment] for segment in output)

      for digit, score in Display._DIGIT_SCORES.items():
        if output_score == score:
          actual_digits.append(digit)
          break

    # Finally, convert the individual digits to one value
    return int(''.join(map(str, actual_digits)))

def get_displays(filename):
  '''Gets the list of displays from the given file'''
  lines = []
  with open(filename, 'r') as fh:
    lines = fh.readlines()

  return [Display(line) for line in lines]

def main():
  '''I hope this isn't a ticking time bomb.'''
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample1.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  displays = get_displays(args.filename)
  if args.part == 1:
    total = sum(display.get_count_simple_outputs() for display in displays)
    print(f'Count of simple outputs: {total}')
  else:
    total = sum(display.get_output_value() for display in displays)
    print(f'Output values sum: {total}')

Display.initialize_digit_scores()
if __name__ == "__main__":
  main()
