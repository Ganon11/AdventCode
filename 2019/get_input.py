"""
Scaffolds a 2019 Advent of Code day.
"""

import argparse
import os
import requests

def create_input_file(daystr, day):
  """Fetches my input from Advent of Code."""
  os.mkdir(os.path.join(daystr, 'input'))
  url = 'https://adventofcode.com/2019/day/' + str(day) + '/input'
  cookies = dict()
  with open('session.txt', 'r') as file:
    cookies['session'] = file.read()

  with open(os.path.join(daystr, 'input', 'input.txt'), 'w') as file:
    input_request = requests.get(url=url, cookies=cookies)
    file.write(input_request.text)

def main():
  """Fetches the input for an AoC day."""
  parser = argparse.ArgumentParser()
  parser.add_argument('day', type=int)
  args = parser.parse_args()

  daystr = 'Day' + str(args.day)

  create_input_file(daystr, args.day)

if __name__ == "__main__":
  main()
