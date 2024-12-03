require 'optparse'

options = {
  :part => 1,
  :filename => 'input.txt'
}

OptionParser.new do |opts|
  opts.banner = 'Usage: d2.rb [options]'
  opts.on('-p PART', '--part=PART', Integer, 'Part 1 or 2?')
  opts.on('-f FILENAME', '--filename=FILENAME', String, 'Which file to use?')
end.parse!(into: options)

memory_lines = IO.readlines(options[:filename]).map(&:strip)

sum = 0
enabled = true
DO_FORMAT    = /^do\(\)/
DONT_FORMAT  = /^don't\(\)/
MUL_FORMAT   = /^mul\((\d{1,3}),(\d{1,3})\)/

memory_lines.each do |memory|
  (0..memory.length - 1).each do |start|
    end_index = [start + 11, memory.length - 1].min
    substr = memory[start..end_index]
    if substr.match(DO_FORMAT)
      enabled = true
    elsif substr.match(DONT_FORMAT)
      enabled = false
    elsif m = substr.match(MUL_FORMAT)
      if options[:part] == 2 && !enabled
        next
      end
      sum += m.captures[0].to_i * m.captures[1].to_i
    end
  end
end

puts "Sum: #{sum}"
