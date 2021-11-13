#!/usr/bin/ruby

require 'optparse'
require 'set'

def get_hex_coordinates(direction)
  i = 0
  q = 0
  r = 0
  while i < direction.length
    if direction[i] == 'e'
      q += 1
      i += 1
    elsif direction[i] == 'w'
      q -= 1
      i += 1
    elsif direction[i] == 's' and direction[i + 1] == 'e'
      r += 1
      i += 2
    elsif direction[i] == 's' and direction[i + 1] == 'w'
      q -= 1
      r += 1
      i += 2
    elsif direction[i] == 'n' and direction[i + 1] == 'e'
      q += 1
      r -= 1
      i += 2
    elsif direction[i] == 'n' and direction[i + 1] == 'w'
      r -= 1
      i += 2
    end
  end

  return [q, r]
end

def get_initial_grid(directions)
  grid = Set[]
  directions.each do |direction|
    key = get_hex_coordinates(direction)
    grid.delete(key) unless grid.add?(key)
  end

  return grid
end

def get_neighbors(hex)
  n1 = [hex[0] + 1, hex[1] - 1]
  n2 = [hex[0] + 1, hex[1]]
  n3 = [hex[0], hex[1] + 1]
  n4 = [hex[0] - 1, hex[1] + 1]
  n5 = [hex[0] - 1, hex[1]]
  n6 = [hex[0], hex[1] - 1]

  return [n1, n2, n3, n4, n5, n6]
end

def count_active_neighbors(grid, hex)
  return get_neighbors(hex).count {|n| grid.include?(n)}
end

def check_hex(grid, hex, new_grid, checked)
  if not checked.member?(hex)
    count = count_active_neighbors(grid, hex)
    if count == 2 or (grid.include?(hex) and count == 1)
      new_grid.add(hex)
    end
    checked.add(hex)
  end
end

def play_round(grid)
  new_grid = Set[]

  checked_hexes = Set[]
  grid.each do |hex|
    check_hex(grid, hex, new_grid, checked_hexes)

    get_neighbors(hex).each do |n|
      check_hex(grid, n, new_grid, checked_hexes)
    end
  end

  return new_grid
end

options = {
  :part => 1,
  :filename => "../input/sample.txt"
}

OptionParser.new do |opts|
  opts.banner = "Usage: Day24.rb [options]"
  opts.on("-p PART", "--part=PART", Integer, "Part 1 or 2")
  opts.on("-f FILENAME", "--filename=FILENAME", String, "Which filename to use?")
end.parse!(into: options)

directions = IO.readlines(options[:filename]).map{|l| l.strip}
grid = get_initial_grid(directions)

if options[:part] == 2
  100.times do
    grid = play_round(grid)
  end
end

puts grid.size
