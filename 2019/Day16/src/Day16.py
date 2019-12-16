import argparse

def get_pattern(index, position):
  actual_index = ((index + 1) // (position + 1)) % len(get_pattern.BASE_PATTERN)

  return get_pattern.BASE_PATTERN[actual_index]

get_pattern.BASE_PATTERN = [0, 1, 0, -1]

def phase(signal):
  output_signal = list()
  for output_index in range(0, len(signal)):
    total = 0
    for digit in enumerate(signal):
      left = digit[1]
      right = get_pattern(digit[0], output_index)
      total += left * right

    output_signal.append(abs(total) % 10)

  return output_signal

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample1.txt')
  parser.add_argument('-s', '--steps', default=100, type=int)
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  signal = list()
  with open(args.filename, 'r') as file:
    signal.extend([int(value) for value in list(file.read())])

  print(f'Input signal: {signal}')

  if args.part == 1:
    for _ in range(0, args.steps):
      signal = phase(signal)
    print(f'After {args.steps} phases, first eight digits are: {signal[0:8]}')

if __name__ == "__main__":
  main()
