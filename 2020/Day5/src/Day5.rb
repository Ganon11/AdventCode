#!/usr/bin/ruby

require 'optparse'

def get_row(line)
  rowchars = line.chars.slice(0, 7)
  min = 0
  max = 127
  rowchars.each do |c|
    avg = (min + max) / 2
    if c == 'F'
      max = avg
    else
      min = avg + 1
    end
  end

  if min != max
    puts "Didn't find row"
    return -1
  end

  return min
end

def get_col(line)
  colchars = line.chars.slice(7, 3)
  min = 0
  max = 7
  colchars.each do |c|
    avg = (min + max) / 2
    if c == 'L'
      max = avg
    else
      min = avg + 1
    end
  end

  if min != max
    return -1
  end

  return min
end

def get_id(r, c)
  return (r * 8) + c
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
  .map {|pass| get_id(get_row(pass.strip), get_col(pass.strip)) }
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
