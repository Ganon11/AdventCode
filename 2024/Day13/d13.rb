require 'point'
require 'optparse'
require 'sorbet-runtime'

# typed: true
extend T::Sig

class ClawMachine
  extend T::Sig

  sig {returns(Point::Point)}
  attr_reader :a

  sig {returns(Point::Point)}
  attr_reader :b

  sig {returns(Point::Point)}
  attr_reader :prize

  BUTTON_A_FORMAT = /^Button A: X\+(\d+), Y\+(\d+)$/
  BUTTON_B_FORMAT = /^Button B: X\+(\d+), Y\+(\d+)$/
  PRIZE_FORMAT = /^Prize: X=(\d+), Y=(\d+)$/

  sig {params(a_line: String, b_line: String, prize_line: String, part: Integer).void}
  def initialize(a_line, b_line, prize_line, part)
    if !a_line.match(BUTTON_A_FORMAT) { |m| @a = Point::Point.new(m[1].to_i, m[2].to_i) }
      raise StandardError.new "Bad format of a line: #{a_line}"
    end
    if !b_line.match(BUTTON_B_FORMAT) { |m| @b = Point::Point.new(m[1].to_i, m[2].to_i) }
      raise StandardError.new "Bad format of b line: #{b_line}"
    end
    if !prize_line.match(PRIZE_FORMAT) { |m| @prize = Point::Point.new(m[1].to_i + (10000000000000 * (part - 1)), m[2].to_i + (10000000000000 * (part - 1))) }
      raise StandardError.new "Bad format of prize line: #{prize_line}"
    end
  end

  sig {returns(Integer)}
  def solve
    return @solve if @solve

    b_count = ((@a.y * @prize.x) - (@a.x * @prize.y)) / ((@a.y * @b.x) - (@a.x * @b.y))
    a_count = (@prize.x - (@b.x * b_count)) / @a.x

    if a_count * @a.x + b_count * @b.x == @prize.x && a_count * @a.y + b_count * @b.y == @prize.y
      @solve = (a_count * 3) + b_count
    else
      @solve = 0
    end

    return @solve
  end
end

options = {
  :part => 1,
  :filename => 'input.txt'
}

OptionParser.new do |opts|
  opts.banner = 'Usage: d13.rb [options]'
  opts.on('-p PART', '--part=PART', Integer, 'Which part?')
  opts.on('-f FILENAME', '--filename=FILENAME', String, 'Which file to use?')
end.parse!(into: options)

lines = IO.readlines(options[:filename]).map(&:strip)
machines = T.let(Array.new, T::Array[ClawMachine])
index = 0
while index < lines.length
  a_line = T.must(lines[index]).strip
  b_line = T.must(lines[index + 1]).strip
  prize_line = T.must(lines[index + 2]).strip
  machines << ClawMachine.new(a_line, b_line, prize_line, options[:part])
  index += 4
end

tokens = machines.map(&:solve).sum
puts "Tokens: #{tokens}"
