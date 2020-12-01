#!/usr/bin/ruby

require 'optparse'

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

if options[:part] == 1
  # do part 1
elsif options[:part] == 2
  # do part 2
end
