"""
Fetches input for a particular year/day from adventofcode
"""

import argparse
from datetime import date
import os
import requests

def create_input_file(year, day, path=None):
  """Fetches my input from Advent of Code."""
  daystr = 'Day' + str(day)
  url = 'https://adventofcode.com/' + str(year) + '/day/' + str(day) + '/input'

  cookies = dict()
  with open('session.txt', 'r') as file:
    cookies['session'] = file.read().strip()

  if path is None:
    path = os.path.join(str(year), daystr, 'input', 'input.txt')

  dirname = os.path.dirname(path)
  if dirname != '' and not os.path.exists(dirname):
    os.makedirs(dirname)

  with open(path, 'w') as file:
    input_request = requests.get(url=url, cookies=cookies)
    file.write(input_request.text)

def get_day(day):
  """Gets today's date, or the date passed in."""
  if day is not None:
    return day

  return date.today().day

def get_year(year):
  """Gets today's date, or the date passed in."""
  if year is not None:
    return year

  return date.today().year

def main():
  """Fetches the input for an AoC day."""
  parser = argparse.ArgumentParser()
  parser.add_argument('--day', '-d', default=None, type=int)
  parser.add_argument('--year', '-y', default=None, type=int)
  parser.add_argument('--path', '-p', type=str)
  args = parser.parse_args()

  day = get_day(args.day)
  year = get_year(args.year)
  print(f'Fetching input for Year {year}, Day {day}')
  create_input_file(year, day, args.path)

if __name__ == "__main__":
  main()
