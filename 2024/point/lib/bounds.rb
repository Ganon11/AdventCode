# frozen_string_literal: true
# typed: true

require_relative "point/version"
require 'sorbet-runtime'

module Point
  class Bounds
    extend T::Sig

    sig {returns(Integer)}
    attr_reader :min_x

    sig {returns(Integer)}
    attr_reader :max_x

    sig {returns(Integer)}
    attr_reader :min_y

    sig {returns(Integer)}
    attr_reader :max_y

    sig {params(points: T::Enumerable[Point]).void}
    def initialize(points)
      @min_x = Point.min_x(points)
      @max_x = Point.max_x(points)
      @min_y = Point.min_y(points)
      @max_y = Point.max_y(points)
    end

    sig {returns(T::Range[Integer])}
    def xrange
      @min_x..@max_x
    end

    sig {returns(T::Range[Integer])}
    def yrange
      @min_y..@max_y
    end

    sig {params(p: Point).returns(T::Boolean)}
    def in_bounds?(p)
      return T.must(@min_x <= p.x && p.x <= @max_x &&
        @min_y <= p.y &&
        p.y <= @max_y)
    end
  end
end
