#!/usr/bin/ruby

require 'optparse'

class Password
  def initialize(line)
    data = line.match(/(?<digit1>\d+)-(?<digit2>\d+) (?<ch>.): (?<pass>.+)/)
    if data
      @password = data[:pass]
      @character = data[:ch]
      @digit1 = data[:digit1].to_i
      @digit2 = data[:digit2].to_i
    else
      puts "Error in regex matching!"
    end
  end

  def password
    @password
  end

  def character
    @character
  end

  def min
    @minimum
  end

  def max
    @maximum
  end

  def print
    puts "Password #{@password}"
    puts "Requires character #{@character}"
    puts "Digit 1 #{@digit1}"
    puts "Digit 2 #{@digit2}"
  end

  def is_valid_part_1?
    count = @password.chars.count {|c| c == @character}
    if (@digit1 <= count) and (count <= @digit2)
      return true
    end

    return false
  end

  def is_valid_part_2?
    return (@password.chars[@digit1 - 1] == @character) ^
      (@password.chars[@digit2 - 1] == @character)
  end
end

def part1(ps_and_ps)
  return ps_and_ps.count do |p|
    p.is_valid_part_1?
  end
end

def part2(ps_and_ps)
  return ps_and_ps.count do |p|
    p.is_valid_part_2?
  end
end

options = {
  :part => 1,
  :filename => "../input/sample.txt"
}

OptionParser.new do |opts|
  opts.banner = "Usage: Day2.rb [options]"
  opts.on("-p PART", "--part=PART", Integer, "Part 1 or 2")
  opts.on("-f FILENAME", "--filename=FILENAME", String, "Which filename to use?")
end.parse!(into: options)

@passwords_and_policies = []
IO.readlines(options[:filename]).each do |line|
  p = Password.new(line)
  @passwords_and_policies.push p
end

if options[:part] == 1
  count = part1(@passwords_and_policies)
elsif options[:part] == 2
  count = part2(@passwords_and_policies)
end

puts count
