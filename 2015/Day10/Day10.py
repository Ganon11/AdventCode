from itertools import groupby
import sys

def look_and_say(input_string, num_iterations):
  for i in range(num_iterations):
    input_string = ''.join([str(len(list(g))) + str(k) for k,g in groupby(input_string)])
  return input_string

def main(start):
  print("Result length: %d" % len(look_and_say(start, 40)))
  print("Result length: %d" % len(look_and_say(start, 50)))

if __name__ == "__main__":
  start = '1'
  if len(sys.argv) > 1:
    start = sys.argv[1]
  main(start)