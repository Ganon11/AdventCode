require 'direction'
require 'point'
require 'optparse'
require 'sorbet-runtime'

# typed: true
extend T::Sig

class GardenPlot
  extend T::Sig

  sig {returns(T::Set[Point::Point])}
  attr_reader :plot

  sig {returns(Integer)}
  attr_reader :perimeter

  sig {params(plot: T::Set[Point::Point], perimeter: Integer).void}
  def initialize(plot, perimeter)
    @plot = plot
    @perimeter = perimeter
  end

  sig {returns(Integer)}
  def price
    return @plot.length * @perimeter
  end

  sig {returns(Integer)}
  def discount_price
    return 0
  end
end

sig {params(point: Point::Point, garden: T::Hash[Point::Point, String]).returns(GardenPlot)}
def find_plot(point, garden)
  plant_type = T.let(T.must(garden[point]), String)
  frontier = Queue.new([point])
  reached = Set.new([point])
  perimeter = 0

  while !frontier.empty?
    current = T.cast(frontier.pop, Point::Point)
    for neighbor in [current.north, current.south, current.east, current.west]
      if !garden.has_key?(neighbor)
        # Going out-of-bounds is an edge, adding to the perimeter
        perimeter += 1
        next
      end

      # Already in the region
      next if reached.include?(neighbor)

      if garden[neighbor] == plant_type
        # Part of the same region - add it!
        reached << neighbor
        frontier << neighbor
      else
        # Part of a new region - add to the perimiter
        perimeter += 1
        next
      end
    end
  end

  return GardenPlot.new(reached, perimeter)
end

sig {params(garden: T::Hash[Point::Point, String]).returns(T::Array[GardenPlot])}
def find_plots(garden)
  seen = T.let(Set.new, T::Set[Point::Point])
  plots = T.let(Array.new, T::Array[GardenPlot])
  unseen_point = T.let(garden.keys.find{ |p| !seen.include?(p) }, T.nilable(Point::Point))
  while unseen_point
    new_plot = find_plot(unseen_point, garden)
    seen.merge(new_plot.plot)
    plots << new_plot
    unseen_point = garden.keys.find{ |p| !seen.include?(p) }
  end

  return plots
end

options = {
  :filename => 'input.txt'
}

OptionParser.new do |opts|
  opts.banner = 'Usage: d12.rb [options]'
  opts.on('-f FILENAME', '--filename=FILENAME', String, 'Which file to use?')
end.parse!(into: options)

garden = T.let({}, T::Hash[Point::Point, String])
IO.readlines(options[:filename]).map(&:strip).each_with_index do |line, row|
  line.chars.each_with_index do |plant, col|
    garden[Point::Point.new(col, row)] = plant
  end
end

#puts "Found #{garden.length} spaces."
plots = find_plots(garden)
#puts "Found #{plots.length} plots: #{plots}"
total_price = plots.map(&:price).sum
puts "Total price: #{total_price}"
discount_price = plots.map(&:discount_price).sum
puts "Discount price: #{discount_price}"
