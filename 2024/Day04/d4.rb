require 'optparse'
require 'point'

# typed: true
extend T::Sig

sig {params(puzzle: T::Hash[Point::Point, String]).returns(Integer)}
def find_max_row(puzzle)
  max_row_point = puzzle.keys.max_by{ |key, value| key.y }
  max_row_point.y if max_row_point
  -1
end

sig {params(puzzle: T::Hash[Point::Point, String]).returns(Integer)}
def find_max_col(puzzle)
  max_col_point = puzzle.keys.max_by{ |key, value| key.x }
  max_col_point.x if max_col_point
  -1
end

sig {params(puzzle: T::Hash[Point::Point, String], x: Point::Point, m: Point::Point, a: Point::Point, s: Point::Point).returns(T::Boolean)}
def search(puzzle, x, m, a, s)
  if [x, m, a, s].any? { |point| !puzzle.key?(point) }
    false
  end

  puzzle[x] == 'X' && puzzle[m] == 'M' && puzzle[a] == 'A' && puzzle[s] == 'S'
end

sig {params(puzzle: T::Hash[Point::Point, String], p: Point::Point).returns(T::Boolean)}
def search_right(puzzle, p)
  search(puzzle, p, Point::Point.new(p.x + 1, p.y), Point::Point.new(p.x + 2, p.y), Point::Point.new(p.x + 3, p.y))
end

sig {params(puzzle: T::Hash[Point::Point, String], p: Point::Point).returns(T::Boolean)}
def search_left(puzzle, p)
  search(puzzle, p, Point::Point.new(p.x - 1, p.y), Point::Point.new(p.x - 2, p.y), Point::Point.new(p.x - 3, p.y))
end

sig {params(puzzle: T::Hash[Point::Point, String], p: Point::Point).returns(T::Boolean)}
def search_down(puzzle, p)
  search(puzzle, p, Point::Point.new(p.x, p.y + 1), Point::Point.new(p.x, p.y + 2), Point::Point.new(p.x, p.y + 3))
end

sig {params(puzzle: T::Hash[Point::Point, String], p: Point::Point).returns(T::Boolean)}
def search_up(puzzle, p)
  search(puzzle, p, Point::Point.new(p.x, p.y - 1), Point::Point.new(p.x, p.y - 2), Point::Point.new(p.x, p.y - 3))
end

sig {params(puzzle: T::Hash[Point::Point, String], p: Point::Point).returns(T::Boolean)}
def search_dr(puzzle, p)
  search(puzzle, p, Point::Point.new(p.x + 1, p.y + 1), Point::Point.new(p.x + 2, p.y + 2), Point::Point.new(p.x + 3, p.y + 3))
end

sig {params(puzzle: T::Hash[Point::Point, String], p: Point::Point).returns(T::Boolean)}
def search_dl(puzzle, p)
  search(puzzle, p, Point::Point.new(p.x - 1, p.y + 1), Point::Point.new(p.x - 2, p.y + 2), Point::Point.new(p.x - 3, p.y + 3))
end

sig {params(puzzle: T::Hash[Point::Point, String], p: Point::Point).returns(T::Boolean)}
def search_ur(puzzle, p)
  search(puzzle, p, Point::Point.new(p.x + 1, p.y - 1), Point::Point.new(p.x + 2, p.y - 2), Point::Point.new(p.x + 3, p.y - 3))
end

sig {params(puzzle: T::Hash[Point::Point, String], p: Point::Point).returns(T::Boolean)}
def search_ul(puzzle, p)
  search(puzzle, p, Point::Point.new(p.x - 1, p.y - 1), Point::Point.new(p.x - 2, p.y - 2), Point::Point.new(p.x - 3, p.y - 3))
end

sig {params(puzzle: T::Hash[Point::Point, String], m_1: Point::Point, m_2: Point::Point, a: Point::Point, s_1: Point::Point, s_2: Point::Point).returns(T::Boolean)}
def search_x_mas(puzzle, m_1, m_2, a, s_1, s_2)
  if [m_1, m_2, a, s_1, s_2].any? { |point| !puzzle.key?(point) }
    false
  end

  puzzle[m_1] == 'M' && puzzle[m_2] == 'M' && puzzle[a] == 'A' && puzzle[s_1] == 'S' && puzzle[s_2] == 'S'
end

sig {params(puzzle: T::Hash[Point::Point, String], p: Point::Point).returns(T::Boolean)}
def search_x_mas_msams(puzzle, p)
  m_1 = p
  m_2 = Point::Point.new(p.x, p.y + 2)
  a = Point::Point.new(p.x + 1, p.y + 1)
  s_1 = Point::Point.new(p.x + 2, p.y)
  s_2 = Point::Point.new(p.x + 2, p.y + 2)
  search_x_mas(puzzle, m_1, m_2, a, s_1, s_2)
end

sig {params(puzzle: T::Hash[Point::Point, String], p: Point::Point).returns(T::Boolean)}
def search_x_mas_mmass(puzzle, p)
  m_1 = p
  m_2 = Point::Point.new(p.x + 2, p.y)
  a = Point::Point.new(p.x + 1, p.y + 1)
  s_1 = Point::Point.new(p.x, p.y + 2)
  s_2 = Point::Point.new(p.x + 2, p.y + 2)
  search_x_mas(puzzle, m_1, m_2, a, s_1, s_2)
end

sig {params(puzzle: T::Hash[Point::Point, String], p: Point::Point).returns(T::Boolean)}
def search_x_mas_smasm(puzzle, p)
  s_1 = p
  s_2 = Point::Point.new(p.x, p.y + 2)
  a = Point::Point.new(p.x + 1, p.y + 1)
  m_1 = Point::Point.new(p.x + 2, p.y)
  m_2 = Point::Point.new(p.x + 2, p.y + 2)
  search_x_mas(puzzle, m_1, m_2, a, s_1, s_2)
end

sig {params(puzzle: T::Hash[Point::Point, String], p: Point::Point).returns(T::Boolean)}
def search_x_mas_ssamm(puzzle, p)
  s_1 = p
  s_2 = Point::Point.new(p.x + 2, p.y)
  a = Point::Point.new(p.x + 1, p.y + 1)
  m_1 = Point::Point.new(p.x, p.y + 2)
  m_2 = Point::Point.new(p.x + 2, p.y + 2)
  search_x_mas(puzzle, m_1, m_2, a, s_1, s_2)
end

sig {params(puzzle: T::Hash[Point::Point, String]).returns(Integer)}
def find_xmas(puzzle)
  max_row = find_max_row(puzzle)
  max_col = find_max_col(puzzle)

  sum = 0
  (0..max_col).each do |row|
    (0..max_row).each do |col|
      p = Point::Point.new(col, row)
      sum += 1 if search_right(puzzle, p)
      sum += 1 if search_left(puzzle, p)
      sum += 1 if search_up(puzzle, p)
      sum += 1 if search_down(puzzle, p)
      sum += 1 if search_dr(puzzle, p)
      sum += 1 if search_dl(puzzle, p)
      sum += 1 if search_ur(puzzle, p)
      sum += 1 if search_ul(puzzle, p)
    end
  end

  sum
end

sig {params(puzzle: T::Hash[Point::Point, String]).returns(Integer)}
def find_x_mas(puzzle)
  max_row = find_max_row(puzzle)
  max_col = find_max_col(puzzle)

  sum = 0
  (0..max_col).each do |row|
    (0..max_row).each do |col|
      p = Point::Point.new(col, row)
      sum += 1 if search_x_mas_msams(puzzle, p)
      sum += 1 if search_x_mas_mmass(puzzle, p)
      sum += 1 if search_x_mas_smasm(puzzle, p)
      sum += 1 if search_x_mas_ssamm(puzzle, p)
    end
  end

  sum
end

options = {
  :filename => 'input.txt'
}

OptionParser.new do |opts|
  opts.banner = 'Usage: d4.rb [options]'
  opts.on('-f FILENAME', '--filename=FILENAME', String, 'Which file to use?')
end.parse!(into: options)

puzzle = {}
IO.readlines(options[:filename]).map(&:strip).each_with_index do |line, row|
  line.each_char.with_index do |letter, col|
    p = Point::Point.new(col, row)
    puzzle[p] = letter
  end
end

puts "XMAS:  #{find_xmas(puzzle)}"
puts "X-MAS: #{find_x_mas(puzzle)}"
