#!/usr/bin/ruby

require 'optparse'
require 'set'

def part1(groups)
  sum = 0
  groups.each do |group|
    sum += group.reduce(Set[]) {|u, p| u = u.union(p)}.count
  end

  return sum
end

def part2(groups)
  sum = 0
  letters = ("a".."z").to_set
  groups.each do |group|
    sum += group.reduce(letters) {|i, p| i = i.intersection(p)}.count
  end

  return sum
end

options = {
  :part => 1,
  :filename => "../input/sample.txt"
}

OptionParser.new do |opts|
  opts.banner = "Usage: Day6.rb [options]"
  opts.on("-p PART", "--part=PART", Integer, "Part 1 or 2")
  opts.on("-f FILENAME", "--filename=FILENAME", String, "Which filename to use?")
end.parse!(into: options)

groups = []
current_people = []
IO.readlines(options[:filename]).each do |line|
  line.strip!
  if line == ''
    groups.push(current_people)
    current_people = []
  else
    current_people.push(line.chars.to_set)
  end
end

if not current_people.empty?
  groups.push(current_people)
  current_people = []
end

if options[:part] == 1
  puts part1(groups)
elsif options[:part] == 2
  puts part2(groups)
end
