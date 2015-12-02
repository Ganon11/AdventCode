import os

def main():
  f = open('Day1Input.txt', 'r')
  parens = f.read()
  length = len(parens)
  count = 0
  for i in range(0, length):
    if parens[i] == '(':
      count = count + 1
    else:
      count = count - 1
      if count < 0:
        print i + 1
        break
  print 'Done!'

if __name__ == "__main__":
  main()