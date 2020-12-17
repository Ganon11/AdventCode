#!/usr/bin/ruby

require 'optparse'

class Point
  def initialize(x, y, z, w=0)
    @x = x
    @y = y
    @z = z
    @w = w
  end

  def self.new_from_string(str)
    data = str.match(/\((?<x>-?\d+),(?<y>-?\d+),(?<z>-?\d+),(?<w>-?\d+)\)/)
    if data
      self.new(data[:x].to_i, data[:y].to_i, data[:z].to_i, data[:w].to_i)
    end
  end

  def x
    @x
  end

  def y
    @y
  end

  def z
    @z
  end

  def w
    @w
  end

  def neighbor?(other_point)
    x_diff = (@x - other_point.x).abs
    y_diff = (@y - other_point.y).abs
    z_diff = (@z - other_point.z).abs
    w_diff = (@w - other_point.w).abs
    if (x_diff <= 1) and (y_diff <= 1) and (z_diff <= 1) and (w_diff <= 1)
      return true
    else
      return false
    end
  end

  def ==(other)
    if (@x == other.x) and (@y == other.y) and (@z == other.z) and (@w == other.w)
      return true
    else
      return false
    end
  end

  def to_s
    return "(#{@x},#{@y},#{@z},#{@w})"
  end
end

class ConwayField
  def initialize(lines)
    # Start with X = 0, Y = 0, Z = 0, because why not
    @points = {}
    z = 0
    lines.each_with_index do |line, y|
      line.chars.each_with_index do |ch, x|
        if ch == '#'
          @points[Point::new(x, y, z).to_s] = 1
        end
      end
    end
  end

  def serialize
    return @points.keys.map{|p| p.to_s}.join(",")
  end

  def step
    old_points = @points.keys.map{|s| Point::new_from_string(s)}
    min_x = old_points.min{|a,b| a.x <=> b.x}.x - 1
    max_x = old_points.max{|a,b| a.x <=> b.x}.x + 1
    min_y = old_points.min{|a,b| a.y <=> b.y}.y - 1
    max_y = old_points.max{|a,b| a.y <=> b.y}.y + 1
    min_z = old_points.min{|a,b| a.z <=> b.z}.z - 1
    max_z = old_points.max{|a,b| a.z <=> b.z}.z + 1

    new_points = {}
    (min_x..max_x).each do |x|
      (min_y..max_y).each do |y|
        (min_z..max_z).each do |z|
          new_point = Point::new(x, y, z)
          neighbors = count_neighbors(new_point)
          if not @points.key?(new_point.to_s) and neighbors == 3
            new_points[new_point.to_s] = 1
          elsif @points.key?(new_point.to_s) and (neighbors == 2 or neighbors == 3)
            new_points[new_point.to_s] = 1
          end
        end
      end
    end

    @points = new_points.dup
  end

  def count_neighbors(point)
    sum = 0
    (point.x - 1 .. point.x + 1).each do |x|
      (point.y - 1 .. point.y + 1).each do |y|
        (point.z - 1 .. point.z + 1).each do |z|
          neighbor = Point::new(x, y, z)
          next if neighbor == point
          if @points.key?(neighbor.to_s)
            sum += 1
          end
        end
      end
    end

    return sum
  end

  def active_cubes
    return @points.length
  end
end

class FourDimensionalConwayField
  def initialize(lines)
    # Start with X = 0, Y = 0, Z = 0, W = 0 because why not
    @points = {}
    z = 0
    w = 0
    lines.each_with_index do |line, y|
      line.chars.each_with_index do |ch, x|
        if ch == '#'
          @points[Point::new(x, y, z, w).to_s] = 1
        end
      end
    end
  end

  def serialize
    return @points.keys.map{|p| p.to_s}.join(",")
  end

  def step
    old_points = @points.keys.map{|s| Point::new_from_string(s)}
    min_x = old_points.min{|a,b| a.x <=> b.x}.x - 1
    max_x = old_points.max{|a,b| a.x <=> b.x}.x + 1
    min_y = old_points.min{|a,b| a.y <=> b.y}.y - 1
    max_y = old_points.max{|a,b| a.y <=> b.y}.y + 1
    min_z = old_points.min{|a,b| a.z <=> b.z}.z - 1
    max_z = old_points.max{|a,b| a.z <=> b.z}.z + 1
    min_w = old_points.min{|a,b| a.w <=> b.w}.w - 1
    max_w = old_points.max{|a,b| a.w <=> b.w}.w + 1

    new_points = {}
    (min_x..max_x).each do |x|
      (min_y..max_y).each do |y|
        (min_z..max_z).each do |z|
          (min_w..max_w).each do |w|
            new_point = Point::new(x, y, z, w)
            neighbors = count_neighbors(new_point)
            if not @points.key?(new_point.to_s) and neighbors == 3
              new_points[new_point.to_s] = 1
            elsif @points.key?(new_point.to_s) and (neighbors == 2 or neighbors == 3)
              new_points[new_point.to_s] = 1
            end
          end
        end
      end
    end

    @points = new_points
  end

  def count_neighbors(point)
    sum = 0
    (point.x - 1 .. point.x + 1).each do |x|
      (point.y - 1 .. point.y + 1).each do |y|
        (point.z - 1 .. point.z + 1).each do |z|
          (point.w - 1 .. point.w + 1).each do |w|
            neighbor = Point::new(x, y, z, w)
            next if neighbor == point
            if @points.key?(neighbor.to_s)
              sum += 1
            end
          end
        end
      end
    end

    return sum
  end

  def active_cubes
    return @points.length
  end
end

options = {
  :part => 1,
  :filename => "../input/sample.txt"
}

OptionParser.new do |opts|
  opts.banner = "Usage: Day17.rb [options]"
  opts.on("-p PART", "--part=PART", Integer, "Part 1 or 2")
  opts.on("-f FILENAME", "--filename=FILENAME", String, "Which filename to use?")
end.parse!(into: options)

lines = IO.readlines(options[:filename]).map{|line| line.strip}
field = nil

if options[:part] == 1
  field = ConwayField::new(lines)
elsif options[:part] == 2
  field = FourDimensionalConwayField::new(lines)
end

6.times do
  field.step
end

puts field.active_cubes
