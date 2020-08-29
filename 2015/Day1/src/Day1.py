import os
import sys

def main():
  filename = '../input/input.txt'
  if len(sys.argv) > 1:
    filename = sys.argv[1]
  f = open(filename, 'r')
  parens = f.read()
  length = len(parens)
  count = 0
  neg = -1
  for i in range(0, length):
    if parens[i] == '(':
      count = count + 1
    else:
      count = count - 1
      if count < 0 and neg == -1:
        neg = i + 1
  print('Floor ' + str(count) + ', went negative at ' + str(neg))

if __name__ == "__main__":
  main()