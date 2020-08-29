"""
Fetches input for a particular year/day from adventofcode
"""

import argparse
import os
import requests

def create_input_file(year, day):
  """Fetches my input from Advent of Code."""
  daystr = 'Day' + str(day)
  os.mkdir(os.path.join(str(year), daystr, 'input'))
  url = 'https://adventofcode.com/' + str(year) + '/day/' + str(day) + '/input'
  cookies = dict()
  with open('session.txt', 'r') as file:
    cookies['session'] = file.read()

  path = os.path.join(str(year), daystr, 'input', 'input.txt')
  with open(path, 'w') as file:
    input_request = requests.get(url=url, cookies=cookies)
    file.write(input_request.text)

def main():
  """Fetches the input for an AoC day."""
  parser = argparse.ArgumentParser()
  parser.add_argument('--day', '-d', type=int)
  parser.add_argument('--year', '-y', type=int)
  args = parser.parse_args()

  create_input_file(args.year, args.day)

if __name__ == "__main__":
  main()
