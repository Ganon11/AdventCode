import json
import os
import re
import sys

def add_to_sum(num):
  add_to_sum.sum = add_to_sum.sum + int(num)
  return int(num)

def solve_part_1(filename):
  add_to_sum.sum = 0
  with open(filename, 'r') as fp:
    json_dump = json.load(fp, parse_int=add_to_sum)

  print("Part 1: %d" % add_to_sum.sum)

def filter_red_objects(obj):
  if "red" in obj.values():
    return {}
  return obj

def solve_part_2(filename):
  add_to_sum.sum = 0
  json_dump = None
  with open(filename, 'r') as fp:
    json_object = json.load(fp, object_hook=filter_red_objects)
    filtered_json_string = json.dumps(json_object)
    json.loads(filtered_json_string, parse_int=add_to_sum)

  print("Part 2: %d" % add_to_sum.sum)

def main():
  filename = 'Sample.txt'
  if len(sys.argv) > 1:
    filename = sys.argv[1]
  solve_part_1(filename)
  solve_part_2(filename)

if __name__ == "__main__":
  main()