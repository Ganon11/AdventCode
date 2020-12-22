#!/usr/bin/ruby

require 'optparse'
require 'set'

class Tile
  attr_reader :id
  attr_reader :cols
  attr_reader :edges

  def initialize(tile_string)
    lines = tile_string.split("\n")
    data = lines[0].match(/Tile (?<tile_id>\d+):/)
    if data
      @id = data[:tile_id].to_i
    else
      puts "Bad parse"
    end

    @grid = []
    top = []
    bottom = []
    left = []
    right = []
    lines.drop(1).each_with_index do |row, r|
      row.strip!
      gridrow = []
      row.chars.each_with_index do |ch, c|
        value = ch == '#' ? 1 : 0
        gridrow.push(value)

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
      @grid.push(gridrow)
    end

    @edges = [top, bottom, left, right, top.reverse, bottom.reverse, left.reverse, right.reverse]
  end
end

def print_puzzle_ids(puzzle)
  puzzle.each do |row|
    puts row.map{|tile| tile.nil? ? "?" : tile.id}.join(", ")
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

print_puzzle_ids(puzzle)
