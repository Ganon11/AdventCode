#!/usr/bin/env python3

"""
Scaffolds an Advent of Code day.
"""

import argparse
from datetime import date
import os
import shutil
import requests

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
  """Creates a number of sample files."""
  if count == 1:
    with open(os.path.join(basedir, 'input', 'sample.txt'), 'w') as file:
      file.write('Fill in sample here!')
  else:
    for i in range(count):
      with open(os.path.join(basedir, 'input', f'sample{i + 1}.txt'), 'w') as file:
        file.write('Fill in sample here!')

def create_cmake_file(basedir, daystr):
  text = (
    f'cmake_minimum_required(VERSION 3.10)\n'
    f'\n'
    f'project(d{daystr} VERSION 1.0)\n'
    f'\n'
    f'file(GLOB SRC_FILES *.cpp)\n'
    f'add_executable(d{daystr} ${{SRC_FILES}})\n'
    f'target_link_libraries(d{daystr} AoCHelpers)\n'
  )
  with open(os.path.join(basedir, 'CMakeLists.txt'), 'w') as file:
    file.write(text)

def add_day_to_root_cmake(daystr):
  new_day = f'add_subdirectory(day{daystr})'
  with open('CMakeLists.txt', 'r+') as file:
    for line in file:
      if new_day in line:
        break
    else:
      file.write(f'{new_day}\n')

def create_entry_point(basedir, day):
  text = (
    f'#include <iostream>\n'
    f'\n'
    f'#include "cxxopts.hpp"\n'
    f'#include "input_handler.h"\n'
    f'\n'
    f'int main(int argc, char* argv[])\n'
    f'{{\n'
    f'  cxxopts::Options options("d{day}", "Day {day} of Advent of Code");\n'
    f'  options.add_options()\n'
    f'    ("f,filename", "Input Filename", cxxopts::value<std::string>())\n'
    f'    ("h,help", "Print usage")\n'
    f'  ;\n'
    f'\n'
    f'  auto result = options.parse(argc, argv);\n'
    f'  if (result.count("help"))\n'
    f'  {{\n'
    f'    std::cout << options.help() << std::endl;\n'
    f'    return 0;\n'
    f'  }}\n'
    f'\n'
    f'  if (!result.count("filename"))\n'
    f'  {{\n'
    f'    return -1;\n'
    f'  }}\n'
    f'\n'
    f'  advent_of_code::InputHandler input{{ result["filename"].as<std::string>() }};\n'
    f'  return 0;\n'
    f'}}\n'
  )

  with open(os.path.join(basedir, f'day{day}.cpp'), 'w') as file:
    file.write(text)

def get_day(day):
  """Gets today's date, or the date passed in."""
  if day is not None:
    return day

  return date.today().day

def main():
  """Scaffolds an AoC day."""
  parser = argparse.ArgumentParser()
  parser.add_argument('-d', '--day', default=None, type=int)
  parser.add_argument('-s', '--samples', default=1, type=int)
  parser.add_argument('-e', '--delete', action='store_true')
  parser.add_argument('-i', '--input-only', action='store_true')
  args = parser.parse_args()

  day = get_day(args.day)
  year = date.today().year

  daystr = 'day' + str(day)
  basedir = os.path.join(daystr)

  if args.delete:
    if not os.path.exists(basedir):
      print(f'{basedir} does not exist.')
    else:
      shutil.rmtree(basedir)
  elif args.input_only: 
    if not os.path.exists(basedir):
      os.mkdir(basedir)

    create_input_file(basedir, day, year)
  else:
    if os.path.exists(basedir):
      print(f'{basedir} already exists.')
    else:
      os.mkdir(basedir)
      create_input_file(basedir, day, year)
      create_sample_files(basedir, args.samples)
      create_cmake_file(basedir, day)
      add_day_to_root_cmake(day)
      create_entry_point(basedir, day)

if __name__ == "__main__":
  main()
