#!/usr/bin/ruby

require 'optparse'

$rules = {}
$pattern_cache = {}

def get_pattern(rule_id, depth = 0, max_depth = 14)
  if depth >= max_depth
    return ''
  end

  if $pattern_cache.key?(rule_id)
    return $pattern_cache[rule_id]
  end

  pattern = '(?:'
  $rules[rule_id].split(' ').each do |token|
    if token == '|'
      pattern += '|'
    elsif token == "\"a\""
      pattern += "a"
    elsif token == "\"b\""
      pattern += "b"
    else
      pattern += get_pattern(token.to_i, depth + 1, max_depth)
    end
  end
  pattern += ')'

  $pattern_cache[rule_id] = pattern
  return pattern
end

def get_pattern_zero
  return "^#{get_pattern(0)}$"
end

options = {
  :part => 1,
  :filename => "../input/sample1.txt"
}

OptionParser.new do |opts|
  opts.banner = "Usage: Day19.rb [options]"
  opts.on("-p PART", "--part=PART", Integer, "Part 1 or 2")
  opts.on("-f FILENAME", "--filename=FILENAME", String, "Which filename to use?")
end.parse!(into: options)

rule_strings, words = IO.read(options[:filename]).split("\n\n")
rule_strings.split("\n").each do |line|
  break if line.strip == ''
  id, rule = line.split(': ', 2)
  $rules[id.to_i] = rule
end

if options[:part] == 1
  puts words.split("\n").count{|w| w.match(get_pattern_zero)}
elsif options[:part] == 2
  # do part 2
  $rules[8] = "42 | 42 8"
  $rules[11] = "42 31 | 42 11 31"
  $pattern_cache = {}
  puts words.split("\n").count{|w| w.match(get_pattern_zero)}
end
