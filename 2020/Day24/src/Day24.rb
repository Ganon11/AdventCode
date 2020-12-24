#!/usr/bin/ruby

require 'optparse'
require 'set'

def get_hex_coordinates(direction)
  i = 0
  x = 0
  y = 0
  z = 0
  while i < direction.length
    if direction[i] == 'e'
      x += 1
      y -= 1
      i += 1
    elsif direction[i] == 'w'
      x -= 1
      y += 1
      i += 1
    elsif direction[i] == 's' and direction[i + 1] == 'e'
      y -= 1
      z += 1
      i += 2
    elsif direction[i] == 's' and direction[i + 1] == 'w'
      x -= 1
      z += 1
      i += 2
    elsif direction[i] == 'n' and direction[i + 1] == 'e'
      x += 1
      z -= 1
      i += 2
    elsif direction[i] == 'n' and direction[i + 1] == 'w'
      y += 1
      z -= 1
      i += 2
    end
  end

  return x, y, z
end

def get_initial_grid(directions)
  grid = Set[]
  directions.each do |direction|
    x, y, z = get_hex_coordinates(direction)
    key = [x, y, z]
    grid.delete(key) unless grid.add?(key)
  end

  return grid
end

def get_neighbors(hex)
  n1 = [hex[0] + 1, hex[1] - 1, hex[2]]
  n2 = [hex[0], hex[1] - 1, hex[2] + 1]
  n3 = [hex[0] - 1, hex[1], hex[2] + 1]
  n4 = [hex[0] - 1, hex[1] + 1, hex[2]]
  n5 = [hex[0], hex[1] + 1, hex[2] - 1]
  n6 = [hex[0] + 1, hex[1], hex[2] - 1]

  return [n1, n2, n3, n4, n5, n6]
end

def count_active_neighbors(grid, hex)
  sum = 0
  get_neighbors(hex).each do |n|
    if grid.include?(n)
      sum += 1
    end
  end

  return sum
end

def play_round(grid)
  new_grid = Set[]

  hexes_to_check = Set[]
  grid.each do |hex|
    hexes_to_check.add(hex)
    get_neighbors(hex).each do |n|
      hexes_to_check.add(n)
    end
  end

  hexes_to_check.each do |hex|
    count = count_active_neighbors(grid, hex)
    if grid.include?(hex) and (count == 1 or count == 2)
      new_grid.add(hex)
    elsif count == 2
      new_grid.add(hex)
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
