"""
Scaffolds an Advent of Code day.
"""

import argparse
import os
import shutil
import requests

def create_sublime_project(basedir, daystr):
  """Creates a skeleton sublime project."""
  with open(os.path.join(basedir, daystr + '.sublime-project'), 'w') as file:
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
      "name": "Ruby Run Part 1",
      "cmd": ["ruby", "$file", "-f", "../input/input.txt", "-p", "1"]
    },
    {
      "name": "Ruby Run Part 2",
      "cmd": ["ruby", "$file", "-f", "../input/input.txt", "-p", "2"]
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
    {
      "name": "Ruby Run Sample 1 Part 1",
      "cmd": ["ruby", "$file", "-f", "../input/sample1.txt", "-p", "1"]
    },
    {
      "name": "Ruby Run Sample 2 Part 1",
      "cmd": ["ruby", "$file", "-f", "../input/sample2.txt", "-p", "1"]
    },
    {
      "name": "Ruby Run Sample 3 Part 1",
      "cmd": ["ruby", "$file", "-f", "../input/sample3.txt", "-p", "1"]
    },
    {
      "name": "Ruby Run Sample 1 Part 2",
      "cmd": ["ruby", "$file", "-f", "../input/sample1.txt", "-p", "2"]
    },
    {
      "name": "Ruby Run Sample 2 Part 2",
      "cmd": ["ruby", "$file", "-f", "../input/sample2.txt", "-p", "2"]
    },
    {
      "name": "Ruby Run Sample 3 Part 2",
      "cmd": ["ruby", "$file", "-f", "../input/sample3.txt", "-p", "2"]
    }
  ]
}""")

def create_src_dir(basedir, daystr):
  os.mkdir(os.path.join(basedir, 'src'))
  create_python_src_file(basedir, daystr)
  create_ruby_src_file(basedir, daystr)

def create_python_src_file(basedir, daystr):
  """Creates a skeleton python file."""
  with open(os.path.join(basedir, 'src', daystr + '.py'), 'w') as file:
    file.write("""import argparse

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample1.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

if __name__ == "__main__":
  main()
""")

def create_ruby_src_file(basedir, daystr):
  """Creates a skeleton python file."""
  with open(os.path.join(basedir, 'src', daystr + '.rb'), 'w') as file:
    file.write(f"""#!/usr/bin/ruby

require 'optparse'

options = {{
  :part => 1,
  :filename => "../input/sample.txt"
}}

OptionParser.new do |opts|
  opts.banner = "Usage: {daystr}.rb [options]"
  opts.on("-p PART", "--part=PART", Integer, "Part 1 or 2")
  opts.on("-f FILENAME", "--filename=FILENAME", String, "Which filename to use?")
end.parse!(into: options)

if options[:part] == 1
  # do part 1
elsif options[:part] == 2
  # do part 2
end
""")

def create_input_file(basedir, day, year):
  """Fetches my input from Advent of Code."""
  os.mkdir(os.path.join(basedir, 'input'))
  url = 'https://adventofcode.com/' + str(year) + '/day/' + str(day) + '/input'
  cookies = dict()
  with open('session.txt', 'r') as file:
    cookies['session'] = file.read()

  with open(os.path.join(basedir, 'input', 'input.txt'), 'w') as file:
    input_request = requests.get(url=url, cookies=cookies)
    file.write(input_request.text)

def main():
  """Scaffolds an AoC day."""
  parser = argparse.ArgumentParser()
  parser.add_argument('day', type=int)
  parser.add_argument('-d', '--delete', action='store_true')
  parser.add_argument('year', type=int)
  args = parser.parse_args()

  yearstr = str(args.year)
  daystr = 'Day' + str(args.day)
  basedir = os.path.join(yearstr, daystr)

  if args.delete:
    if not os.path.exists(basedir):
      print(f'{basedir} does not exist.')
    else:
      shutil.rmtree(basedir)
  else:
    if os.path.exists(basedir):
      print(f'{basedir} already exists.')
    else:
      os.mkdir(basedir)
      create_sublime_project(basedir, daystr)
      create_src_dir(basedir, daystr)
      create_input_file(basedir, args.day, args.year)

if __name__ == "__main__":
  main()
