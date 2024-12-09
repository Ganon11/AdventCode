require 'point'
require 'optparse'
require 'set'
require 'sorbet-runtime'

# typed: true
extend T::Sig

class Tile
  OBSTACLE = '#'
  VISITED = 'X'
end

class Path
  extend T::Sig

  sig {returns(Point::Point)}
  attr_reader :position

  sig {returns(Symbol)}
  attr_reader :direction

  sig {params(position: Point::Point, direction: Symbol).void}
  def initialize(position, direction)
    @position = position
    @direction = direction
  end

  sig {params(other: Path).returns(T::Boolean)}
  def eql?(other)
    @position.eql?(other.position) && @direction == other.direction
  end

  sig {returns(Integer)}
  def hash
    [@position, @direction].hash
  end

  sig {returns(String)}
  def to_s
    "#{@position} facing #{@direction}"
  end
end

class Guard
  extend T::Sig

  sig {returns(Path)}
  attr_accessor :current

  sig {returns(T::Set[Path])}
  attr_reader :path

  sig {params(initial_position: Path).void}
  def initialize(initial_position)
    @current = initial_position
    @path = T.let(Set.new([@current]), T::Set[Path])
  end

  sig {void}
  def rotate
    if @current.direction == :north
      @current = Path.new(@current.position, :east)
    elsif @current.direction == :east
      @current = Path.new(@current.position, :south)
    elsif @current.direction == :west
      @current = Path.new(@current.position, :north)
    elsif @current.direction == :south
      @current = Path.new(@current.position, :west)
    end

    @path << @current
  end

  class StepResult
    WALK = 1
    ROTATE = 2
    CYCLE_DETECTED = 3
    OUT_OF_BOUNDS = 4
  end

  sig {params(map: T::Hash[Point::Point, String], max_row: Integer, max_col: Integer).returns(Integer)}
  def step(map, max_row, max_col)
    p = T.let(nil, T.nilable(Point::Point))
    if @current.direction == :north
      p = @current.position.north
    elsif @current.direction == :east
      p = @current.position.east
    elsif @current.direction == :west
      p = @current.position.west
    elsif @current.direction == :south
      p = @current.position.south
    end

    if !in_bounds?(map, T.must(p), max_row, max_col)
      return StepResult::OUT_OF_BOUNDS
    elsif map[T.must(p)] == Tile::OBSTACLE
      if @current.direction == :north
        path_entry = Path.new(@current.position, :east)
      elsif @current.direction == :east
        path_entry = Path.new(@current.position, :south)
      elsif @current.direction == :west
        path_entry = Path.new(@current.position, :north)
      elsif @current.direction == :south
        path_entry = Path.new(@current.position, :west)
      end

      if @path.include?(T.must(path_entry))
        return StepResult::CYCLE_DETECTED
      end
      @path << T.must(path_entry)
      @current = T.must(path_entry)
      return StepResult::ROTATE
    else
      path_entry = Path.new(T.must(p), @current.direction)
      if @path.include?(path_entry)
        return StepResult::CYCLE_DETECTED
      end

      path << path_entry
      @current = path_entry
      return StepResult::WALK
    end
  end

  sig {returns(Integer)}
  def count_unique_locations
    @path.map{ |p| p.position }.uniq.count
  end
end

sig {params(map: T::Hash[Point::Point, String]).returns(Integer)}
def find_max_row(map)
  T.must(map.keys.map{ |p| p.y }.max)
end

sig {params(map: T::Hash[Point::Point, String]).returns(Integer)}
def find_max_col(map)
  T.must(map.keys.map{ |p| p.x }.max)
end

sig {params(map: T::Hash[Point::Point, String], position: Point::Point, max_row: Integer, max_col: Integer).returns(T::Boolean)}
def in_bounds?(map, position, max_row, max_col)
  0 <= position.x && position.x <= max_col && 0 <= position.y && position.y <= max_row
end

sig {params(map: T::Hash[Point::Point, String], guard: Guard).returns(T::Boolean)}
def do_walk(map, guard)
  max_row = find_max_row(map)
  max_col = find_max_col(map)

  while true do
    step_result = guard.step(map, max_row, max_col)
    if step_result == Guard::StepResult::CYCLE_DETECTED
      return true
    elsif step_result == Guard::StepResult::OUT_OF_BOUNDS
      return false
    end
  end
end

sig {params(map: T::Hash[Point::Point, String], path: T::Set[Path], initial: Path).returns(Integer)}
def count_cycles(map, path, initial)
  sum = 0
  path.map{ |p| p.position }.uniq.each do |p|
    map[p] = Tile::OBSTACLE
    sum += 1 if do_walk(map, Guard.new(initial))
    map.delete(p)
  end

  return sum
end

options = {
  :filename => 'input.txt'
}

OptionParser.new do |opts|
  opts.banner = 'Usage: d6.rb [options]'
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
      guard = Guard.new(Path.new(p, :north))
      map[p] = Tile::VISITED
    end
  end
end

initial = T.must(guard).current
do_walk(map, T.must(guard))
visited_count = T.must(guard).count_unique_locations
puts "Visited: #{visited_count}"
puts "#{count_cycles(map, T.must(guard).path, initial)} cycles could be created"
