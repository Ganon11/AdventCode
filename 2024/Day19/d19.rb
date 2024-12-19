require 'optparse'
require 'sorbet-runtime'

# typed: true
extend T::Sig

TOWEL_CACHE = T.let(Hash.new, T::Hash[String, Integer])

sig {params(pattern: String, towels: T::Array[String]).returns(Integer)}
def valid_arrangements(pattern, towels)
  return T.must(TOWEL_CACHE[pattern]) if TOWEL_CACHE.include?(pattern)

  result = towels.sum(0) do |towel|
    if pattern.length < towel.length || !pattern.start_with?(towel)
      0
    elsif pattern.eql?(towel)
      1
    else
      valid_arrangements(T.must(pattern[towel.length..]), towels)
    end
  end

  TOWEL_CACHE[pattern] = result
  return result
end

options = {
  :filename => 'input.txt'
}

OptionParser.new do |opts|
  opts.banner = 'Usage: d19.rb [options]'
  opts.on('-f FILENAME', '--filename=FILENAME', String, 'Which file to read as input?')
end.parse!(into: options)

lines = IO.readlines(options[:filename]).map(&:strip)
if lines.length < 3
  raise Exception.new "Bad input: #{lines}"
end

towels = T.let(T.must(lines[0]).split(', '), T::Array[String])
desired = lines[2..]
result = T.must(desired).map { |d| valid_arrangements(d, towels) }

puts "Valid patterns: #{result.select{ |r| r > 0 }.length }"
puts "Valid arrangements: #{result.sum}"
