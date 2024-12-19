require 'optparse'
require 'sorbet-runtime'

# typed: true
extend T::Sig

TOWEL_CACHE = T.let(Hash.new, T::Hash[String, Integer])

sig {params(pattern: String, towels: T::Array[String]).returns(Integer)}
def valid_arrangements(pattern, towels)
  return 1 if pattern.empty?

  if TOWEL_CACHE.include?(pattern)
    return T.must(TOWEL_CACHE[pattern])
  end

  result = T.let(0, Integer)

  towels.each do |towel|
    next if pattern.length < towel.length
    next if !pattern.start_with?(towel)
    result += valid_arrangements(T.must(pattern[towel.length..]), towels)
  end

  TOWEL_CACHE[pattern] = result
  return result
end

options = {
  :filename => 'input.txt'
}

OptionParser.new do |opts|
  opts.banner = 'Usage: d18.rb [options]'
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
