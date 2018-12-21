import os
import re

def main():
  f = open('Input.txt', 'r')
  presents = f.readlines()
  p = re.compile('(\d+)x(\d+)x(\d+)', re.IGNORECASE)
  paper = 0
  ribbon = 0
  for present in presents:
    m = p.match(present)
    l = int(m.group(1))
    w = int(m.group(2))
    h = int(m.group(3))
    side1 = l * w
    side2 = w * h
    side3 = h * l

    # Wrapping paper is total surface area, plus extra equal to the smallest side
    paper = paper + (2 * side1) + (2 * side2) + (2 * side3) + min(side1, side2, side3)

    # Ribbon is equal to total volume, plus the shortest distance around the sides.
    ribLength = int(2 * (l + w + h - max(l, w, h)))
    ribbon = ribbon + (l * w * h) + ribLength

  print(str(paper) + ' ft^2 of wrapping paper, ' + str(ribbon) + ' ft of ribbon')

if __name__ == "__main__":
  main()