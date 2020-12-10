#!/usr/bin/ruby

require 'optparse'

def part1(adapters)
  target = adapters[-1] + 3
  diffs = {
    1=>0,
    2=>0,
    3=>1 # The last jump to our device is always 3
  }
  adapters.each_with_index do |value, index|
    if index == 0
      diffs[value] += 1
      next
    end

    diff = value - adapters[index - 1]
    if diff <= 0 or 3 < diff
      puts "Invalid adapter change at #{index}"
      break
    end

    diffs[diff] += 1
  end

  return diffs[1] * diffs[3]
end

def part2(adapters)
  target = adapters[-1] + 3
  adapters.push(target)
  dynamic = {0 => 1}
  adapters.each_with_index do |adapter, index|
    sum = 0
    (1..3).each do |diff|
      if dynamic.key?(adapter - diff)
        sum += dynamic[adapter - diff]
      end
    end

    dynamic[adapter] = sum
  end

  return dynamic[target]
end

options = {
  :part => 1,
  :filename => "../input/sample1.txt"
}

OptionParser.new do |opts|
  opts.banner = "Usage: {daystr}.rb [options]"
  opts.on("-p PART", "--part=PART", Integer, "Part 1 or 2")
  opts.on("-f FILENAME", "--filename=FILENAME", String, "Which filename to use?")
end.parse!(into: options)

adapters = IO.readlines(options[:filename])
  .map{|line| line.strip.to_i}
  .sort

if options[:part] == 1
  puts part1(adapters)
elsif options[:part] == 2
  puts part2(adapters)
end
