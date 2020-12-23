#!/usr/bin/ruby

require 'optparse'

def play_game(cups, rounds)
  circle = {}
  (0..cups.length-1).each do |i|
    circle[cups[i]] = cups[(i + 1) % cups.length]
  end

  current_label = cups[-1]

  rounds.times do
    current_label = circle[current_label]

    pick_up = []
    tmp_label = current_label
    3.times do
      tmp_label = circle[tmp_label]
      pick_up.push(tmp_label)
    end

    destination_label = current_label - 1
    while pick_up.include?(destination_label) or destination_label < 1
      destination_label -= 1
      if destination_label < 1
        destination_label = cups.max
      end
    end

    circle[current_label] = circle[pick_up[-1]]
    circle[pick_up[-1]] = circle[destination_label]
    circle[destination_label] = pick_up[0]
  end

  return circle
end

def part1(cups)
  circle = play_game(cups, 100)
  next_ = 1
  str = ''
  8.times do
    next_ = circle[next_]
    str += next_.to_s
  end

  str
end

def part2(cups)
  max_value = cups.max
  cups += (max_value..1000000).to_a
  new_arrangement = play_game(cups, 10000000)
  val1 = new_arrangement[1]
  val2 = new_arrangement[val1]

  return val1 * val2
end

options = {
  :part => 1,
  :filename => "../input/sample.txt"
}

OptionParser.new do |opts|
  opts.banner = "Usage: Day23.rb [options]"
  opts.on("-p PART", "--part=PART", Integer, "Part 1 or 2")
  opts.on("-f FILENAME", "--filename=FILENAME", String, "Which filename to use?")
end.parse!(into: options)

cups = IO.read(options[:filename]).strip.split("").map{|c| c.to_i}

if options[:part] == 1
  puts part1(cups)
elsif options[:part] == 2
  puts part2(cups)
end
