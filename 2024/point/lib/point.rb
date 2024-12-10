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

    sig {params(min_x: Integer, min_y: Integer, max_x: Integer, max_y: Integer).returns(T::Boolean)}
    def in_bounds?(min_x, min_y, max_x, max_y)
      return min_x <= @x && @x <= max_x && min_y <= @y && @y <= max_y
    end

    sig {params(other: Point).returns(T::Boolean)}
    def eql?(other)
      self.x == other.x && self.y == other.y
    end

    sig {returns(Integer)}
    def hash
      [x, y].hash
    end

    sig {returns(String)}
    def to_s
      "(#{x}, #{y})"
    end

    sig {params(other: Point).returns(Point)}
    def +(other)
      xdiff = other.x - @x
      ydiff = other.y - @y
      return Point.new(other.x + xdiff, other.y + ydiff)
    end
  end
end
