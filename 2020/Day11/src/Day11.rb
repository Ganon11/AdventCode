#!/usr/bin/ruby

require 'optparse'

def print_ferry(ferry)
  ferry.each do |row|
    row.each do |cell|
      print cell
    end
    puts ""
  end
  puts ""
end

def valid_coordinates?(ferry, row, col)
  if row < 0 or ferry.length <= row or col < 0 or ferry[0].length <= col
    return false
  end

  return true
end

def new_cell_value(ferry, row, col)
  if ferry[row][col] == '.' # floors stay floors
    return '.'
  end

  occupied = 0
  (-1 .. 1).each do |rdiff|
    (-1 .. 1).each do |cdiff|
      r = row + rdiff
      c = col + cdiff

      next if not valid_coordinates?(ferry, r, c) or (r == row and c == col)

      occupied += 1 if ferry[r][c] == '#'
    end
  end

  if ferry[row][col] == 'L' && occupied == 0
    return '#'
  elsif ferry[row][col] == '#' && occupied >= 4
    return 'L'
  else
    return ferry[row][col]
  end
end

def part1(ferry)
  old_ferry = ferry
  loop do
    new_ferry = Array.new(old_ferry.length) { Array.new(old_ferry[0].length) }
    old_ferry.each_with_index do |old_row, r|
      old_row.each_with_index do |old_col, c|
        new_ferry[r][c] = new_cell_value(old_ferry, r, c)
      end
    end

    break if old_ferry == new_ferry

    old_ferry = new_ferry
  end

  occupied = 0
  old_ferry.each do |row|
    occupied += row.count {|c| c == '#'}
  end

  return occupied
end

def new_cell_value_2(ferry, row, col)
  if ferry[row][col] == '.' # floors stay floors
    return '.'
  end

  occupied = 0
  (-1 .. 1).each do |rdiff|
    (-1 .. 1).each do |cdiff|
      next if rdiff == 0 and cdiff == 0

      r = row
      c = col
      loop do
        r += rdiff
        c += cdiff
        break if not valid_coordinates?(ferry, r, c)

        break if ferry[r][c] != '.'
      end

      next if not valid_coordinates?(ferry, r, c)

      occupied += 1 if ferry[r][c] == '#'
    end
  end

  if ferry[row][col] == 'L' && occupied == 0
    return '#'
  elsif ferry[row][col] == '#' && occupied >= 5
    return 'L'
  else
    return ferry[row][col]
  end
end

def part2(ferry)
  old_ferry = ferry
  loop do
    new_ferry = Array.new(old_ferry.length) { Array.new(old_ferry[0].length) }
    old_ferry.each_with_index do |old_row, r|
      old_row.each_with_index do |old_col, c|
        new_ferry[r][c] = new_cell_value_2(old_ferry, r, c)
      end
    end

    break if old_ferry == new_ferry

    old_ferry = new_ferry
  end

  occupied = 0
  old_ferry.each do |row|
    occupied += row.count {|c| c == '#'}
  end

  return occupied
end

options = {
  :part => 1,
  :filename => "../input/sample.txt"
}

OptionParser.new do |opts|
  opts.banner = "Usage: {daystr}.rb [options]"
  opts.on("-p PART", "--part=PART", Integer, "Part 1 or 2")
  opts.on("-f FILENAME", "--filename=FILENAME", String, "Which filename to use?")
end.parse!(into: options)

rows = IO.readlines(options[:filename]).map{ |line| line.strip }
num_rows = rows.length
num_cols = rows[0].length
ferry = Array.new(num_rows) { Array.new(num_cols) }
rows.each_with_index do |row, rowNum|
  row.chars.each_with_index do |cell, colNum|
    ferry[rowNum][colNum] = cell
  end
end

if options[:part] == 1
  puts part1(ferry)
elsif options[:part] == 2
  puts part2(ferry)
end
