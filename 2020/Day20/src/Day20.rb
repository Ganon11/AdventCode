#!/usr/bin/ruby

require 'optparse'

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

def part1(tiles)
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

  return tile_unique_edge_count.select{|tile, count| count == 4}.keys.reduce(&:*)
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

if options[:part] == 1
  puts part1(tiles)
elsif options[:part] == 2
  # do part 2
end
