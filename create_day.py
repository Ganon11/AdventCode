"""
Scaffolds an Advent of Code day.
"""

import argparse
import os
import shutil
import requests

def create_sublime_project(basedir, daystr, write_python, write_ruby, sample_count):
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
  [""")
    if write_python:
      file.write("""
    {
      "name": "Run Python Part 1",
      "cmd": ["py", "-3", "$file", "-f", "../input/input.txt", "-p", "1"]
    },
    {
      "name": "Run Python Part 2",
      "cmd": ["py", "-3", "$file", "-f", "../input/input.txt", "-p", "2"]
    },""")
      if sample_count == 1:
        file.write("""
    {
      "name": "Run Python Sample Part 1",
      "cmd": ["py", "-3", "$file", "-f", "../input/sample.txt", "-p", "1"]
    },
    {
      "name": "Run Python Sample Part 2",
      "cmd": ["py", "-3", "$file", "-f", "../input/sample.txt", "-p", "2"]
    },""")
      else:
        for i in range(sample_count):
          message = (
            "\n"
            "    {\n"
            f"      \"name\": \"Run Python Sample {i + 1} Part 1\",\n"
            f"      \"cmd\": [\"py\", \"-3\", \"$file\", \"-f\", \"../input/sample{i + 1}.txt\", \"-p\", \"1\"]\n"
            "    },\n"
            "    {\n"
            f"      \"name\": \"Run Python Sample {i + 1} Part 2\",\n"
            f"      \"cmd\": [\"py\", \"-3\", \"$file\", \"-f\", \"../input/sample{i + 1}.txt\", \"-p\", \"2\"]\n"
            "    },"
          )
          file.write(message)
    if write_ruby:
      file.write("""
    {
      "name": "Run Ruby Part 1",
      "cmd": ["ruby", "$file", "-f", "../input/input.txt", "-p", "1"]
    },
    {
      "name": "Run Ruby Part 2",
      "cmd": ["ruby", "$file", "-f", "../input/input.txt", "-p", "2"]
    },""")
      if sample_count == 1:
        file.write("""
    {
      "name": "Run Ruby Sample Part 1",
      "cmd": ["ruby", "$file", "-f", "../input/sample.txt", "-p", "1"]
    },
    {
      "name": "Run Ruby Sample Part 2",
      "cmd": ["ruby", "$file", "-f", "../input/sample.txt", "-p", "2"]
    },""")
      else:
        for i in range(sample_count):
          message = (
            "\n"
            "    {\n"
            f"      \"name\": \"Run Ruby Sample {i + 1} Part 1\",\n"
            f"      \"cmd\": [\"ruby\", \"$file\", \"-f\", \"../input/sample{i + 1}.txt\", \"-p\", \"1\"]\n"
            "    },\n"
            "    {\n"
            f"      \"name\": \"Run Ruby Sample {i + 1} Part 2\",\n"
            f"      \"cmd\": [\"ruby\", \"$file\", \"-f\", \"../input/sample{i + 1}.txt\", \"-p\", \"2\"]\n"
            "    },"
          )
          file.write(message)

    file.write("""
  ]
}
""")

def create_src_dir(basedir, daystr, write_python, write_ruby, sample_count):
  os.mkdir(os.path.join(basedir, 'src'))
  if write_python:
    create_python_src_file(basedir, daystr, sample_count)
  if write_ruby:
    create_ruby_src_file(basedir, daystr, sample_count)

def create_python_src_file(basedir, daystr, sample_count):
  """Creates a skeleton python file."""
  filename = ''
  if sample_count == 1:
    filename = '../input/sample.txt'
  else:
    filename = '../input/sample1.txt'
  with open(os.path.join(basedir, 'src', daystr + '.py'), 'w') as file:
    message = (
      "import argparse\n"
      "\n"
      "def main():\n"
      "  parser = argparse.ArgumentParser()\n"
      f"  parser.add_argument('-f', '--filename', default='{filename}')\n"
      "  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)\n"
      "  args = parser.parse_args()\n"
      "\n"
      "if __name__ == \"__main__\":\n"
      "  main()\n"
    )
    file.write(message)

def create_ruby_src_file(basedir, daystr, sample_count):
  """Creates a skeleton python file."""
  filename = ''
  if sample_count == 1:
    filename = '../input/sample.txt'
  else:
    filename = '../input/sample1.txt'
  with open(os.path.join(basedir, 'src', daystr + '.rb'), 'w') as file:
    message = (
      "#!/usr/bin/ruby\n"
      "\n"
      "require 'optparse'\n"
      "\n"
      "options = {\n"
      "  :part => 1,\n"
      f"  :filename => \"{filename}\"\n"
      "}\n"
      "\n"
      "OptionParser.new do |opts|\n"
      f"  opts.banner = \"Usage: {daystr}.rb [options]\"\n"
      "  opts.on(\"-p PART\", \"--part=PART\", Integer, \"Part 1 or 2\")\n"
      "  opts.on(\"-f FILENAME\", \"--filename=FILENAME\", String, \"Which filename to use?\")\n"
      "end.parse!(into: options)\n"
      "\n"
      "if options[:part] == 1\n"
      "  # do part 1\n"
      "elsif options[:part] == 2\n"
      "  # do part 2\n"
      "end\n"
    )
    file.write(message)

def create_input_file(basedir, day, year):
  """Fetches my input from Advent of Code."""
  os.mkdir(os.path.join(basedir, 'input'))
  url = 'https://adventofcode.com/' + str(year) + '/day/' + str(day) + '/input'
  cookies = dict()
  with open('session.txt', 'r') as file:
    cookies['session'] = file.read().strip()

  with open(os.path.join(basedir, 'input', 'input.txt'), 'w') as file:
    input_request = requests.get(url=url, cookies=cookies)
    file.write(input_request.text)

def create_sample_files(basedir, count):
  if count == 1:
    with open(os.path.join(basedir, 'input', 'sample.txt'), 'w') as file:
      file.write('Fill in sample here!')
  else:
    for i in range(count):
      with open(os.path.join(basedir, 'input', f'sample{i + 1}.txt'), 'w') as file:
        file.write('Fill in sample here!')

def main():
  """Scaffolds an AoC day."""
  parser = argparse.ArgumentParser()
  parser.add_argument('-d', '--day', type=int, required=True)
  parser.add_argument('-y', '--year', type=int, required=True)
  parser.add_argument('-e', '--delete', action='store_true')
  parser.add_argument('-r', '--ruby', action='store_true')
  parser.add_argument('-p', '--python', action='store_true')
  parser.add_argument('-s', '--samples', default=1, type=int)
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
      create_sublime_project(basedir, daystr, args.python, args.ruby, args.samples)
      create_src_dir(basedir, daystr, args.python, args.ruby, args.samples)
      create_input_file(basedir, args.day, args.year)
      create_sample_files(basedir, args.samples)

if __name__ == "__main__":
  main()
