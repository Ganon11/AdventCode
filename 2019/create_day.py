"""
Scaffolds a 2019 Advent of Code day.
"""

import argparse
import os
import shutil
import requests

def create_sublime_project(daystr):
  """Creates a skeleton sublime project."""
  with open(os.path.join(daystr, daystr + '.sublime-project'), 'w') as file:
    file.write("""{
  "folders":
  [
    {
      "path": "src",
      "name": "Source"
    },
    {
      "path": "input",
      "name": "Input files"
    }
  ],
  "build_systems":
  [
    {
      "name": "Run Part 1",
      "cmd": ["python", "$file", "-f", "../input/input.txt", "-p", "1"]
    },
    {
      "name": "Run Part 2",
      "cmd": ["python", "$file", "-f", "../input/input.txt", "-p", "2"]
    },
    {
      "name": "Run Sample 1 Part 1",
      "cmd": ["python", "$file", "-f", "../input/sample1.txt", "-p", "1"]
    },
    {
      "name": "Run Sample 2 Part 1",
      "cmd": ["python", "$file", "-f", "../input/sample2.txt", "-p", "1"]
    },
    {
      "name": "Run Sample 3 Part 1",
      "cmd": ["python", "$file", "-f", "../input/sample3.txt", "-p", "1"]
    },
    {
      "name": "Run Sample 1 Part 2",
      "cmd": ["python", "$file", "-f", "../input/sample1.txt", "-p", "2"]
    },
    {
      "name": "Run Sample 2 Part 2",
      "cmd": ["python", "$file", "-f", "../input/sample2.txt", "-p", "2"]
    },
    {
      "name": "Run Sample 3 Part 2",
      "cmd": ["python", "$file", "-f", "../input/sample3.txt", "-p", "2"]
    }
  ]
}""")

def create_src_file(daystr):
  """Creates a skeleton python file."""
  os.mkdir(os.path.join(daystr, 'src'))
  with open(os.path.join(daystr, 'src', daystr + '.py'), 'w') as file:
    file.write("""import argparse
import os

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample1.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

if __name__ == "__main__":
  main()""")

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
  """Scaffolds a 2019 AoC day."""
  parser = argparse.ArgumentParser()
  parser.add_argument('day', type=int)
  parser.add_argument('-d', '--delete', action='store_true')
  args = parser.parse_args()

  daystr = 'Day' + str(args.day)

  if args.delete:
    shutil.rmtree(daystr)
  else:
    os.mkdir(daystr)
    create_sublime_project(daystr)
    create_src_file(daystr)
    create_input_file(daystr, args.day)

if __name__ == "__main__":
  main()
