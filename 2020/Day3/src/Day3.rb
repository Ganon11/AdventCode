#!/usr/bin/ruby

require 'optparse'

def add_trees(forest, line)
  row = []
  line.chars.each do |c|
    if c == '.'
      row.push(0)
    elsif c == '#'
      row.push(1)
    end
  end

  forest.push row
end

def print_forest(forest)
  forest.each do |row|
    row.each do |space|
      if space == 1
        print '#'
      else
        print '.'
      end
    end
    print "\n"
  end
end

def check_slope(forest, rowdelta, coldelta)
  r = 0
  c = 0
  trees = 0
  while r < forest.length
    #puts "Row #{r} col #{c} forest[r][c] #{forest[r][c]}"
    if forest[r][c] == 1
      trees += 1
    end

    c += coldelta
    if forest[r].length <= c
      c -= forest[r].length
    end

    r += rowdelta
  end

  return trees
end

def part1(forest)
  return check_slope(forest, 1, 3)
end

def part2(forest)
  slopes = [ [1, 1], [1, 3], [1, 5], [1, 7], [2, 1] ]
  return slopes.reduce(1) {|product, pair| product * check_slope(forest, pair[0], pair[1])}
end

options = {
  :part => 1,
  :filename => "../input/sample.txt"
}

OptionParser.new do |opts|
  opts.banner = "Usage: Day3.rb [options]"
  opts.on("-p PART", "--part=PART", Integer, "Part 1 or 2")
  opts.on("-f FILENAME", "--filename=FILENAME", String, "Which filename to use?")
end.parse!(into: options)

forest = []
IO.readlines(options[:filename]).each do |line|
  add_trees(forest, line)
end

#print_forest(forest)

if options[:part] == 1
  # do part 1
  puts part1(forest)
elsif options[:part] == 2
  # do part 2
  puts part2(forest)
end
