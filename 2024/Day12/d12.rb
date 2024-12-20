require 'bounds'
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
  def calculate_sides
    bounds = Point::Bounds.new(@plot)

    vertical_sides = 0
    has_left_border = T.let(Set.new, T::Set[Point::Point])
    has_right_border = T.let(Set.new, T::Set[Point::Point])
    # From top to bottom...
    bounds.yrange.each do |row|
      # Scan from left-to-right
      inside = T.let(false, T::Boolean)
      bounds.xrange.each do |col|
        p = Point::Point.new(col, row)
        if !@plot.include?(p) and inside
          inside = false
          was_inside = p.west
          has_right_border << was_inside
          if !has_right_border.include?(was_inside.north)
            vertical_sides += 1
          end
        elsif @plot.include?(p) and !inside
          inside = true
          has_left_border << p
          if !has_left_border.include?(p.north)
            vertical_sides += 1
          end
        end
      end
    end

    horizontal_sides = 0
    has_top_border = T.let(Set.new, T::Set[Point::Point])
    has_bottom_border = T.let(Set.new, T::Set[Point::Point])
    # From left to right...
    bounds.xrange.each do |col|
      # Scan from top-to-bottom
      inside = T.let(false, T::Boolean)
      bounds.yrange.each do |row|
        p = Point::Point.new(col, row)
        if !@plot.include?(p) and inside
          inside = false
          was_inside = p.north
          has_bottom_border << was_inside
          if !has_bottom_border.include?(was_inside.west)
            horizontal_sides += 1
          end
        elsif @plot.include?(p) and !inside
          inside = true
          has_top_border << p
          if !has_top_border.include?(p.west)
            horizontal_sides += 1
          end
        end
      end
    end

    return vertical_sides + horizontal_sides
  end

  private :calculate_sides

  sig {returns(Integer)}
  def sides
    @sides ||= calculate_sides
  end

  sig {returns(Integer)}
  def price
    return @plot.length * @perimeter
  end

  sig {returns(Integer)}
  def discount_price
    return @plot.length * sides
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

  bounds = Point::Bounds.new(garden.keys)
  (bounds.min_x..bounds.max_x).each do |x|
    (bounds.min_y..bounds.max_y).each do |y|
      p = Point::Point.new(x, y)
      next if seen.include?(p)
      
      new_plot = find_plot(p, garden)
      seen.merge(new_plot.plot)
      plots << new_plot
    end
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

plots = find_plots(garden)
total_price = plots.map(&:price).sum
puts "Total price: #{total_price}"
discount_price = plots.map(&:discount_price).sum
puts "Discount price: #{discount_price}"
