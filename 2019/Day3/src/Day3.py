import argparse
import os
import Position

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample1.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  text = list()
  with open(args.filename, 'r') as f:
    text = f.readlines()

  path1 = Position.walk_path(text[0].split(','))
  path2 = Position.walk_path(text[1].split(','))

  path1set = {p for p in path1 if p.x != 0 and p.y != 0}
  path2set = {p for p in path2 if p.x != 0 and p.y != 0}
  intersections = path1set.intersection(path2set)
  if args.part == 1:
    intersections = sorted(intersections, key=lambda p: p.DistanceToOrigin())
    print(f'Closest intersection distance: {intersections[0].DistanceToOrigin()}')
  elif args.part == 2:
    intersections = sorted(intersections, key=lambda p: path1.index(p) + path2.index(p))
    print(f'Best step count: {path1.index(intersections[0]) + path2.index(intersections[0]) + 2}')
  else:
    print(f'Unrecognized part {args.part}')

if __name__ == "__main__":
  main()