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
  end
end
