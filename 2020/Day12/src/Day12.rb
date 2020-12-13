#!/usr/bin/ruby

class Ferry
  def initialize()
    @position = { :x => 0, :y => 0 }
    # The first value is used as the delta X, and the second value is used as the delta Y, for
    # determining what changes when we move forward.
    # When we rotate left or right, we rotate the @direction array accordingly to update which
    # way we're facing.
    # Idea adapted from a reddit comment!
    @direction = [1, 0, -1, 0]
  end

  def follow_instruction(instruction)
    value = instruction[1..].to_i
    case instruction[0]
    when 'N' then @position[:y] += value
    when 'S' then @position[:y] -= value
    when 'E' then @position[:x] += value
    when 'W' then @position[:x] -= value
    when 'L' then @direction.rotate!(-1 * (value / 90))
    when 'R' then @direction.rotate!(value / 90)
    when 'F'
      @position[:x] += (@direction[0] * value)
      @position[:y] += (@direction[1] * value)
    end
  end

  def distance_from_origin
    @position[:x].abs + @position[:y].abs
  end
end

class FerryWithWaypoint < Ferry
  def initialize
    super
    @waypoint = { :x => 10, :y => 1 }
  end

  def follow_instruction(instruction)
    value = instruction[1..].to_i
    case instruction[0]
    when 'N' then @waypoint[:y] += value
    when 'S' then @waypoint[:y] -= value
    when 'E' then @waypoint[:x] += value
    when 'W' then @waypoint[:x] -= value
    when 'L'
      (value / 90).times do
        @waypoint = { :x => -@waypoint[:y], :y => @waypoint[:x] }
      end
    when 'R'
      (value / 90).times do
        @waypoint = { :x => @waypoint[:y], :y => -@waypoint[:x] }
      end
    when 'F'
      @position[:x] += (@waypoint[:x] * value)
      @position[:y] += (@waypoint[:y] * value)
    end
  end
end

require 'optparse'

options = {
  :part => 1,
  :filename => "../input/sample.txt"
}

OptionParser.new do |opts|
  opts.banner = "Usage: {daystr}.rb [options]"
  opts.on("-p PART", "--part=PART", Integer, "Part 1 or 2")
  opts.on("-f FILENAME", "--filename=FILENAME", String, "Which filename to use?")
end.parse!(into: options)

instructions = IO.readlines(options[:filename]).map {|line| line.strip}

ferry = nil
if options[:part] == 1
  ferry = Ferry.new
elsif options[:part] == 2
  ferry = FerryWithWaypoint.new
end

instructions.each do |instruction|
  ferry.follow_instruction(instruction)
end

puts ferry.distance_from_origin
