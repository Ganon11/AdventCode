#!/usr/bin/ruby

require 'optparse'
require 'set'

def find_combination_totaling_target(expenses, n, target)
  return expenses.map(&:to_i).combination(n).find { |combo| combo.reduce(&:+) == target }.reduce(&:*)
end

def part1(expenses, target)
  return find_combination_totaling_target(expenses, 2, target)
end

def part2(expenses, target)
  return find_combination_totaling_target(expenses, 3, target)
end

options = {
  :part => 1,
  :filename => "../input/sample.txt"
}

OptionParser.new do |opts|
  opts.banner = "Usage: Day1.rb [options]"
  opts.on("-p PART", "--part=PART", Integer, "Part 1 or 2")
  opts.on("-f FILENAME", "--filename=FILENAME", String, "Which filename to use?")
end.parse!(into: options)

expenses = IO.readlines(options[:filename])
checksum = -1
target = 2020
if options[:part] == 1
  checksum = part1(expenses, target)
elsif options[:part] == 2
  checksum = part2(expenses, target)
end

if checksum != -1
  puts "Checksum: #{checksum}"
end
