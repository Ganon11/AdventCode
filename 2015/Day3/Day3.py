import os

def gib_present(x, y):
  gib_present.total = gib_present.total + 1

  if (x,y) in gib_present.presents:
    gib_present.presents[x, y] = gib_present.presents[x, y] + 1
  else:
    gib_present.presents[x, y] = 1
    gib_present.unique = gib_present.unique + 1

def do_part1(directions):
  length = len(directions)
  x = 0
  y = 0
  gib_present(x, y)
  for i in range(0, length):
    if directions[i] == '<':
      x = x - 1
    elif directions[i] == 'v':
      y = y - 1
    elif directions[i] == '>':
      x = x + 1
    elif directions[i] == '^':
      y = y + 1
    gib_present(x, y)
  print("Part 1: Unique: " + str(gib_present.unique) + ", Total: " + str(gib_present.total))

def do_part2(directions):
  length = len(directions)
  santax = 0
  santay = 0
  robox = 0
  roboy = 0
  is_santa = True
  gib_present(santax, santay)
  gib_present(robox, roboy)
  for i in range(0, length):
    if is_santa is True:
      if directions[i] == '<':
        santax = santax - 1
      elif directions[i] == 'v':
        santay = santay - 1
      elif directions[i] == '>':
        santax = santax + 1
      elif directions[i] == '^':
        santay = santay + 1
      gib_present(santax, santay)
      is_santa = False
    else:
      if directions[i] == '<':
        robox = robox - 1
      elif directions[i] == 'v':
        roboy = roboy - 1
      elif directions[i] == '>':
        robox = robox + 1
      elif directions[i] == '^':
        roboy = roboy + 1
      gib_present(robox, roboy)
      is_santa = True
      
  print("Part 2: Unique: " + str(gib_present.unique) + ", Total: " + str(gib_present.total))

def main():
  f = open('Input.txt', 'r')
  directions = f.read()

  gib_present.presents = {}
  gib_present.unique = 0
  gib_present.total = 0
  do_part1(directions)

  gib_present.presents = {}
  gib_present.unique = 0
  gib_present.total = 0
  do_part2(directions)

if __name__ == "__main__":
  main()