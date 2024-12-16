require 'direction'
require 'fc'
require 'point'
require 'optparse'
require 'sorbet-runtime'

# typed: true
extend T::Sig

class Reindeer
  extend T::Sig

  sig {returns(Point::Point)}
  attr_accessor :position

  sig {returns(Point::Direction)}
  attr_accessor :direction

  sig {params(position: Point::Point, direction: Point::Direction).void}
  def initialize(position, direction)
    @position = position
    @direction = direction
  end

  sig {params(other: Reindeer).returns(T::Boolean)}
  def eql?(other)
    self.position.eql?(other.position) && self.direction.eql?(other.direction)
  end

  sig {returns(Integer)}
  def hash
    [@position, @direction].hash
  end

  sig {returns(String)}
  def to_s
    "#{@position}, #{@direction}"
  end
end

sig {params(start: Point::Point, destination: Point::Point, maze: T::Set[Point::Point]).returns(T::Array[Integer])}
def find_path(start, destination, maze)
  frontier = FastContainers::PriorityQueue.new(:min)
  starting_state = Reindeer.new(start, Point::Direction::East)
  frontier.push(starting_state, 0)
  came_from = T.let(Hash.new, T::Hash[Reindeer, T::Set[Reindeer]])
  cost_so_far = T.let(Hash.new, T::Hash[Reindeer, Integer])
  came_from[starting_state] = Set.new
  cost_so_far[starting_state] = 0

  while !frontier.empty?
    current = T.cast(frontier.pop, Reindeer)

    # if current.position.eql?(destination)
    #   break
    # end
    # 
    #puts "Considering #{current}"

    neighbors = T.let(Array.new, T::Array[Reindeer])
    case current.direction
    when Point::Direction::East
      neighbors << Reindeer.new(current.position.east, Point::Direction::East) if !maze.include?(current.position.east)
      neighbors << Reindeer.new(current.position, Point::Direction::North) if !maze.include?(current.position.north)
      neighbors << Reindeer.new(current.position, Point::Direction::South) if !maze.include?(current.position.south)
    when Point::Direction::West
      neighbors << Reindeer.new(current.position.west, Point::Direction::West) if !maze.include?(current.position.west)
      neighbors << Reindeer.new(current.position, Point::Direction::North) if !maze.include?(current.position.north)
      neighbors << Reindeer.new(current.position, Point::Direction::South) if !maze.include?(current.position.south)
    when Point::Direction::North
      neighbors << Reindeer.new(current.position.north, Point::Direction::North)if !maze.include?(current.position.north)
      neighbors << Reindeer.new(current.position, Point::Direction::East) if !maze.include?(current.position.east)
      neighbors << Reindeer.new(current.position, Point::Direction::West) if !maze.include?(current.position.west)
    when Point::Direction::South
      neighbors << Reindeer.new(current.position.south, Point::Direction::South) if !maze.include?(current.position.south)
      neighbors << Reindeer.new(current.position, Point::Direction::East) if !maze.include?(current.position.east)
      neighbors << Reindeer.new(current.position, Point::Direction::West) if !maze.include?(current.position.west)
    else
      T.absurd(current)
    end

    #puts "  #{neighbors.length} possible neighbors"

    for neighbor in neighbors
      new_cost = T.cast(cost_so_far[current], Integer)
      if neighbor.direction.eql?(current.direction)
        new_cost += 1
      elsif !neighbor.direction.eql?(current.direction)
        new_cost += 1000
      end

      #puts "  Cost to get to #{neighbor}: #{new_cost}"
      if cost_so_far.include?(neighbor)
        old_cost = T.must(cost_so_far[neighbor])
        next if new_cost > old_cost

        if new_cost == old_cost
          #puts "  Equal cost: adding new came_from"
          T.must(came_from[neighbor]) << current
        else
          #puts "  Lower cost: recalculate..."
          cost_so_far[neighbor] = new_cost
          priority = new_cost + neighbor.position.distance(destination)
          frontier.push(neighbor, priority)
          came_from[neighbor] = Set.new([current])
        end
      else
        #puts "  New location: calculate..."
        cost_so_far[neighbor] = new_cost
        priority = new_cost + neighbor.position.distance(destination)
        frontier.push(neighbor, priority)
        came_from[neighbor] = Set.new([current])
      end
    end
  end

  # Reverse BFS to find all shortest paths
  shortest_path_points = T.let(Set.new, T::Set[Point::Point])
  reverse_frontier = Queue.new(came_from.keys.select{ |r| r.position.eql?(destination) })
  while !reverse_frontier.empty?
    current = T.cast(reverse_frontier.pop, Reindeer)
    shortest_path_points << current.position
    for neighbor in T.must(came_from[current])
      reverse_frontier.push(neighbor)
    end
  end

  shortest_cost = T.must(cost_so_far.find{ |r, _| r.position.eql?(destination) })[1]
  path_count = shortest_path_points.length

  return [shortest_cost, path_count]
end

options = {
  :filename => 'input.txt'
}

OptionParser.new do |opts|
  opts.banner = 'Usage: d16.rb [options]'
  opts.on('-f FILENAME', '--filename=FILENAME', String, 'Which file to use?')
end.parse!(into: options)

maze = T.let(Set.new, T::Set[Point::Point])
starting_position = T.let(nil, T.nilable(Point::Point))
destination = T.let(nil, T.nilable(Point::Point))
IO.readlines(options[:filename]).each_with_index do |line, row|
  line.strip.chars.each_with_index do |char, col|
    p = Point::Point.new(col, row)
    maze << p if char == '#'
    starting_position = p if char == 'S'
    destination = p if char == 'E'
  end
end

starting_position = T.must(starting_position)
destination = T.must(destination)

cost = find_path(starting_position, destination, maze)
puts "Shortest path data: #{cost}"
#puts "Total positions on shortest paths: #{get_shortest_path_count(starting_position, destination, maze, cost)}"
