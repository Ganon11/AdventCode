#!/usr/bin/ruby

require 'optparse'
require 'set'

class Tile
  attr_reader :id
  attr_reader :cols
  attr_reader :state

  def initialize(tile_string)
    lines = tile_string.split("\n")
    data = lines[0].match(/Tile (?<tile_id>\d+):/)
    if data
      @id = data[:tile_id].to_i
    else
      puts "Bad parse"
    end

    @grid = []
    @top = []
    @bottom = []
    @left = []
    @right = []
    lines.drop(1).each_with_index do |row, r|
      row.strip!
      gridrow = []
      row.chars.each_with_index do |ch, c|
        value = ch == '#' ? 1 : 0
        gridrow.push(value)

        if r == 0
          @top.push(value)
        elsif r == lines.length - 2
          @bottom.push(value)
        end

        if c == 0
          @left.push(value)
        elsif c == lines.length - 2
          @right.push(value)
        end
      end
      @grid.push(gridrow)
    end

    @state = nil
  end

  def edges
    [@top, @bottom, @left, @right, @top.reverse, @bottom.reverse, @left.reverse, @right.reverse]
  end

  def align_top_left_corner(corner_edges)
    alignment_found = false
    [:UP, :RIGHT, :LEFT, :DOWN, :UP_FLIPPED, :RIGHT_FLIPPED, :LEFT_FLIPPED, :DOWN_FLIPPED].each do |alignment|
      @state = alignment
      if corner_edges.include?(get_edge(:LEFT).join("")) and corner_edges.include?(get_edge(:UP).join(""))
        alignment_found = true
        break
      end
    end

    alignment_found
  end

  def align_edge(direction, edge)
    alignment_found = false
    [:UP, :RIGHT, :LEFT, :DOWN, :UP_FLIPPED, :RIGHT_FLIPPED, :LEFT_FLIPPED, :DOWN_FLIPPED].each do |alignment|
      @state = alignment
      if get_edge(direction) == edge
        alignment_found = true
        break
      end
    end

    alignment_found
  end

  def get_edge(direction)
    if @state == :UP
      case direction
      when :UP then @top
      when :RIGHT then @right
      when :LEFT then @left
      when :DOWN then @bottom
      end
    elsif @state == :RIGHT
      case direction
      when :UP then @left
      when :RIGHT then @top
      when :LEFT then @bottom
      when :DOWN then @right
      end
    elsif @state == :LEFT
      case direction
      when :UP then @right
      when :RIGHT then @bottom
      when :LEFT then @top
      when :DOWN then @left
      end
    elsif @state == :DOWN
      case direction
      when :UP then @bottom
      when :RIGHT then @left
      when :LEFT then @right
      when :DOWN then @top
      end
    elsif @state == :UP_FLIPPED
      case direction
      when :UP then @top.reverse
      when :RIGHT then @right.reverse
      when :LEFT then @left.reverse
      when :DOWN then @bottom.reverse
      end
    elsif @state == :RIGHT_FLIPPED
      case direction
      when :UP then @left.reverse
      when :RIGHT then @top.reverse
      when :LEFT then @bottom.reverse
      when :DOWN then @right.reverse
      end
    elsif @state == :LEFT_FLIPPED
      case direction
      when :UP then @right.reverse
      when :RIGHT then @bottom.reverse
      when :LEFT then @top.reverse
      when :DOWN then @left.reverse
      end
    elsif @state == :DOWN_FLIPPED
      case direction
      when :UP then @bottom.reverse
      when :RIGHT then @left.reverse
      when :LEFT then @right.reverse
      when :DOWN then @top.reverse
      end
    end
  end

  def get_innards
    copy = []
    (0..@grid.length - 1).each do |r|
      row = []
      (0..@grid[r].length - 1).each do |c|
        row.push(@grid[r][c])
      end
      copy.push(row)
    end

    if @state == :UP
      # do nothing
    elsif @state == :RIGHT
      copy = copy.transpose.map(&:reverse)
    elsif @state == :LEFT
      copy = copy.transpose.map(&:reverse).transpose.map(&:reverse).transpose.map(&:reverse)
    elsif @state == :DOWN
      copy = copy.transpose.map(&:reverse).transpose.map(&:reverse)
    elsif @state == :UP_FLIPPED
      copy = copy.map(&:reverse)
    elsif @state == :RIGHT_FLIPPED
      copy = copy.map(&:reverse).transpose.map(&:reverse)
    elsif @state == :LEFT_FLIPPED
      copy = copy.map(&:reverse).transpose.map(&:reverse).transpose.map(&:reverse).transpose.map(&:reverse)
    elsif @state == :DOWN_FLIPPED
      copy = copy.map(&:reverse).transpose.map(&:reverse).transpose.map(&:reverse)
    end

    innards = copy[1..-2]
    copy.each_with_index do |row, index|
      if index == 0 or index = copy.length - 1
        next
      end
      innards[index] = copy[row][1..-2]
    end

    return innards
  end
end

def print_puzzle_ids(puzzle)
  puzzle.each do |row|
    puts row.map{|tile| tile.id}.join(", ")
  end
end

def print_puzzle_alignments(puzzle)
  puzzle.each do |row|
    puts row.map{|tile| tile.state}.join(", ")
  end
end

def dump_puzzle_to_grid(puzzle, grid_size)
  grid = {}
  max_row = -1
  max_col = -1
  (0..grid_size-1).each do |r|
    (0..grid_size-1).each do |c|
      innards = puzzle[r][c].get_innards
      innards.each_with_index do |_, r2|
        innards[r2].each_with_index do |value, c2|
          actual_row = (r * innards.length) + r2
          actual_column = (c * innards[r2].length) + c2
          grid["#{actual_row},#{actual_column}"] = value
          if actual_row > max_row
            max_row = actual_row
          end

          if actual_column > max_col
            max_col = actual_column
          end
        end
      end
    end
  end

  actual_grid = []
  (0..max_row).each do |r|
    row = []
    (0..max_col).each do |c|
      row.push(grid["#{r},#{c}"])
    end
    actual_grid.push(row)
  end

  return actual_grid
end

def print_grid(grid)
  grid.each do |row|
    row.each do |value|
      if value == 0
        print ' '
      elsif value == 1
        print '#'
      else
        print value
      end
    end
    puts ""
  end
end

def shape_at_position?(grid, shape, row, col)
  shape.each_with_index do |shape_row, sr|
    shape_row.each_with_index do |value, sc|
      if value == 1 and grid[row + sr][col + sc] != 1
        return false
      end
    end
  end

  return true
end

def find_shape(grid, shape)
  shape_height = shape.length
  shape_width = shape[0].length
  positions = []

  grid.each_with_index do |row, r|
    if r + shape_height >= grid.length
      next
    end

    row.each_with_index do |_, c|
      if c + shape_width >= row.length
        next
      end

      if shape_at_position?(grid, shape, r, c)
        positions.push([r, c])
      end
    end
  end

  return positions
end

def rotate_grid(grid)
  return grid.transpose.map(&:reverse)
end

def mask_shape(grid, shape, row, col)
  shape.each_with_index do |shape_row, sr|
    shape_row.each_with_index do |value, sc|
      if value == 1
        grid[row + sr][col + sc] = 'O'
      end
    end
  end
end

options = {
  :part => 1,
  :filename => "../input/sample.txt"
}

OptionParser.new do |opts|
  opts.banner = "Usage: Day20.rb [options]"
  opts.on("-p PART", "--part=PART", Integer, "Part 1 or 2")
  opts.on("-f FILENAME", "--filename=FILENAME", String, "Which filename to use?")
end.parse!(into: options)

tiles = IO.read(options[:filename]).split("\n\n").map{|str| Tile::new(str)}
tile_map = {}
tiles.each do |t|
  tile_map[t.id] = t
end

grid_size = Math.sqrt(tiles.length).to_i

edge_to_tiles = {}
tiles.each do |tile|
  tile.edges.each do |edge|
    edge_str = edge.join("")
    if edge_to_tiles.key?(edge_str)
      edge_to_tiles[edge_str].push(tile.id)
    else
      edge_to_tiles[edge_str] = [tile.id]
    end
  end
end

tile_unique_edge_count = {}
edge_to_tiles.keys.each do |edge|
  if edge_to_tiles[edge].size == 1
    tile_id = edge_to_tiles[edge][0]
    if tile_unique_edge_count.key?(tile_id)
      tile_unique_edge_count[tile_id] += 1
    else
      tile_unique_edge_count[tile_id] = 1
    end
  end
end

corners = tile_unique_edge_count.select{|tile, count| count == 4}.keys
#puts "Corners:"
#puts corners

if options[:part] == 1
  puts corners.reduce(&:*)
end

edges = tile_unique_edge_count.select{|tile, count| count == 2}.keys
#puts "Edges:"
#puts edges

middles = tile_map.reject{ |k| corners.include?(k) or edges.include?(k) }.keys
#puts "Middles:"
#puts middles

puzzle = []
grid_size.times do |r|
  row = []
  grid_size.times do |c|
    row.push(nil)
  end
  puzzle.push(row)
end

# Pick a corner
puzzle[0][0] = tile_map[corners[0]]
assigned_tile_ids = Set[corners[0]]
(0..grid_size-1).each do |r|
  (0..grid_size-1).each do |c|
    if not puzzle[r][c].nil?
      next
    end

    tile_to_match = nil
    if r == 0
      tile_to_match = puzzle[r][c - 1]
    else
      tile_to_match = puzzle[r - 1][c]
    end

    new_tile_id = nil
    if (r == 0 or r == (grid_size - 1)) and (c == 0 or c == (grid_size - 1))
      # We're picking from corners
      new_tile_id = corners.find{|t| not assigned_tile_ids.member?(t) and tile_map[t].edges.any?{|te| tile_to_match.edges.any?{|me| te == me}}}
    elsif (r == 0 or r == (grid_size - 1)) or (c == 0 or c == (grid_size - 1))
      # We're picking from edges
      new_tile_id = edges.find{|t| not assigned_tile_ids.member?(t) and tile_map[t].edges.any?{|te| tile_to_match.edges.any?{|me| te == me}}}
    else
      # We're picking from middles
      new_tile_id = middles.find{|t| not assigned_tile_ids.member?(t) and tile_map[t].edges.any?{|te| tile_to_match.edges.any?{|me| te == me}}}
    end

    puzzle[r][c] = tile_map[new_tile_id]
    assigned_tile_ids.add(new_tile_id)
  end
end

#print_puzzle_ids(puzzle)

corner_edges = edge_to_tiles.keys.filter{|e| edge_to_tiles[e].size == 1}
#puts corner_edges

puzzle[0][0].align_top_left_corner(corner_edges)
#puts puzzle[0][0].state
(0..grid_size-1).each do |r|
  (0..grid_size-1).each do |c|
    if r == 0
      tile_to_match = puzzle[r][c - 1]
      puzzle[r][c].align_edge(:LEFT, tile_to_match.get_edge(:RIGHT))
    else
      tile_to_match = puzzle[r - 1][c]
      puzzle[r][c].align_edge(:UP, tile_to_match.get_edge(:DOWN))
    end
  end
end

#print_puzzle_alignments(puzzle)
grid = dump_puzzle_to_grid(puzzle, grid_size)
#print_grid(grid)


shape = [
  '                  # ',
  '#    ##    ##    ###',
  ' #  #  #  #  #  #   '
]

shape = shape.map{ |line| line.chars.map{ |c| c == '#' ? 1 : 0}}

grid = grid.map(&:reverse)
grid = rotate_grid(grid)
grid = rotate_grid(grid)
grid = rotate_grid(grid)
positions = find_shape(grid, shape)

positions.each do |p|
  mask_shape(grid, shape, p[0], p[1])
end

print_grid(grid)

sum = 0
grid.each do |row|
  sum += row.count {|v| v == 1}
end

puts sum
# positions.each do |p|
#   puts "#{p[0]},#{p[1]}"
# end
