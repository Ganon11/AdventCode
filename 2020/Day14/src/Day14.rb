#!/usr/bin/ruby

require 'optparse'

def transform(value, mask)
  transformed_str = value.to_s(2).rjust(mask.length, '0')
  transformed_str.chars.each_with_index do |c, i|
    if mask[i] != 'X'
      transformed_str[i] = mask[i]
    end
  end
  result = transformed_str.to_i(base=2)
  return result
end

def part1(lines)
  memory = {}
  mask = ''

  lines.each do |line|
    if matchdata = line.match(/mask = (?<mask>[01X]{36})/)
      mask = matchdata[:mask]
    elsif matchdata = line.match(/mem\[(?<key>\d+)\] = (?<value>\d+)/)
      memory[matchdata[:key].to_i] = transform(matchdata[:value].to_i, mask)
    end
  end

  return memory.map {|k, v| v}.sum
end

def expand(key)
  values = []
  if not i = key.index('X')
    values.push(key.to_i(base=2))
  else
    temp_key = key.dup
    temp_key[i] = '0'
    values += expand(temp_key)
    temp_key[i] = '1'
    values += expand(temp_key)
  end

  return values
end

def get_values(mask, key)
  ['1', 'X'].each do |ch|
    offset = 0
    while i = mask.index(ch, offset)
      key[i] = ch
      offset = i + 1
    end
  end

  return expand(key)
end

def part2(lines)
  memory = {}
  mask = ''

  lines.each do |line|
    if matchdata = line.match(/mask = (?<mask>[01X]{36})/)
      mask = matchdata[:mask]
    elsif matchdata = line.match(/mem\[(?<key>\d+)\] = (?<value>\d+)/)
      key = matchdata[:key].to_i
      value = matchdata[:value].to_i
      memvals = get_values(mask, key.to_s(2).rjust(mask.length, '0'))
      memvals.each do |key|
        memory[key] = value
      end
    end
  end

  return memory.map {|k, v| v}.sum
end

options = {
  :part => 1,
  :filename => "../input/sample.txt"
}

OptionParser.new do |opts|
  opts.banner = "Usage: Day14.rb [options]"
  opts.on("-p PART", "--part=PART", Integer, "Part 1 or 2")
  opts.on("-f FILENAME", "--filename=FILENAME", String, "Which filename to use?")
end.parse!(into: options)

lines = IO.readlines(options[:filename]).map{|line| line.strip}

if options[:part] == 1
  puts part1(lines)
elsif options[:part] == 2
  puts part2(lines)
end
