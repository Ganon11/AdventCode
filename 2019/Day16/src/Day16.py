import argparse
from functools import reduce

def get_pattern(index, position): # pylint: disable=C0116
  # Magic formula to figure out where in the BASE_PATTERN this index/position combo belongs
  # As position increases, index has less and less of an effect (as each element is repeated)
  actual_index = ((index + 1) // (position + 1)) % len(get_pattern.BASE_PATTERN)
  return get_pattern.BASE_PATTERN[actual_index]

get_pattern.BASE_PATTERN = [0, 1, 0, -1]

def phase(signal): # pylint: disable=C0116
  output_signal = list()
  for position in range(len(signal)):
    total = 0
    for index, digit in enumerate(signal):
      total += digit * get_pattern(index, position)
    output_signal.append(abs(total) % 10)
  return output_signal

def get_message(signal, phases): # pylint: disable=C0116
  # Get the integer value of the offset, which is the first 7 digits of the input signal
  offset = reduce(lambda acc, x: 10 * acc + x, signal[:7], 0)

  # Instead of actually expanding signal, determine the logical length
  logical_len = 10000 * len(signal)

  # This pattern only holds for digits in the latter half of the signal
  assert offset < logical_len <= 2 * offset

  # Create a smaller subsection by repeating signal only past the offset
  subsection = [signal[i % len(signal)] for i in range(offset, logical_len)]

  # Still have to do multiple phases
  for _ in range(phases):
    # Each element is abs(sum(all-later-elements)) % 10
    # Working backwards, create a partial sum in each element
    # That is, add the last element to the second-to-last element, then add that total to the
    # third-to-last element, and so on
    for i in reversed(range(1, len(subsection))):
      subsection[i - 1] += subsection[i]

    # Now get the last digits of our partial sums
    for index, element in enumerate(subsection):
      subsection[index] = abs(element) % 10

  # The answer is the first 8 digits of our subsection (which would have been the 8 digits
  # following the offset).
  return subsection[:8]

def main(): # pylint: disable=C0116
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample1.txt')
  parser.add_argument('-s', '--steps', default=100, type=int)
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  signal = list()
  with open(args.filename, 'r') as file:
    signal.extend([int(value) for value in list(file.read())])

  if args.part == 1:
    print(f'Input signal: {signal}')
    for _ in range(args.steps):
      signal = phase(signal)
    print(f'Message: {signal[:8]}')
  elif args.part == 2:
    message = get_message(signal, args.steps)
    print(f'Message: {message}')

if __name__ == "__main__":
  main()
