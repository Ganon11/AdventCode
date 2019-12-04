import argparse
import os
import requests
import shutil

def create_sublime_project(daystr):
  with open(os.path.join(daystr, daystr + '.sublime-project'), 'w') as f:
    f.write("""{
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
  os.mkdir(os.path.join(daystr, 'src'))
  with open(os.path.join(daystr, 'src', daystr + '.py'), 'w') as f:
    f.write("""import argparse
import os

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample1.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

if __name__ == "__main__":
  main()""")

def create_input_file(daystr, day):
  os.mkdir(os.path.join(daystr, 'input'))
  url = 'https://adventofcode.com/2019/day/' + str(day) + '/input'
  with open(os.path.join(daystr, 'input', 'input.txt'), 'w') as f:
    r = requests.get(url=url)
    f.write(r.text)

def main():
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