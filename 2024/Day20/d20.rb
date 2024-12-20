require 'bounds'
require 'direction'
require 'point'
require 'optparse'
require 'sorbet-runtime'

# typed: true
extend T::Sig

class Cheat
  extend T::Sig

  sig {returns(Point::Point)}
  attr_reader :start

  sig {returns(Point::Point)}
  attr_reader :intermediate

  sig {returns(Point::Point)}
  attr_reader :destination

  sig {params(start: Point::Point, intermediate: Point::Point, destination: Point::Point).void}
  def initialize(start, intermediate, destination)
    @start = start
    @intermediate = intermediate
    @destination = destination
  end

  sig {returns(String)}
  def to_s
    "From #{@start}, through #{@intermediate}, to #{@destination}"
  end
end

class ShortestPathResult
  extend T::Sig

  sig {returns(T::Array[Point::Point])}
  attr_accessor :path

  sig {returns(T::Array[Cheat])}
  attr_accessor :possible_cheats

  sig {void}
  def initialize
    @path = Array.new
    @possible_cheats = Array.new
  end
end

sig {params(maze: T::Set[Point::Point], start: Point::Point, destination: Point::Point).returns(ShortestPathResult)}
def shortest_path(maze, start, destination)
  frontier = Queue.new
  frontier << start
  came_from = T.let(Hash.new, T::Hash[Point::Point, T.nilable(Point::Point)])
  came_from[start] = nil
  bounds = Point::Bounds.new(maze)

  result = ShortestPathResult.new

  while !frontier.empty?
    current = T.cast(frontier.pop, Point::Point)
    break if current.eql?(destination)

    # Check for possible cheats north, south, east, and west
    if maze.include?(current.north) && !maze.include?(current.north.north) && !came_from.include?(current.north.north) && bounds.in_bounds?(current.north.north)
      result.possible_cheats << Cheat.new(current, current.north, current.north.north)
    end
    if maze.include?(current.south) && !maze.include?(current.south.south) && !came_from.include?(current.south.south) && bounds.in_bounds?(current.south.south)
      result.possible_cheats << Cheat.new(current, current.south, current.south.south)
    end
    if maze.include?(current.east) && !maze.include?(current.east.east) && !came_from.include?(current.east.east) && bounds.in_bounds?(current.east.east)
      result.possible_cheats << Cheat.new(current, current.east, current.east.east)
    end
    if maze.include?(current.west) && !maze.include?(current.west.west) && !came_from.include?(current.west.west) && bounds.in_bounds?(current.west.west)
      result.possible_cheats << Cheat.new(current, current.west, current.west.west)
    end

    for neighbor in current.neighbors
      next if maze.include?(neighbor)
      next if came_from.include?(neighbor)
      frontier << neighbor
      came_from[neighbor] = current
    end
  end

  walkback = destination
  while !walkback.eql?(start)
    result.path << walkback
    walkback = T.must(came_from[walkback])
  end
  result.path << walkback
  result.path.reverse!

  return result
end

sig {params(path: T::Hash[Point::Point, Integer], cheat: Cheat).returns(Integer)}
def shortest_path_with_cheat(path, cheat)
  #puts "Using cheat #{cheat}"
  #puts "  Path: #{path.join(', ')}"
  start_index = T.must(path[cheat.start])
  destination_index = T.must(path[cheat.destination])

  return start_index + (path.length - destination_index) + 2
end

options = {
  :filename => 'input.txt'
}

OptionParser.new do |opts|
  opts.banner = 'Usage: d20.rb [options]'
  opts.on('-f FILENAME', '--filename=FILENAME', String, 'Which file to read as input?')
end.parse!(into: options)

maze = T.let(Set.new, T::Set[Point::Point])
start = T.let(nil, T.nilable(Point::Point))
destination = T.let(nil, T.nilable(Point::Point))
IO.readlines(options[:filename]).each_with_index do |line, row|
  line.strip.chars.each_with_index do |char, col|
    p = Point::Point.new(col, row)
    case char
    when '#'
      maze << p
    when 'S'
      start = p
    when 'E'
      destination = p
    end
  end
end

result = shortest_path(maze, T.must(start), T.must(destination))
puts "Normal shortest: #{result.path.length - 1}"
puts "Possible cheats: #{result.possible_cheats.length}"
path_to_index = T.let(Hash.new, T::Hash[Point::Point, Integer])
result.path.each_with_index do |p, i|
  path_to_index[p] = i
end
lengths = result.possible_cheats.map { |c| result.path.length - shortest_path_with_cheat(path_to_index, c) }
puts "Under 50: #{lengths.filter{ |l| l >= 50 }.length}"
puts "Under 100: #{lengths.filter{ |l| l >= 100 }.length}"
