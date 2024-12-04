require 'optparse'
require 'point'

def find_max_row(puzzle)
  puzzle.keys.max_by{ |key, value| key.y }.y
end

def find_max_col(puzzle)
  puzzle.keys.max_by{ |key, value| key.x }.x
end

def search(puzzle, x, m, a, s)
  if [x, m, a, s].any? { |point| !puzzle.key?(point) }
    false
  end

  puzzle[x] == 'X' && puzzle[m] == 'M' && puzzle[a] == 'A' && puzzle[s] == 'S'
end

def search_right(puzzle, p)
  search(puzzle, p, Point::Point.new(p.x + 1, p.y), Point::Point.new(p.x + 2, p.y), Point::Point.new(p.x + 3, p.y))
end

def search_left(puzzle, p)
  search(puzzle, p, Point::Point.new(p.x - 1, p.y), Point::Point.new(p.x - 2, p.y), Point::Point.new(p.x - 3, p.y))
end

def search_down(puzzle, p)
  search(puzzle, p, Point::Point.new(p.x, p.y + 1), Point::Point.new(p.x, p.y + 2), Point::Point.new(p.x, p.y + 3))
end

def search_up(puzzle, p)
  search(puzzle, p, Point::Point.new(p.x, p.y - 1), Point::Point.new(p.x, p.y - 2), Point::Point.new(p.x, p.y - 3))
end

def search_dr(puzzle, p)
  search(puzzle, p, Point::Point.new(p.x + 1, p.y + 1), Point::Point.new(p.x + 2, p.y + 2), Point::Point.new(p.x + 3, p.y + 3))
end

def search_dl(puzzle, p)
  search(puzzle, p, Point::Point.new(p.x - 1, p.y + 1), Point::Point.new(p.x - 2, p.y + 2), Point::Point.new(p.x - 3, p.y + 3))
end

def search_ur(puzzle, p)
  search(puzzle, p, Point::Point.new(p.x + 1, p.y - 1), Point::Point.new(p.x + 2, p.y - 2), Point::Point.new(p.x + 3, p.y - 3))
end

def search_ul(puzzle, p)
  search(puzzle, p, Point::Point.new(p.x - 1, p.y - 1), Point::Point.new(p.x - 2, p.y - 2), Point::Point.new(p.x - 3, p.y - 3))
end

def search_x_mas(puzzle, m_1, m_2, a, s_1, s_2)
  if [m_1, m_2, a, s_1, s_2].any? { |point| !puzzle.key?(point) }
    false
  end

  puzzle[m_1] == 'M' && puzzle[m_2] == 'M' && puzzle[a] == 'A' && puzzle[s_1] == 'S' && puzzle[s_2] == 'S'
end

def search_x_mas_msams(puzzle, p)
  m_1 = p
  m_2 = Point::Point.new(p.x, p.y + 2)
  a = Point::Point.new(p.x + 1, p.y + 1)
  s_1 = Point::Point.new(p.x + 2, p.y)
  s_2 = Point::Point.new(p.x + 2, p.y + 2)
  search_x_mas(puzzle, m_1, m_2, a, s_1, s_2)
end

def search_x_mas_mmass(puzzle, p)
  m_1 = p
  m_2 = Point::Point.new(p.x + 2, p.y)
  a = Point::Point.new(p.x + 1, p.y + 1)
  s_1 = Point::Point.new(p.x, p.y + 2)
  s_2 = Point::Point.new(p.x + 2, p.y + 2)
  search_x_mas(puzzle, m_1, m_2, a, s_1, s_2)
end

def search_x_mas_smasm(puzzle, p)
  s_1 = p
  s_2 = Point::Point.new(p.x, p.y + 2)
  a = Point::Point.new(p.x + 1, p.y + 1)
  m_1 = Point::Point.new(p.x + 2, p.y)
  m_2 = Point::Point.new(p.x + 2, p.y + 2)
  search_x_mas(puzzle, m_1, m_2, a, s_1, s_2)
end

def search_x_mas_ssamm(puzzle, p)
  s_1 = p
  s_2 = Point::Point.new(p.x + 2, p.y)
  a = Point::Point.new(p.x + 1, p.y + 1)
  m_1 = Point::Point.new(p.x, p.y + 2)
  m_2 = Point::Point.new(p.x + 2, p.y + 2)
  search_x_mas(puzzle, m_1, m_2, a, s_1, s_2)
end

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
