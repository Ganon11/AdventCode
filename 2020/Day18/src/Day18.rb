#!/usr/bin/ruby

require 'optparse'

def evaluate(expression)
  value = nil
  e = expression.dup
  operator = nil
  operands = []
  while not e.empty?
    if e[0] == '+' or e[0] == '*'
      operator = e[0]
      e[0] = ''
    elsif e[0] == '('
      paren_count = 1
      end_index = 1
      while paren_count != 0
        if e[end_index] == '('
          paren_count += 1
        elsif e[end_index] == ')'
          paren_count -= 1
        end
        end_index += 1
      end

      rhs = evaluate(e[1..end_index - 2])
      if value.nil?
        value = rhs
      else
        if operator == '+'
          value += rhs
        else
          value *= rhs
        end
      end

      e[0..end_index - 1] = ''
    else
      rhs = e[0].to_i
      if value.nil?
        value = rhs
      else
        if operator == '+'
          value += rhs
        else
          value *= rhs
        end
      end

      e[0] = ''
    end
  end

  return value
end

def part1(expressions)
  overall_sum = 0
  expressions.each do |expression|
    overall_sum += evaluate(expression)
  end

  return overall_sum
end

def part2(expressions)
  return 0
end

options = {
  :part => 1,
  :filename => "../input/sample1.txt"
}

OptionParser.new do |opts|
  opts.banner = "Usage: Day18.rb [options]"
  opts.on("-p PART", "--part=PART", Integer, "Part 1 or 2")
  opts.on("-f FILENAME", "--filename=FILENAME", String, "Which filename to use?")
end.parse!(into: options)

expressions = IO.readlines(options[:filename]).map{|l| l.strip.gsub(' ', '')}.select{|l| l != ''}

if options[:part] == 1
  puts part1(expressions)
elsif options[:part] == 2
  puts part2(expressions)
end
