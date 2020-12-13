#!/usr/bin/ruby

require 'optparse'

def part1(start, busses)
  i = start

  i += 1 while not busses.any? {|b| i % b == 0}

  return i, busses.find{|b| i % b == 0}
end

def part2(restrictions)
  busses = restrictions.map.with_index do |e, i|
    next if e == "x"
    e = e.to_i
    [e, (e - i) % e]
  end.compact

  busses.each do |b|
    puts "Bus #{b[0]}, wait time #{b[1]}"
  end
  
  time = busses[0][1]
  offset = busses[0][0]
  i = 0
  while i < busses.size - 1
    next_bus = busses[i + 1][0]
    next_wait = busses[i + 1][1]

    time += offset until (time % next_bus) == next_wait

    offset *= next_bus
    i += 1
  end

  return time
end

options = {
  :part => 1,
  :filename => "../input/sample.txt"
}

OptionParser.new do |opts|
  opts.banner = "Usage: Day13.rb [options]"
  opts.on("-p PART", "--part=PART", Integer, "Part 1 or 2")
  opts.on("-f FILENAME", "--filename=FILENAME", String, "Which filename to use?")
end.parse!(into: options)

start = nil
busses = []
File.open(options[:filename], "r") do |fh|
  start = fh.readline.to_i
  busses = fh.readline.split(",")
end

if options[:part] == 1
  answers = part1(start, busses.reject{|c| c == "x"}.map{|id| id.to_i})
  puts (answers[0] - start) * answers[1]
elsif options[:part] == 2
  puts part2(busses)
end
