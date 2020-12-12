#!/usr/bin/ruby

class Ferry
  @@delta_x = {
    :NORTH => 0,
    :SOUTH => 0,
    :EAST => 1,
    :WEST => -1
  }

  @@delta_y = {
    :NORTH => 1,
    :SOUTH => -1,
    :EAST => 0,
    :WEST => 0
  }

  def initialize()
    @x = 0
    @y = 0
    @facing = :EAST
  end

  def follow_instruction(instruction)
    action = instruction.chars[0]
    instruction[0] = ''
    value = instruction.to_i
    case action
    when 'N'
      @y += value
    when 'S'
      @y -= value
    when 'E'
      @x += value
    when 'W'
      @x -= value
    when 'L'
      rotate(:COUNTERCLOCKWISE, value)
    when 'R'
      rotate(:CLOCKWISE, value)
    when 'F'
      case @facing
      when :NORTH
        @y += value
      when :SOUTH
        @y -= value
      when :EAST
        @x += value
      when :WEST
        @x -= value
      end
    end
  end

  def rotate(direction, degrees)
    case degrees
    when 90
      case direction
      when :CLOCKWISE
        case @facing
        when :NORTH
          @facing = :EAST
        when :SOUTH
          @facing = :WEST
        when :EAST
          @facing = :SOUTH
        when :WEST
          @facing = :NORTH
        end
      when :COUNTERCLOCKWISE
        case @facing
        when :NORTH
          @facing = :WEST
        when :SOUTH
          @facing = :EAST
        when :EAST
          @facing = :NORTH
        when :WEST
          @facing = :SOUTH
        end
      else
        puts "Unrecognized direction"
      end
    when 180
      # Clockwise v counter-clockwise doesn't matter
      case @facing
      when :NORTH
        @facing = :SOUTH
      when :SOUTH
        @facing = :NORTH
      when :EAST
        @facing = :WEST
      when :WEST
        @facing = :EAST
      end
    when 270
      case direction
      when :CLOCKWISE
        case @facing
        when :NORTH
          @facing = :WEST
        when :SOUTH
          @facing = :EAST
        when :EAST
          @facing = :NORTH
        when :WEST
          @facing = :SOUTH
        end
      when :COUNTERCLOCKWISE
        case @facing
        when :NORTH
          @facing = :EAST
        when :SOUTH
          @facing = :WEST
        when :EAST
          @facing = :SOUTH
        when :WEST
          @facing = :NORTH
        end
      else
        puts "Unrecognized direction"
      end
    else
      puts "Unrecognized degrees"
    end
  end

  def distance_from_origin
    @x.abs + @y.abs
  end
end

class FerryWithWaypoint < Ferry
  def initialize
    super
    @waypoint_x = 10
    @waypoint_y = 1
  end

  def follow_instruction(instruction)
    action = instruction.chars[0]
    instruction[0] = ''
    value = instruction.to_i
    case action
    when 'N'
      @waypoint_y += value
    when 'S'
      @waypoint_y -= value
    when 'E'
      @waypoint_x += value
    when 'W'
      @waypoint_x -= value
    when 'L'
      case value
      when 90
        tmp = @waypoint_y
        @waypoint_y = @waypoint_x
        @waypoint_x = -tmp
      when 180
        @waypoint_x *= -1
        @waypoint_y *= -1
      when 270
        tmp = @waypoint_y
        @waypoint_y = -@waypoint_x
        @waypoint_x = tmp
      end
    when 'R'
      case value
      when 90
        tmp = @waypoint_y
        @waypoint_y = -@waypoint_x
        @waypoint_x = tmp
      when 180
        @waypoint_x *= -1
        @waypoint_y *= -1
      when 270
        tmp = @waypoint_y
        @waypoint_y = @waypoint_x
        @waypoint_x = -tmp
      end
    when 'F'
      value.times do |i|
        @x += @waypoint_x
        @y += @waypoint_y
      end
    end
  end
end

def part1(lines)
  ferry = Ferry.new
  lines.each do |line|
    ferry.follow_instruction(line)
  end

  return ferry.distance_from_origin
end

def part2(lines)
  ferry = FerryWithWaypoint.new
  lines.each do |line|
    ferry.follow_instruction(line)
  end

  return ferry.distance_from_origin
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

if options[:part] == 1
  puts part1(instructions)
elsif options[:part] == 2
  puts part2(instructions)
end
