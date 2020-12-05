#!/usr/bin/ruby

require 'optparse'

def binary_search_range(chars, min, max)
  chars.each do |c|
    avg = (min + max) / 2
    if c == 'F' or c == 'L'
      max = avg
    else
      min = avg + 1
    end
  end

  if min != max
    puts "Didn't find middle"
    return -1
  end

  return min
end

def get_row(pass)
  return binary_search_range(pass.chars.slice(0, 7), 0, 127)
end

def get_col(pass)
  return binary_search_range(pass.chars.slice(7, 3), 0, 7)
end

def get_id(pass)
  row = get_row(pass)
  col = get_col(pass)
  return (row * 8) + col
end

options = {
  :part => 1,
  :filename => "../input/sample.txt"
}

OptionParser.new do |opts|
  opts.banner = "Usage: Day5.rb [options]"
  opts.on("-p PART", "--part=PART", Integer, "Part 1 or 2")
  opts.on("-f FILENAME", "--filename=FILENAME", String, "Which filename to use?")
end.parse!(into: options)

ids = IO.readlines(options[:filename])
  .map {|pass| get_id(pass) }
  .sort!

if options[:part] == 1
  puts ids[-1]
elsif options[:part] == 2
  ids.each_with_index do |id, index|
    if index == 0
      next
    end

    if ids[index - 1] != id - 1
      puts "Missing ID found: #{id - 1}"
      break
    end
  end
end
