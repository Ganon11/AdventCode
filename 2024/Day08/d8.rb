require 'point'
require 'optparse'
require 'sorbet-runtime'

# typed: true
extend T::Sig

sig {params(map: T::Enumerable[Point::Point]).returns(Integer)}
def find_max_row(map)
  T.must(map.map{ |p| p.y }.max)
end

sig {params(map: T::Enumerable[Point::Point]).returns(Integer)}
def find_max_col(map)
  T.must(map.map{ |p| p.x }.max)
end

sig {params(p: Point::Point, max_row: Integer, max_col: Integer).returns(T::Boolean)}
def in_bounds?(p, max_row, max_col)
  return 0 <= p.x && p.x <= max_col && 0 <= p.y && p.y <= max_row
end

sig {params(a: Point::Point, b: Point::Point, limit: T::Boolean, max_row: Integer, max_col: Integer).returns(T::Set[Point::Point])}
def generate_line(a, b, limit, max_row, max_col)
  line = T.let(Set.new([]), T::Set[Point::Point])
  xdiff = b.x - a.x
  ydiff = b.y - a.y
  m = Point::Point.new(b.x + xdiff, b.y + ydiff)
  while in_bounds?(m, max_row, max_col) do
    line << m
    break if limit
    m = Point::Point.new(m.x + xdiff, m.y + ydiff)
  end
  return line
end

options = {
  :part => 1,
  :filename => 'input.txt'
}

OptionParser.new do |opts|
  opts.banner = 'Usage: d8.rb [options]'
  opts.on('-p PART', '--part=PART', Integer, 'Which part?')
  opts.on('-f FILENAME', '--filename=FILENAME', String, 'Which file to use?')
end.parse!(into: options)

map = T.let({}, T::Hash[Point::Point, String])
lines = IO.readlines(options[:filename])
lines.each_with_index do |line, row|
  line.strip.chars.each_with_index do |char, col|
    next if !char.match?(/([[:alpha:]]|[[:digit:]])/)

    p = Point::Point .new(col, row)
    map[p] = char
  end
end

max_row = lines.length - 1
max_col = T.must(lines[0]).length - 2
antinodes = T.let(Set.new, T::Set[Point::Point])
map.values.uniq.each do |frequency|
  antennae = map.select{ |antenna, freq| freq == frequency }.keys
  next if antennae.count < 2
  antennae.combination(2).each do |a, b|
    a = T.must(a)
    b = T.must(b)
    generate_line(a, b, options[:part] == 1, max_row, max_col).each { |p| antinodes << p }
    generate_line(b, a, options[:part] == 1, max_row, max_col).each { |p| antinodes << p }
    if options[:part] == 2
      antinodes << a
      antinodes << b
    end
  end
end

puts "#{antinodes.count} unique locations with antinodes:"
