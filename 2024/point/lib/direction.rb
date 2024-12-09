# frozen_string_literal: true
# typed: true

require_relative "point/version"
require 'sorbet-runtime'

module Point
  class Direction < T::Enum
    enums do
      North = new
      South = new
      East = new
      West = new
    end
  end
end
