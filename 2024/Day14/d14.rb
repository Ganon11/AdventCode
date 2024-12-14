require 'point'
require 'optparse'
require 'sorbet-runtime'

# typed: true
extend T::Sig

class Quadrant < T::Enum
  enums do
    None = new
    Northwest = new
    Northeast = new
    Southwest = new
    Southeast = new
  end
end

class Robot
  extend T::Sig

  ROBOT_PATTERN = /^p=(-?\d+),(-?\d+) v=(-?\d+),(-?\d+)$/

  sig {returns(Point::Point)}
  attr_reader :position

  sig {returns(Point::Point)}
  attr_reader :velocity

  sig {params(line: String).void}
  def initialize(line)
    m = ROBOT_PATTERN.match(line)
    if !m
      raise Exception.new "Invalid input: #{line}"
    end

    @position = Point::Point.new(m[1].to_i, m[2].to_i)
    @velocity = Point::Point.new(m[3].to_i, m[4].to_i)
  end

  sig {params(seconds: Integer, rows: Integer, cols: Integer).void}
  def walk(seconds, rows, cols)
    new_x = (@position.x + (@velocity.x * seconds)) % cols
    new_y = (@position.y + (@velocity.y * seconds)) % rows
    @position = Point::Point.new(new_x, new_y)
  end

  sig {params(rows: Integer, cols: Integer).returns(Quadrant)}
  def quadrant(rows, cols)
    middle_row = (rows - 1) / 2
    middle_col = (cols - 1) / 2
    return Quadrant::None if @position.x == middle_col
    return Quadrant::None if @position.y == middle_row

    is_west = @position.x < middle_col
    is_north = @position.y < middle_row

    return Quadrant::Northwest if is_west && is_north
    return Quadrant::Northeast if !is_west && is_north
    return Quadrant::Southwest if is_west && !is_north
    return Quadrant::Southeast
  end
end

sig {params(robots: T::Enumerable[Robot], rows: Integer, cols: Integer).returns(T::Boolean)}
def find_easter_egg(robots, rows, cols)
  robopositions = T.let(Set.new(robots.map(&:position)), T::Set[Point::Point])
  str = ""
  (0..rows - 1).each do |row|
    (0..cols - 1).each do |col|
      p = Point::Point.new(col, row)
      if robopositions.include?(p)
        str << "#"
      else
        str << "."
      end
    end

    str << "\n"
  end
  
  if str.include?("############")
    puts str
    return true
  end

  return false
end

options = {
  :part => 1,
  :filename => 'input.txt'
}

OptionParser.new do |opts|
  opts.banner = 'Usage: d14.rb [options]'
  opts.on('-p PART', '--part=PART', Integer, 'Which part?')
  opts.on('-f FILENAME', '--filename=FILENAME', String, 'Which file to use?')
end.parse!(into: options)

robots = T.let(Array.new, T::Array[Robot])
IO.readlines(options[:filename]).each do |line|
  robots << Robot.new(line.strip)
end

puts "Parsed #{robots.length} robots"
puts "The first is at #{T.must(robots[0]).position} and has velocity #{T.must(robots[0]).velocity}"

rows = 0
cols = 0
if options[:filename] == "sample.txt"
  rows = 7
  cols = 11
else
  rows = 103
  cols = 101
end

if options[:part] == 1
  robots.each { |r| r.walk(100, rows, cols) }

  puts robots.group_by { |robot| robot.quadrant(rows, cols) }
    .select { |q, _| q != Quadrant::None }
    .map { |_, r| r.length}
    .reduce(:*)
elsif options[:part] == 2
  (1..10000).each do |step|
    robots.each { |r| r.walk(1, rows, cols) }
    result = find_easter_egg(robots, rows, cols)
    if result
      puts "Step #{step}"
      break
    end
  end
end
