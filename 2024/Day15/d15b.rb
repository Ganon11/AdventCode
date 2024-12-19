require 'bounds'
require 'direction'
require 'optparse'
require 'point'
require 'sorbet-runtime'

# typed: true
extend T::Sig

class TileType < T::Enum
  enums do
    Empty = new
    Wall = new
    Robot = new
    BoxL = new
    BoxR = new
  end
end

TILE_TO_STRING_MAP = {
  TileType::Empty => '.',
  TileType::Wall => '#',
  TileType::Robot => '@',
  TileType::BoxL => '[',
  TileType::BoxR => ']'
}

STRING_TO_DIRECTION_MAP = {
  '<' => Point::Direction::West,
  '>' => Point::Direction::East,
  'v' => Point::Direction::South,
  '^' => Point::Direction::North
}

sig {params(maze: T::Hash[Point::Point, TileType]).void}
def print(maze)
  min_x = Point::Point.min_x(maze.keys)
  max_x = Point::Point.max_x(maze.keys)
  min_y = Point::Point.min_y(maze.keys)
  max_y = Point::Point.max_y(maze.keys)

  str = ""
  (min_y..max_y).each do |y|
    (min_x..max_x).each do |x|
      p = Point::Point.new(x, y)
      str += TILE_TO_STRING_MAP[maze[p]]
    end
    str += "\n"
  end

  puts str
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

  sig {params(direction: Point::Direction).void}
  def move(direction)
    case direction
    when Point::Direction::North
      @left = @left.north
      @right = @right.north
    when Point::Direction::South
      @left = @left.south
      @right = @right.south
    when Point::Direction::East
      @left = @left.east
      @right = @right.east
    when Point::Direction::West
      @left = @left.west
      @right = @right.west
    end
  end

  sig {params(bounds: Point::Bounds).returns(Integer)}
  def gps(bounds)
    x = [bounds.max_x - @right.x, @left.x - bounds.min_x].min
    y = [bounds.max_y - @right.y, @left.y - bounds.min_y].min
    100 * y + x
  end

  sig {params(other: BigBox).returns(T::Boolean)}
  def eql?(other)
    @left.eql?(other.left) && @y.eql?(other.y)
  end

  sig {returns(Integer)}
  def hash
    [@left, @right].hash
  end
end

sig {params(boxes: T::Array[BigBox], bounds: Point::Bounds).returns(Integer)}
def calc_gps(boxes, bounds)
  boxes.sum(0) { |b| b.gps(bounds) }
end

sig {params(maze: T::Hash[Point::Point, TileType], robot: Point::Point, boxes: T::Array[BigBox], direction: Point::Direction).returns(T::Boolean)}
def can_move_north_or_south(maze, robot, boxes, direction)
  raise Exception.new "Bad direction: #{direction}" if direction != Point::Direction::North && direction != Point::Direction::South
  points_to_check = Queue.new
  if direction == Point::Direction::North
    points_to_check << robot.north
  else
    points_to_check << robot.south
  end
  points_checked = Set.new
  while !points_to_check.empty?
    point_to_check = points_to_check.pop
    next if points_checked.include?(point_to_check)

    points_checked << point_to_check
    return false if maze[point_to_check] == TileType::Wall
    next if maze[point_to_check] == TileType::Empty

    box = T.must(boxes.select { |b| b.left.eql?(point_to_check) || b.right.eql?(point_to_check) }[0])

    if direction == Point::Direction::North
      points_to_check << box.left.north
      points_to_check << box.right.north
    else
      points_to_check << box.left.south
      points_to_check << box.right.south
    end
  end

  return true
end

sig {params(maze: T::Hash[Point::Point, TileType], robot: Point::Point, boxes: T::Array[BigBox], direction: Point::Direction).returns(T::Enumerable[BigBox])}
def boxes_to_move(maze, robot, boxes, direction)
  raise Exception.new "Bad direction: #{direction}" if direction != Point::Direction::North && direction != Point::Direction::South
  boxes = Set.new
  points_to_check = Queue.new
  if direction == Point::Direction::North
    points_to_check << robot.north
  else
    points_to_check << robot.south
  end
  points_checked = Set.new
  while !points_to_check.empty?
    point_to_check = points_to_check.pop
    next if points_checked.include?(point_to_check)
    puts "Checking #{point_to_check}"

    points_checked << point_to_check
    next if maze[point_to_check] == TileType::Empty
    if maze[point_to_check] == TileType::BoxL
      box = boxes.select { |b| b.left.eql?(point_to_check) }[0]
    elsif maze[point_to_check] == TileType::BoxR
      box = boxes.select { |b| b.right.eql?(point_to_check) }[0]
    end

    next if box.nil?
    boxes << box

    if direction == Point::Direction::North
      points_to_check << box.left.north
      points_to_check << box.right.north
    else
      points_to_check << box.left.south
      points_to_check << box.right.south
    end
  end

  return boxes
end

sig {params(maze: T::Hash[Point::Point, TileType], directions: T::Array[Point::Direction], boxes: T::Array[BigBox]).void}
def operate(maze, directions, boxes)
  robot = T.must(maze.select { |_, t| t == TileType::Robot }.keys[0])
  puts "Robot at #{robot}"
  directions.each do |d|
    case d
    when Point::Direction::North
      next if !can_move_north_or_south(maze, robot, boxes, Point::Direction::North)
      moving_boxes = boxes_to_move(maze, robot, boxes, Point::Direction::North)
      moving_boxes.each do |b|
        maze[b.left] = TileType::Empty
        maze[b.right] = TileType::Empty
      end
      moving_boxes.each do |b|
        b.move(Point::Direction::North)
      end
      moving_boxes.each do |b|
        maze[b.left] = TileType::BoxL
        maze[b.right] = TileType::BoxR
      end
      maze[robot] = TileType::Empty
      robot = robot.north
      maze[robot] = TileType::Robot

    when Point::Direction::South
      next if !can_move_north_or_south(maze, robot, boxes, Point::Direction::South)
      moving_boxes = boxes_to_move(maze, robot, boxes, Point::Direction::South)
      moving_boxes.each do |b|
        maze[b.left] = TileType::Empty
        maze[b.right] = TileType::Empty
      end
      moving_boxes.each do |b|
        b.move(Point::Direction::South)
      end
      moving_boxes.each do |b|
        maze[b.left] = TileType::BoxL
        maze[b.right] = TileType::BoxR
      end
      maze[robot] = TileType::Empty
      robot = robot.south
      maze[robot] = TileType::Robot

    when Point::Direction::East
      free = robot.east
      while maze[free] == TileType::BoxL || maze[free] == TileType::BoxR
        free = free.east
      end
      next if maze[free] == TileType::Wall
      # Walk backwards, moving boxes as we find their left edges
      free = free.west
      while maze[free] != TileType::Robot
        if maze[free] == TileType::BoxL
          box = T.must(boxes.select{ |b| b.left.eql?(free) }[0])
          maze[box.left] = TileType::Empty
          maze[box.right] = TileType::Empty
          box.move(Point::Direction::East)
          maze[box.left] = TileType::BoxL
          maze[box.right] = TileType::BoxR
        end

        free = free.west
      end
      maze[robot] = TileType::Empty
      robot = robot.east
      maze[robot] = TileType::Robot

    when Point::Direction::West
      free = robot.west
      while maze[free] == TileType::BoxL || maze[free] == TileType::BoxR
        free = free.west
      end
      next if maze[free] == TileType::Wall
      # Walk backwards, moving boxes as we find their right edges
      free = free.east
      while maze[free] != TileType::Robot
        if maze[free] == TileType::BoxR
          box = T.must(boxes.select{ |b| b.right.eql?(free) }[0])
          maze[box.left] = TileType::Empty
          maze[box.right] = TileType::Empty
          box.move(Point::Direction::West)
          maze[box.left] = TileType::BoxL
          maze[box.right] = TileType::BoxR
        end

        free = free.east
      end
      maze[robot] = TileType::Empty
      robot = robot.west
      maze[robot] = TileType::Robot

    else
      T.absurd(d)
    end
  end
end

options = {
  :filename => 'input.txt'
}

OptionParser.new do |opts|
  opts.banner = 'Usage: d15b.rb [options]'
  opts.on('-f FILENAME', '--filename=FILENAME', String, 'Which file to use?')
end.parse!(into: options)

parsing_maze = T.let(true, T::Boolean)
maze = T.let(Hash.new, T::Hash[Point::Point, TileType])
directions = T.let(Array.new, T::Array[Point::Direction])
boxes = T.let(Array.new, T::Array[BigBox])
IO.readlines(options[:filename]).each_with_index do |line, row|
  line.strip!
  if line.empty?
    parsing_maze = false
    next
  end

  if parsing_maze
    line.chars.each_with_index do |char, col|
      p = Point::Point.new(col * 2, row)
      p2 = p.east
      case char
      when '#'
        maze[p] = TileType::Wall
        maze[p2] = TileType::Wall
      when 'O'
        maze[p] = TileType::BoxL
        maze[p2] = TileType::BoxR
        boxes << BigBox.new(p)
      when '.'
        maze[p] = TileType::Empty
        maze[p2] = TileType::Empty
      when '@'
        maze[p] = TileType::Robot
        maze[p2] = TileType::Empty
      else
        T.absurd(char)
      end
    end
  else
    line.chars.each do |char|
      directions << STRING_TO_DIRECTION_MAP[char]
    end
  end
end

print(maze)
operate(maze, directions, boxes)
print(maze)
puts "Current GPS: #{calc_gps(boxes, Point::Bounds.new(maze.keys))}"
