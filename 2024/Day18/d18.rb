require 'direction'
require 'fc'
require 'point'
require 'optparse'
require 'sorbet-runtime'

# typed: true
extend T::Sig

sig {params(pixels: T::Array[Point::Point], steps: Integer).returns(T::Set[Point::Point])}
def simulate_steps(pixels, steps)
  memory_space = T.let(Set.new, T::Set[Point::Point])
  (0..steps - 1).each do |index|
    memory_space << T.must(pixels[index])
  end

  return memory_space
end

sig {params(start: Point::Point, destination: Point::Point, memory: T::Set[Point::Point]).returns(T::Set[Point::Point])}
def a_star(start, destination, memory)
  frontier = FastContainers::PriorityQueue.new(:min)
  frontier.push(start, 0)
  came_from = T.let(Hash.new, T::Hash[Point::Point, T.nilable(Point::Point)])
  cost_so_far = T.let(Hash.new, T::Hash[Point::Point, Integer])
  came_from[start] = nil
  cost_so_far[start] = 0

  while !frontier.empty?
    current = T.cast(frontier.pop, Point::Point)
    break if current == destination

    for neighbor in current.neighbors
      next if neighbor.x < start.x || neighbor.y < start.y
      next if neighbor.x > destination.x || neighbor.y > destination.y
      next if memory.include?(neighbor)
      new_cost = T.cast(cost_so_far[current], Integer) + 1
      if !cost_so_far.include?(neighbor) || new_cost < T.must(cost_so_far[neighbor])
        cost_so_far[neighbor] = new_cost
        priority = new_cost + neighbor.distance(destination)
        frontier.push(neighbor, priority)
        came_from[neighbor] = current
      end
    end
  end

  # Reverse through came_from to build path
  result = T.let(Set.new, T::Set[Point::Point])
  return result if !came_from.include?(destination)
  walkback = destination
  while !walkback.nil?
    result << walkback
    walkback = came_from[walkback]
  end
  return result
end

options = {
  :filename => 'input.txt',
  :steps => 1024
}

OptionParser.new do |opts|
  opts.banner = 'Usage: d18.rb [options]'
  opts.on('-f FILENAME', '--filename=FILENAME', String, 'Which file to read as input?')
  opts.on('-s STEPS', '--steps=STEPS', Integer, 'How many steps to simulate?')
end.parse!(into: options)

pixels = T.let(Array.new, T::Array[Point::Point])
IO.readlines(options[:filename]).each do |line|
  coords = line.strip.split(',')
  pixels << Point::Point.new(coords[0].to_i, coords[1].to_i)
end

puts "Parsed #{pixels.length} pixels."
if options[:filename] == 'input.txt'
  bounds = 70
else
  bounds = 6
end

memory_space = simulate_steps(pixels, options[:steps])
start = Point::Point.new(0, 0)
destination = Point::Point.new(bounds, bounds)
fastest_path = a_star(start, destination, memory_space)
puts "Shortest path: #{fastest_path.length - 1}"

next_pixel = options[:steps]
while !fastest_path.empty?
  while !fastest_path.include?(T.must(pixels[next_pixel]))
    memory_space << T.must(pixels[next_pixel])
    next_pixel += 1
  end

  memory_space << T.must(pixels[next_pixel])
  fastest_path = a_star(start, destination, memory_space)
end

puts "Pixel that broke us: #{pixels[next_pixel].x},#{pixels[next_pixel].y}"
