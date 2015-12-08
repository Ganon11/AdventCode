import os
import re

def turn_on(lights, x1, y1, x2, y2):
  x = x1
  while x != x2 + 1:
    y = y1
    while y != y2 + 1:
      lights[x][y] = 1
      y = y + 1
    x = x + 1

def turn_off(lights, x1, y1, x2, y2):
  x = x1
  while x != x2 + 1:
    y = y1
    while y != y2 + 1:
      lights[x][y] = 0
      y = y + 1
    x = x + 1

def toggle(lights, x1, y1, x2, y2):
  x = x1
  while x != x2 + 1:
    y = y1
    while y != y2 + 1:
      if lights[x][y] == 0:
        lights[x][y] = 1
      else:
        lights[x][y] = 0
      y = y + 1
    x = x + 1

def num_lights_lit(lights):
  total = 0
  for row in lights:
    for light in row:
      if light == 1:
        total = total + 1
  return total

def sum_brightness(lights):
  total = 0
  for row in lights:
    for light in row:
      total = total + light
  return total

def part1(directions):
  p = re.compile(r'((?:turn on)|toggle|(?:turn off)) (\d{1,4}),(\d{1,4}) through (\d{1,4}),(\d{1,4})', re.IGNORECASE)
  lights = [[False for x in range(1000)] for x in range(1000)]
  for d in directions:
    m = p.match(d.rstrip())
    command = m.group(1)
    x1 = int(m.group(2))
    y1 = int(m.group(3))
    x2 = int(m.group(4))
    y2 = int(m.group(5))

    if command == 'turn on':
      turn_on(lights, x1, y1, x2, y2)
    elif command == 'turn off':
      turn_off(lights, x1, y1, x2, y2)
    elif command == 'toggle':
      toggle(lights, x1, y1, x2, y2)

  print("Part 1: Lights lit %d" % num_lights_lit(lights))

def elvish_turn_on(lights, x1, y1, x2, y2):
  x = x1
  while x != x2 + 1:
    y = y1
    while y != y2 + 1:
      lights[x][y] = lights[x][y] + 1
      y = y + 1
    x = x + 1

def elvish_turn_off(lights, x1, y1, x2, y2):
  x = x1
  while x != x2 + 1:
    y = y1
    while y != y2 + 1:
      lights[x][y] = lights[x][y] - 1
      if lights[x][y] < 0:
        lights[x][y] = 0
      y = y + 1
    x = x + 1

def elvish_toggle(lights, x1, y1, x2, y2):
  x = x1
  while x != x2 + 1:
    y = y1
    while y != y2 + 1:
      lights[x][y] = lights[x][y] + 2
      y = y + 1
    x = x + 1

def part2(directions):
  p = re.compile(r'((?:turn on)|toggle|(?:turn off)) (\d{1,4}),(\d{1,4}) through (\d{1,4}),(\d{1,4})', re.IGNORECASE)
  lights = [[0 for x in range(1000)] for x in range(1000)]
  for d in directions:
    m = p.match(d.rstrip())
    command = m.group(1)
    x1 = int(m.group(2))
    y1 = int(m.group(3))
    x2 = int(m.group(4))
    y2 = int(m.group(5))

    if command == 'turn on':
      elvish_turn_on(lights, x1, y1, x2, y2)
    elif command == 'turn off':
      elvish_turn_off(lights, x1, y1, x2, y2)
    elif command == 'toggle':
      elvish_toggle(lights, x1, y1, x2, y2)

  print("Part 2: Total brightness %d" % sum_brightness(lights))

def main():
  f = open('Day6Input.txt', 'r')
  directions = f.readlines()
  part1(directions)
  part2(directions)

if __name__ == "__main__":
  main()