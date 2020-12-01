#!/usr/bin/ruby

require 'optparse'

def part1(expenses)
  i1 = 0
  while i1 < (expenses.length - 1) do
    e1 = expenses[i1].to_i

    i2 = i1 + 1
    while i2 < expenses.length do
      e2 = expenses[i2].to_i

      if e1 + e2 == 2020
        return e1 * e2
      end

      i2 += 1
    end

    i1 += 1
  end

  return -1
end

def part2(expenses)
  i1 = 0
  while i1 < (expenses.length - 2) do
    e1 = expenses[i1].to_i

    i2 = i1 + 1
    while i2 < (expenses.length - 1) do
      e2 = expenses[i2].to_i

      i3 = i2 + 1
      while i3 < expenses.length do
        e3 = expenses[i3].to_i

        if e1 + e2 + e3 == 2020
          return e1 * e2 * e3
        end

        i3 += 1
      end

      i2 += 1
    end

    i1 += 1
  end

  return -1
end

options = {
  :part => 1,
  :filename => "../input/sample.txt"
}

OptionParser.new do |opts|
  opts.banner = "Usage: Day1.rb [options]"

  opts.on("-p PART", "--part=PART", Integer, "Part 1 or 2") do |p|
    options[:part] = p
  end

  opts.on("-f FILENAME", "--filename=FILENAME", String, "Which filename to use?") do |f|
    options[:filename] = f
  end
end.parse!

expenses = IO.readlines(options[:filename])
checksum = -1
if options[:part] == 1
  checksum = part1(expenses)
elsif options[:part] == 2
  checksum = part2(expenses)
end

if checksum != -1
  puts "Checksum: #{checksum}"
end
