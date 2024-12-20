# frozen_string_literal: true
# typed: true

require_relative "point/version"
require 'sorbet-runtime'

module Point
  class Point
    extend T::Sig

    sig {returns(Integer)}
    attr_reader :x

    sig {returns(Integer)}
    attr_reader :y

    sig {params(x: Integer, y: Integer).void}
    def initialize(x, y)
      @x = x
      @y = y
    end

    sig {returns(Point)}
    def north
      Point.new(@x, @y - 1)
    end

    sig {returns(Point)}
    def south
      Point.new(@x, @y + 1)
    end

    sig {returns(Point)}
    def east
      Point.new(@x + 1, @y)
    end

    sig {returns(Point)}
    def west
      Point.new(@x - 1, @y)
    end

    sig {returns(T::Array[Point])}
    def neighbors
      return neighbors_at_distance(1)
    end

    sig {params(distance: Integer).returns(T::Array[Point])}
    def neighbors_at_distance(distance)
      neighbors = T.let(Array.new, T::Array[Point])
      (-distance..distance).each do |dist|
        y_dist = distance - dist.abs
        neighbors << Point.new(@x + dist, @y + y_dist)
        neighbors << Point.new(@x + dist, @y - y_dist) if !y_dist.zero?
      end

      return neighbors
    end

    sig {params(min_x: Integer, min_y: Integer, max_x: Integer, max_y: Integer).returns(T::Boolean)}
    def in_bounds?(min_x, min_y, max_x, max_y)
      return min_x <= @x && @x <= max_x && min_y <= @y && @y <= max_y
    end

    sig {params(other: Point).returns(T::Boolean)}
    def eql?(other)
      @x == other.x && @y == other.y
    end

    sig {returns(Integer)}
    def hash
      [@x, @y].hash
    end

    sig {returns(String)}
    def to_s
      "(#{@x}, #{@y})"
    end

    sig {params(other: Point).returns(Point)}
    def +(other)
      return Point.new(@x + other.x, @y + other.y)
    end

    sig {params(other: Point).returns(Integer)}
    def distance(other)
      return (@x - other.x).abs + (@y - other.y).abs
    end

    sig {params(other: Point).returns(Point)}
    def next_in_line(other)
      xdiff = other.x - @x
      ydiff = other.y - @y
      return Point.new(other.x + xdiff, other.y + ydiff)
    end

    sig {params(points: T::Enumerable[Point]).returns(Integer)}
    def self.min_x(points)
      T.must(points.map{ |p| p.x }.min)
    end

    sig {params(points: T::Enumerable[Point]).returns(Integer)}
    def self.min_y(points)
      T.must(points.map{ |p| p.y }.min)
    end

    sig {params(points: T::Enumerable[Point]).returns(Integer)}
    def self.max_x(points)
      T.must(points.map{ |p| p.x }.max)
    end

    sig {params(points: T::Enumerable[Point]).returns(Integer)}
    def self.max_y(points)
      T.must(points.map{ |p| p.y }.max)
    end
  end
end
