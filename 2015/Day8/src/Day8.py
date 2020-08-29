import os
import re
import sys

def count_strings(strings):
  total_diff = 0
  slash_pattern = re.compile(r'\\\\', re.IGNORECASE)
  quote_pattern = re.compile(r'\\"', re.IGNORECASE)
  hex_pattern = re.compile(r'\\x[0-9a-f]{2}', re.IGNORECASE)

  for s in strings:
    codecount = len(s)

    # Trim leading and closing "s
    copy = s[1:-1]
    copy = slash_pattern.sub('>', copy)
    copy = quote_pattern.sub('^', copy)
    copy = hex_pattern.sub('<', copy)
    memcount = len(copy)

    total_diff = total_diff + (codecount - memcount)

    #print("String: '%s', codecount: %d, stripped string '%s' memcount: %d" % (s, codecount, copy, memcount))

  print("Part 1: Difference: %d" % total_diff)

def encode_strings(strings):
  total_diff = 0

  for s in strings:
    codecount = len(s)
    copy = s

    copy = copy.replace("\"", "<") # copy = s.replace("\"", "\\\"")
    copy = copy.replace("\\", ">") # copy = s.replace("\\", "\\\\")

    copy = copy.replace("<", "\\\"")
    copy = copy.replace(">", "\\\\")

    copy = "\"" + copy + "\""
    encodecount = len(copy)

    total_diff = total_diff + (encodecount - codecount)

  print("Part 2: Difference: %d" % total_diff)

def main():
  filename = '../input/input.txt'
  if len(sys.argv) > 1:
    filename = sys.argv[1]
  f = open(filename, 'r')
  strings = [l.rstrip() for l in f.readlines()]
  count_strings(strings)
  encode_strings(strings)

if __name__ == "__main__":
  main()