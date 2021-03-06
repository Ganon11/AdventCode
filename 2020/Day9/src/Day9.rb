#!/usr/bin/ruby

require 'optparse'

def part1(values, size)
  seen = []
  invalid_value = -1
  values.each do |value|
    if seen.length < size
      seen.push(value)
      next
    end

    combos = seen.combination(2).to_a
    if not combos.any? {|n| n.sum == value}
      invalid_value = value
      break
    end

    seen.shift
    seen.push(value)
  end

  return invalid_value
end

def part2(values, size)
  target = part1(values, size)
  running_values = []
  running_sum = 0
  i = 0
  while i < values.length
    v = values[i]
    if running_sum == target
      running_values.sort!
      return running_values[0] + running_values[-1]
    elsif running_sum < target
      running_values.push(values[i])
      running_sum += values[i]
      i += 1
    else
      old = running_values.shift
      running_sum -= old
    end
  end
end

options = {
  :part => 1,
  :filename => "../input/sample.txt",
  :size => 5
}

OptionParser.new do |opts|
  opts.banner = "Usage: {daystr}.rb [options]"
  opts.on("-p PART", "--part=PART", Integer, "Part 1 or 2")
  opts.on("-f FILENAME", "--filename=FILENAME", String, "Which filename to use?")
  opts.on("-s SIZE", "--size=SIZE", Integer, "How large is the preamble?")
end.parse!(into: options)

values = IO.readlines(options[:filename]).map{|line| line.to_i}

if options[:part] == 1
  puts part1(values, options[:size])
elsif options[:part] == 2
  puts part2(values, options[:size])
end
