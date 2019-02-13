import argparse
import re
import sys

def get_coordinates(filename):
  line = ''
  with open(filename, 'r') as fp:
    line = fp.readline()
  match = re.search("row (\\d+), column (\\d+)", line)
  return (int(match.group(1)), int(match.group(2)))

def convert_index_to_coordinates(index):
  # 1       2       3       4       5       6       7
  # (1, 1), (2, 1), (1, 2), (3, 1), (2, 2), (1, 3), (4, 1)...
  return (None, None)

def convert_coordinates_to_index(row, col):
  return None

def get_next_code(code):
  n = code * 252533
  return n % 33554393

def get_code(row, col):
  code = 20151125
  r = 1
  c = 1
  while not (r == row and c == col):
    code = get_next_code(code)
    if r == 1:
      r = c + 1
      c = 1
    else:
      r = r - 1
      c = c + 1
  print("At (%d, %d) code is %d" % (r, c, code))

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('filename')
  args = parser.parse_args()

  (row, col) = get_coordinates(args.filename)

  get_code(row, col)

if __name__ == "__main__":
  main()
