#!/usr/bin/ruby

require 'optparse'

$bag_definitions = {}

class Bag
  def initialize(line)
    line.strip!
    inputs = line.split(' contain ')
    data = inputs[0].match(/(?<color>.+) bags/)
    if data
      @color = data[:color]
    else
      puts "Error in regex matching!"
    end

    @bags_contained = {}
    if inputs[1] != 'no other bags.'
      inputs[1].split(', ').each do |output|
        data = output.match(/(?<count>\d+) (?<color>.+) bags?/)
        if data
          @bags_contained[data[:color]] = data[:count].to_i
        else
          puts "Error in regex matching!"
        end
      end
    end
  end

  def color
    @color
  end

  def bags_contained
    @bags_contained
  end

  def can_contain_color?(color)
    if @bags_contained.keys.include?(color)
      return true
    end

    @bags_contained.keys.each do |key|
      if $bag_definitions.key?(key) and $bag_definitions[key].can_contain_color?(color)
        return true
      end
    end

    return false
  end

  def expand
    sum = 0
    @bags_contained.each do |color, count|
      expanded_count = $bag_definitions[color].expand
      sum += (count * expanded_count) + count
    end

    return sum
  end
end

options = {
  :part => 1,
  :filename => "../input/sample.txt"
}

OptionParser.new do |opts|
  opts.banner = "Usage: {daystr}.rb [options]"
  opts.on("-p PART", "--part=PART", Integer, "Part 1 or 2")
  opts.on("-f FILENAME", "--filename=FILENAME", String, "Which filename to use?")
end.parse!(into: options)

IO.readlines(options[:filename]).each do |line|
  b = Bag.new(line)
  $bag_definitions[b.color] = b
end

if options[:part] == 1
  puts $bag_definitions.count {|bag_color, bag| bag.can_contain_color?('shiny gold')}
elsif options[:part] == 2
  # do part 2
  puts $bag_definitions['shiny gold'].expand
end
