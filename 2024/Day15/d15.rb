require 'direction'
require 'point'
require 'optparse'
require 'sorbet-runtime'

# typed: true
extend T::Sig

class TileType < T::Enum
  enums do
    Empty = new
    Wall = new
    Box = new
    Robot = new
    BoxL = new
    BoxR = new
  end
end

STRING_TO_TILE_MAP = {
  '.' => TileType::Empty,
  '#' => TileType::Wall,
  'O' => TileType::Box,
  '@' => TileType::Robot,
  '[' => TileType::BoxL,
  ']' => TileType::BoxR
}

STRING_TO_DIRECTION_MAP = {
  '<' => Point::Direction::West,
  '>' => Point::Direction::East,
  'v' => Point::Direction::South,
  '^' => Point::Direction::North
}

sig {params(maze: T::Hash[Point::Point, TileType], directions: T::Array[Point::Direction]).void}
def operate(maze, directions)
  robot = T.must(maze.select { |_, t| t == TileType::Robot }.keys[0])
  puts "Robot at #{robot}"
  directions.each do |d|
    case d
    when Point::Direction::North
      free = robot.north
      while maze[free] == TileType::Box
        free = free.north
      end
      next if maze[free] == TileType::Wall
      maze[free] = TileType::Box
      maze[robot] = TileType::Empty
      robot = robot.north
      maze[robot] = TileType::Robot

    when Point::Direction::South
      free = robot.south
      while maze[free] == TileType::Box
        free = free.south
      end
      next if maze[free] == TileType::Wall
      maze[free] = TileType::Box
      maze[robot] = TileType::Empty
      robot = robot.south
      maze[robot] = TileType::Robot

    when Point::Direction::East
      free = robot.east
      while maze[free] == TileType::Box
        free = free.east
      end
      next if maze[free] == TileType::Wall
      maze[free] = TileType::Box
      maze[robot] = TileType::Empty
      robot = robot.east
      maze[robot] = TileType::Robot

    when Point::Direction::West
      free = robot.west
      while maze[free] == TileType::Box
        free = free.west
      end
      next if maze[free] == TileType::Wall
      maze[free] = TileType::Box
      maze[robot] = TileType::Empty
      robot = robot.west
      maze[robot] = TileType::Robot

    else
      T.absurd(d)
    end
  end
end

sig {params(maze: T::Hash[Point::Point, TileType]).returns(Integer)}
def calc_gps(maze)
  maze.select{ |position, t| t == TileType::Box }.keys.map{ |p| 100 * p.y + p.x }.sum
end

class BigBox
  extend T::Sig

  sig {returns(Point::Point)}
  attr_reader :left

  sig {returns(Point::Point)}
  attr_reader :right

  sig {params(point: Point::Point).void}
  def initialize(point)
    @left = point
    @right = point.east
  end

  sig {returns(Integer)}
  def gps
    100 * @left.y + @left.x
  end
end

options = {
  :part => 1,
  :filename => 'input.txt'
}

OptionParser.new do |opts|
  opts.banner = 'Usage: d15.rb [options]'
  opts.on('-p PART', '--part=PART', Integer, 'Which part?')
  opts.on('-f FILENAME', '--filename=FILENAME', String, 'Which file to use?')
end.parse!(into: options)

parsing_maze = T.let(true, T::Boolean)
if options[:part] == 1
  maze = T.let(Hash.new, T::Hash[Point::Point, TileType])
  directions = T.let(Array.new, T::Array[Point::Direction])
  IO.readlines(options[:filename]).each_with_index do |line, row|
    line.strip!
    if line.empty?
      parsing_maze = false
      next
    end

    if parsing_maze
      line.chars.each_with_index do |char, col|
        p = Point::Point.new(col, row)
        maze[p] = STRING_TO_TILE_MAP[char]
      end
    else
      line.chars.each do |char|
        directions << STRING_TO_DIRECTION_MAP[char]
      end
    end
  end

  operate(maze, directions)
  puts "Current GPS: #{calc_gps(maze)}"
elsif options[:part] == 2
else
  raise Exception.new "Stop mucking around! #{options[:part]}"
end