"""
Fetches input for a particular year/day from adventofcode
"""

import argparse
import os
import requests

def create_input_file(year, day, path=None):
  """Fetches my input from Advent of Code."""
  daystr = 'Day' + str(day)
  url = 'https://adventofcode.com/' + str(year) + '/day/' + str(day) + '/input'

  cookies = dict()
  with open('session.txt', 'r') as file:
    cookies['session'] = file.read()

  if path is None:
    path = os.path.join(str(year), daystr, 'input', 'input.txt')

  dirname = os.path.dirname(path)
  if dirname != '' and not os.path.exists(dirname):
    os.makedirs(dirname)

  with open(path, 'w') as file:
    input_request = requests.get(url=url, cookies=cookies)
    file.write(input_request.text)

def main():
  """Fetches the input for an AoC day."""
  parser = argparse.ArgumentParser()
  parser.add_argument('--day', '-d', type=int)
  parser.add_argument('--year', '-y', type=int)
  parser.add_argument('--path', '-p', type=str)
  args = parser.parse_args()

  if args.day is None:
    for day in range(25):
      create_input_file(args.year, day + 1)
  else:
    create_input_file(args.year, args.day, args.path)

if __name__ == "__main__":
  main()
