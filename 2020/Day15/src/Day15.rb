#!/usr/bin/ruby

require 'optparse'

def memory_game(numbers, target)
  turn = 0
  last_seen_indexes = {}
  last_spoken = nil
  while turn < target
    if turn < numbers.length
      last_spoken = numbers[turn]
    else
      indexes = last_seen_indexes[last_spoken]
      if indexes.length == 1
        last_spoken = 0
      else
        last_spoken = indexes[-1] - indexes[-2]
      end
    end

    if last_seen_indexes.key?(last_spoken)
      last_seen_indexes[last_spoken].push(turn)
    else
      last_seen_indexes[last_spoken] = [turn]
    end

    turn += 1
  end

  return last_spoken
end

def part1(numbers)
  return memory_game(numbers, 2020)
end

def part2(numbers)
  return memory_game(numbers, 30000000)
end

options = {
  :part => 1,
  :filename => "../input/sample1.txt"
}

OptionParser.new do |opts|
  opts.banner = "Usage: Day15.rb [options]"
  opts.on("-p PART", "--part=PART", Integer, "Part 1 or 2")
  opts.on("-f FILENAME", "--filename=FILENAME", String, "Which filename to use?")
end.parse!(into: options)

numbers = IO.readlines(options[:filename])[0].split(',').map{|d| d.to_i}

if options[:part] == 1
  puts part1(numbers)
elsif options[:part] == 2
  puts part2(numbers)
end
