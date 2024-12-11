require 'optparse'
require 'sorbet-runtime'

# typed: true
extend T::Sig

sig {params(stone: Integer).returns(T::Array[Integer])}
def transform_stone(stone)
  if stone == 0
    stone_digits = 1
  else
    stone_digits = Math.log10(stone).to_i + 1
  end

  if stone == 0 # Rule 1: If the stone is engraved with 0, change value to 1
    return [1]
  elsif stone_digits % 2 == 0 # Rule 2: If the stone has an even number of digits, split into 2 stones
    power_of_ten = (10 ** (stone_digits / 2))
    return [(stone / power_of_ten).to_i, (stone % power_of_ten).to_i]
  else # Rule 3: Multiply the stone's value by 2024
    return [stone * 2024]
  end
end

class BlinkMemo
  extend T::Sig

  sig {returns(Integer)}
  attr_reader :stone

  sig {returns(Integer)}
  attr_reader :depth

  sig {params(stone: Integer, depth: Integer).void}
  def initialize(stone, depth)
    @stone = stone
    @depth = depth
  end

  sig {params(other: BlinkMemo).returns(T::Boolean)}
  def eql?(other)
    self.stone == other.stone && self.depth == other.depth
  end

  sig {returns(Integer)}
  def hash
    [@stone, @depth].hash
  end
end

RECURSIVE_BLINK_CACHE = T.let({}, T::Hash[BlinkMemo, Integer])

sig {params(stone: Integer, depth: Integer).returns(Integer)}
def recursive_blink(stone, depth)
  if depth == 0
    return 1
  end

  memo = BlinkMemo.new(stone, depth)
  if RECURSIVE_BLINK_CACHE.include?(memo)
    return T.must(RECURSIVE_BLINK_CACHE[memo])
  end

  children = transform_stone(stone)
  count = children.map{ |child| recursive_blink(child, depth - 1) }.sum
  RECURSIVE_BLINK_CACHE[memo] = count
  return count
end

options = {
  :blinks => 25,
  :filename => 'input.txt'
}

OptionParser.new do |opts|
  opts.banner = 'Usage: d11.rb [options]'
  opts.on('-b BLINKS', '--blinks=BLINKS', Integer, 'How many times are you blinking?')
  opts.on('-f FILENAME', '--filename=FILENAME', String, 'Which file to use?')
end.parse!(into: options)

stones = File.new(options[:filename]).readline.strip.split(' ').map(&:to_i)

total = stones.map{ |stone| recursive_blink(stone, options[:blinks]) }.sum
puts total
