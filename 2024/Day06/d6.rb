require 'point'
require 'optparse'
require 'sorbet-runtime'

# typed: true
extend T::Sig

class Tile
  OBSTACLE = '#'
  VISITED = 'X'
end

class Guard
  extend T::Sig

  sig {returns(Point::Point)}
  attr_accessor :position
  
  sig {returns(Symbol)}
  attr_accessor :direction

  sig {params(position: Point::Point, direction: Symbol).void}
  def initialize(position, direction)
    @position = position
    @direction = direction
  end

  sig {void}
  def rotate
    if @direction == :north
      @direction = :east
    elsif @direction == :east
      @direction = :south
    elsif @direction == :west
      @direction = :north
    elsif @direction == :south
      @direction = :west
    end
  end
end

sig {params(map: T::Hash[Point::Point, String]).returns(Integer)}
def find_max_row(map)
  max_row_point = map.keys.max_by{ |p| p.y }
  return max_row_point.y if !max_row_point.nil?
  -1
end

sig {params(map: T::Hash[Point::Point, String]).returns(Integer)}
def find_max_col(map)
  max_col_point = map.keys.max_by{ |p| p.x }
  return max_col_point.x if !max_col_point.nil?
  -1
end

sig {params(map: T::Hash[Point::Point, String], position: Point::Point, max_row: Integer, max_col: Integer).returns(T::Boolean)}
def in_bounds?(map, position, max_row, max_col)
  0 <= position.x && position.x <= max_col && 0 <= position.y && position.y <= max_row
end

sig {params(map: T::Hash[Point::Point, String], guard: Guard).void}
def do_walk(map, guard)
  max_row = find_max_row(map)
  max_col = find_max_col(map)

  #puts "Max Row: #{max_row}, Max Col: #{max_col}"
  while in_bounds?(map, guard.position, max_row, max_col) do
    p = T.let(nil, T.nilable(Point::Point))
    if guard.direction == :north
      p = guard.position.north
    elsif guard.direction == :east
      p = guard.position.east
    elsif guard.direction == :west
      p = guard.position.west
    elsif guard.direction == :south
      p = guard.position.south
    end

    #puts "Checking #{T.must(p)}..."
    if !in_bounds?(map, T.must(p), max_row, max_col)
      #puts "  Guard will step out of bounds!"
      break
    elsif map[T.must(p)] == Tile::OBSTACLE
      #puts "  Obstacle! We must turn"
      guard.rotate
    else
      #puts "  Open! Take a step"
      guard.position = T.must(p)
      map[T.must(p)] = Tile::VISITED
    end
  end
end

options = {
  :part => 1,
  :filename => 'input.txt'
}

OptionParser.new do |opts|
  opts.banner = 'Usage: d6.rb [options]'
  opts.on('-p PART', '--part=PART', Integer, 'Which part?')
  opts.on('-f FILENAME', '--filename=FILENAME', String, 'Which file to use?')
end.parse!(into: options)

guard = T.let(nil, T.nilable(Guard))
map = T.let({}, T::Hash[Point::Point, String])
IO.readlines(options[:filename]).map(&:strip).each_with_index do |line, row|
  line.each_char.with_index do |character, col|
    p = Point::Point.new(col, row)
    if character == Tile::OBSTACLE
      map[p] = Tile::OBSTACLE
    elsif character == '^'
      guard = Guard.new(p, :north)
      map[p] = Tile::VISITED
    end
  end
end

#puts "Parsed: guard is at #{T.must(guard).position} and is facing #{T.must(guard).direction}"

do_walk(map, T.must(guard))
visited_count = map.count{ |_, t| t == Tile::VISITED }
puts "Visited: #{visited_count}"
