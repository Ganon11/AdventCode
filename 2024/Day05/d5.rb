require 'optparse'
require 'sorbet-runtime'

# typed: true
extend T::Sig

class OrderingRule
  extend T::Sig

  sig {returns(Integer)}
  attr_reader :first

  sig {returns(Integer)}
  attr_reader :second

  sig {params(line: String).void}
  def initialize(line)
    @first, @second = line.split('|').map(&:to_i)
  end

  sig {params(manual: SafetyManual).returns(T::Boolean)}
  def manual_is_valid?(manual)
    if !manual.pages.include?(@first)
      true
    elsif !manual.pages.include?(@second)
      true
    elsif T.must(manual.pages.index(@first)) < T.must(manual.pages.index(@second))
      true
    else
      false
    end
  end

  sig {returns(String)}
  def to_s
    "#{@first}|#{@second}"
  end
end

class SafetyManual
  extend T::Sig

  sig {returns(T::Array[Integer])}
  attr_reader :pages

  sig {params(line: String).void}
  def initialize(line)
    @pages = line.split(',').map(&:to_i)
  end

  sig {params(rules: T::Array[OrderingRule]).returns(T::Boolean)}
  def valid?(rules)
    rules.all? { |rule| rule.manual_is_valid?(self) }
  end

  sig {returns(Integer)}
  def middle
    @pages[@pages.length / 2]
  end

  sig {returns(String)}
  def to_s
    "(#{@pages.join(', ')})"
  end
end

options = {
  :filename => 'input.txt'
}

OptionParser.new do |opts|
  opts.banner = 'Usage: d5.rb [options]'
  opts.on('-f FILENAME', '--filename=FILENAME', String, 'Which file to use?')
end.parse!(into: options)

rules = T.let([], T::Array[OrderingRule])
manuals = T.let([], T::Array[SafetyManual])
reading_rules = T.let(true, T::Boolean)
IO.readlines(options[:filename]).map(&:strip).each do |line|
  if line.empty?
    reading_rules = false
    next
  end

  if reading_rules
    rules << OrderingRule.new(line)
  else
    manuals << SafetyManual.new(line)
  end
end

correct_sum = manuals.select{ |manual| manual.valid?(rules) }.map{ |manual| manual.middle }.sum
puts "Correct Manual sum: #{correct_sum}"

sorted_sum = T.let(0, Integer)
swap_sum = T.let(0, Integer)
manuals.select{ |manual| !manual.valid?(rules) }.each do |manual|
  swap_count = T.let(0, Integer)
  while invalid_rule = rules.detect{ |rule| !rule.manual_is_valid?(manual) } do
    index1 = manual.pages.index(invalid_rule.first)
    index2 = manual.pages.index(invalid_rule.second)
    manual.pages[T.must(index1)] = invalid_rule.second
    manual.pages[T.must(index2)] = invalid_rule.first
    swap_count += 1
  end

  puts "I fixed Manual #{manual} but it took #{swap_count} swaps, gross."
  swap_sum += swap_count
  sorted_sum += manual.middle
end

puts "I sorted all manuals, but it took #{swap_sum} swaps, gross."
puts "Sorted Manual sum: #{sorted_sum}"
