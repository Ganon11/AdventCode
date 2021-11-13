#!/usr/bin/ruby

require 'optparse'
require 'set'

class Tile
  attr_reader :id
  attr_reader :state

  def initialize(tile_string)
    lines = tile_string.split("\n")
    data = lines[0].match(/Tile (?<tile_id>\d+):/)
    if data
      @id = data[:tile_id].to_i
    else
      puts "Bad parse"
    end

    @grid = Array.new(10) { Array.new(10) { 0 }}
    top = []
    bottom = []
    left = []
    right = []
    lines.drop(1).each_with_index do |row, r|
      row.strip!
      row.chars.each_with_index do |ch, c|
        value = ch == '#' ? 1 : 0
        @grid[r][c] = value

        if r == 0
          top.push(value)
        elsif r == lines.length - 2
          bottom.push(value)
        end

        if c == 0
          left.push(value)
        elsif c == lines.length - 2
          right.push(value)
        end
      end
    end

    @top = top.join("").to_i(base=2)
    @bottom = bottom.join("").to_i(base=2)
    @left = left.join("").to_i(base=2)
    @right = right.join("").to_i(base=2)
    @top_flipped = top.reverse.join("").to_i(base=2)
    @bottom_flipped = bottom.reverse.join("").to_i(base=2)
    @left_flipped = left.reverse.join("").to_i(base=2)
    @right_flipped = right.reverse.join("").to_i(base=2)

    @state = :UP
  end

  def edges
    [@top, @left, @bottom, @right, @top_flipped, @left_flipped, @bottom_flipped, @right_flipped]
  end

  def align_top_left_corner(corner_edges)
    [:UP, :RIGHT, :DOWN, :LEFT, :UP_FLIPPED, :RIGHT_FLIPPED, :DOWN_FLIPPED, :LEFT_FLIPPED].each do |alignment|
      @state = alignment
      if corner_edges.include?(get_edge(:LEFT)) and corner_edges.include?(get_edge(:UP))
        return true
      end
    end

    return false
  end

  def align_edge(direction, edge)
    [:UP, :RIGHT, :DOWN, :LEFT, :UP_FLIPPED, :RIGHT_FLIPPED, :DOWN_FLIPPED, :LEFT_FLIPPED].each do |alignment|
      @state = alignment
      our_edge = get_edge(direction)
      if our_edge == edge
        return true
        break
      end
    end

    return false
  end

  def get_edge(direction)
    if @state == :UP
      if direction == :UP
        @top
      elsif direction == :RIGHT
        @right
      elsif direction == :DOWN
        @bottom
      elsif direction == :LEFT
        @left
      end
    elsif @state == :RIGHT
      if direction == :UP
        @left_flipped
      elsif direction == :RIGHT
        @top
      elsif direction == :DOWN
        @right_flipped
      elsif direction == :LEFT
        @bottom
      end
    elsif @state == :DOWN
      if direction == :UP
        @bottom_flipped
      elsif direction == :RIGHT
        @left_flipped
      elsif direction == :DOWN
        @top_flipped
      elsif direction == :LEFT
        @right_flipped
      end
    elsif @state == :LEFT
      if direction == :UP
        @right
      elsif direction == :RIGHT
        @bottom_flipped
      elsif direction == :DOWN
        @left
      elsif direction == :LEFT
        @top_flipped
      end
    elsif @state == :UP_FLIPPED
      if direction == :UP
        @top_flipped
      elsif direction == :RIGHT
        @left
      elsif direction == :DOWN
        @bottom_flipped
      elsif direction == :LEFT
        @right
      end
    elsif @state == :RIGHT_FLIPPED
      if direction == :UP
        @right_flipped
      elsif direction == :RIGHT
        @top_flipped
      elsif direction == :DOWN
        @left_flipped
      elsif direction == :LEFT
        @bottom_flipped
      end
    elsif @state == :DOWN_FLIPPED
      if direction == :UP
        @bottom
      elsif direction == :RIGHT
        @right_flipped
      elsif direction == :DOWN
        @top
      elsif direction == :LEFT
        @left_flipped
      end
    elsif @state == :LEFT_FLIPPED
      if direction == :UP
        @left
      elsif direction == :RIGHT
        @bottom
      elsif direction == :DOWN
        @right
      elsif direction == :LEFT
        @top
      end
    end
  end

  def grid
    copy = Array.new(@grid.length) { Array.new(@grid[0].length) { 0 } }
    @grid.each_with_index do |row, r|
      row.each_with_index do |v, c|
        copy[r][c] = v
      end
    end

    if @state == :UP
      return copy
    elsif @state == :RIGHT
      return copy.transpose.map(&:reverse)
    elsif @state == :DOWN
      return copy.transpose.map(&:reverse).transpose.map(&:reverse)
    elsif @state == :LEFT
      return copy.transpose.map(&:reverse).transpose.map(&:reverse).transpose.map(&:reverse)
    elsif @state == :UP_FLIPPED
      return copy.map(&:reverse)
    elsif @state == :RIGHT_FLIPPED
      return copy.map(&:reverse).transpose.map(&:reverse)
    elsif @state == :DOWN_FLIPPED
      return copy.map(&:reverse).transpose.map(&:reverse).transpose.map(&:reverse)
    elsif @state == :LEFT_FLIPPED
      return copy.map(&:reverse).transpose.map(&:reverse).transpose.map(&:reverse).transpose.map(&:reverse)
    end

    return nil
  end

  def innards
    aligned_grid = self.grid
    innards = aligned_grid[1..-2]
    (0..innards.length - 1).each do |i|
      innards[i] = innards[i][1..-2]
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
  grid = Array.new(grid_size * 10) { Array.new(grid_size * 10) { 0 }}
  puzzle.each_with_index do |row, r|
    row.each_with_index do |tile, c|
      innards = tile.grid
      innards.each_with_index do |innard_row, r2|
        innard_row.each_with_index do |value, c2|
          actual_row = (r * 10) + r2
          actual_column = (c * 10) + c2
          grid[actual_row][actual_column] = value
        end
      end
    end
  end

  return grid
end

def dump_innards_to_grid(puzzle, grid_size)
  grid = Array.new(grid_size * 8) { Array.new(grid_size * 8) { 0 }}
  puzzle.each_with_index do |row, r|
    row.each_with_index do |tile, c|
      innards = tile.innards
      innards.each_with_index do |innard_row, r2|
        innard_row.each_with_index do |value, c2|
          actual_row = (r * 8) + r2
          actual_column = (c * 8) + c2
          grid[actual_row][actual_column] = value
        end
      end
    end
  end

  return grid
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

tiles = IO.read(options[:filename]).split("\n\n").map{|str| Tile.new(str)}
tile_map = {}
tiles.each do |t|
  tile_map[t.id] = t
end

grid_size = Math.sqrt(tiles.length).to_i

edge_to_tiles = {}
tiles.each do |tile|
  tile.edges.each do |edge|
    if edge_to_tiles.key?(edge)
      edge_to_tiles[edge].push(tile.id)
    else
      edge_to_tiles[edge] = [tile.id]
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

if options[:part] == 1
  puts corners.reduce(&:*)
end

puzzle = Array.new(grid_size) { Array.new(grid_size) { nil }}

puzzle[0][0] = tile_map[corners[0]]
assigned_tile_ids = Set[puzzle[0][0].id]
corner_edges = edge_to_tiles.keys.filter{|e| edge_to_tiles[e].size == 1}
puzzle[0][0].align_top_left_corner(corner_edges)

puzzle.each_with_index do |row, r|
  row.each_with_index do |tile, c|
    if not tile.nil?
      next
    end

    edge_to_match = nil
    direction_to_match = nil
    if r == 0
      tile_to_match = puzzle[r][c - 1]
      edge_to_match = tile_to_match.get_edge(:RIGHT)
      direction_to_match = :LEFT
    else
      tile_to_match = puzzle[r - 1][c]
      edge_to_match = tile_to_match.get_edge(:DOWN)
      direction_to_match = :UP
    end

    new_tile = tiles.find {|t| not assigned_tile_ids.member?(t.id) and t.edges.include?(edge_to_match)}
    puzzle[r][c] = new_tile
    assigned_tile_ids.add(new_tile.id)
    if not new_tile.align_edge(direction_to_match, edge_to_match)
      puts "Error aligning."
    end
  end
end

grid = dump_puzzle_to_grid(puzzle, grid_size)
print_grid(grid)
grid = dump_innards_to_grid(puzzle, grid_size)

shape = [
   '                  # ',
   '#    ##    ##    ###',
   ' #  #  #  #  #  #   '
].map{ |line| line.chars.map{ |c| c == '#' ? 1 : 0}}

possible_grids = [
  grid,
  rotate_grid(grid),
  rotate_grid(rotate_grid(grid)),
  rotate_grid(rotate_grid(rotate_grid(grid))),
  grid.map(&:reverse),
  rotate_grid(grid.map(&:reverse)),
  rotate_grid(rotate_grid(grid.map(&:reverse))),
  rotate_grid(rotate_grid(rotate_grid(grid.map(&:reverse))))
]

possible_grids.each do |g|
  positions = find_shape(g, shape)
  if positions.length == 0
    next
  end

  #puts "The grid..."
  #print_grid(grid)

  sum = 0
  grid.each do |row|
    #puts row.join(",")
    row_count = row.count {|v| v == 1}
    #puts row_count
    sum += row_count
  end

  puts sum

  positions.each do |p|
    mask_shape(grid, shape, p[0], p[1])
  end

  #puts "Thar be monsters!"
  #print_grid(grid)

  sum = 0
  grid.each do |row|
    #puts row.join(",")
    row_count = row.count {|v| v == 1}
    #puts row_count
    sum += row_count
  end

  puts sum
end

