require 'direction'
require 'point'
require 'optparse'
require 'sorbet-runtime'

# typed: true
extend T::Sig

sig {
  params(map: T::Hash[Point::Point, Integer], trailhead: Point::Point)
  .returns(T::Hash[Point::Point, T::Set[Point::Point]])
}
def bfs(map, trailhead)
  frontier = Queue.new
  frontier << trailhead
  came_from = T.let({}, T::Hash[Point::Point, T::Set[Point::Point]])
  came_from[trailhead] = Set.new

  while !frontier.empty?
    current = T.cast(frontier.pop, Point::Point)
    for neighbor in [current.north, current.south, current.east, current.west]
      next if !map.has_key?(neighbor)
      next if came_from.include?(neighbor) && T.must(came_from[neighbor]).include?(current)

      current_height = T.must(map[current])
      next if map[neighbor] != current_height + 1

      if !came_from.include?(neighbor)
        came_from[neighbor] = Set.new([current])
      else
        T.must(came_from[neighbor]) << current
      end
      frontier << neighbor
    end
  end

  return came_from
end

sig {
  params(walked: T::Hash[Point::Point, T::Set[Point::Point]], peak: Point::Point)
  .returns(Integer)
}
def count_paths_from(walked, peak)
  frontier = Queue.new
  frontier << peak
  came_from = T.let({}, T::Hash[Point::Point, T::Set[Point::Point]])
  came_from[peak] = Set.new

  paths = 0

  while !frontier.empty?
    current = T.cast(frontier.pop, Point::Point)
    next if !walked.include?(current)
    if T.must(walked[current]).empty?
      paths += 1
    else
      T.must(walked[current]).each do |neighbor|
        frontier << neighbor
      end
    end
  end

  return paths
end

sig {
  params(map: T::Hash[Point::Point, Integer], trailhead: Point::Point)
  .returns([Integer, Integer])
}
def score_and_rate_trailhead(map, trailhead)
  came_from = bfs(map, trailhead)
  peaks = came_from.select { |p, _| map[p] == 9 }
  score = peaks.count
  rating = 0
  peaks.each do |peak, _|
    rating += count_paths_from(came_from, peak)
  end

  return score, rating
end

options = {
  :filename => 'input.txt'
}

OptionParser.new do |opts|
  opts.banner = 'Usage: d9.rb [options]'
  opts.on('-f FILENAME', '--filename=FILENAME', String, 'Which file to use?')
end.parse!(into: options)

trailmap = T.let({}, T::Hash[Point::Point, Integer])
IO.readlines(options[:filename]).map(&:strip).each_with_index do |line, row|
  line.chars.each_with_index do |height, col|
    if height == '.'
      height = '-1'
    end
    trailmap[Point::Point.new(col, row)] = height.to_i
  end
end

score = 0
rating = 0
trailmap.select{ |point, height| height == 0 }.map { |point, _| point}.each do |point|
  t_score, t_rating = score_and_rate_trailhead(trailmap, point)
  score += t_score
  rating += t_rating
end

puts "Score: #{score}"
puts "Rating: #{rating}"
