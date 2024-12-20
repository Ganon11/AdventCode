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
  attr_reader :destination

  sig {params(start: Point::Point, destination: Point::Point).void}
  def initialize(start, destination)
    @start = start
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

sig {params(maze: T::Set[Point::Point], start: Point::Point, destination: Point::Point, cheat_length: Integer).returns(ShortestPathResult)}
def shortest_path(maze, start, destination, cheat_length)
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
    (2..cheat_length).each do |j_dist|
      for possible_cheat_destination in current.neighbors_at_distance(j_dist)
        result.possible_cheats << Cheat.new(current, possible_cheat_destination) if !maze.include?(possible_cheat_destination) && !came_from.include?(possible_cheat_destination) && bounds.in_bounds?(possible_cheat_destination)
      end
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
  start_index = T.must(path[cheat.start])
  destination_index = T.must(path[cheat.destination])

  return start_index + (path.length - destination_index) + cheat.start.distance(cheat.destination)
end

options = {
  :filename => 'input.txt',
  :cheat => 2
}

OptionParser.new do |opts|
  opts.banner = 'Usage: d20.rb [options]'
  opts.on('-c CHEAT', '--cheat=CHEAT', Integer, 'How many nanoseconds of cheating?')
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

start = T.must(start)
destination = T.must(destination)

result = shortest_path(maze, start, destination, options[:cheat])
path_to_index = T.let(Hash.new, T::Hash[Point::Point, Integer])
result.path.each_with_index do |p, i|
  path_to_index[p] = i
end
lengths = result.possible_cheats.map { |c| result.path.length - shortest_path_with_cheat(path_to_index, c) }

threshhold = options[:filename].eql?("input.txt") ? 100 : 50
puts "Under #{threshhold}: #{lengths.filter{ |l| l >= threshhold }.length}"
