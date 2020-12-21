#!/usr/bin/ruby

require 'optparse'
require 'set'

def decode_allergen_map(allergen_sets)
  allergen_map = {}
  ingredients_assigned = []
  while not allergen_sets.empty?
    could_not_assign = {}
    allergen_sets.keys.each do |allergen|
      set = allergen_sets[allergen][0]
      allergen_sets[allergen].drop(1).each do |s|
        set &= s
      end

      set.keep_if {|i| not ingredients_assigned.include?(i)}
      if set.size != 1
        could_not_assign[allergen] = [set]
      else
        ingredient = set.to_a[0]
        allergen_map[ingredient] = allergen
        ingredients_assigned.push(ingredient)
      end
    end

    allergen_sets = could_not_assign.dup
  end

  return allergen_map
end

options = {
  :part => 1,
  :filename => "../input/sample.txt"
}

OptionParser.new do |opts|
  opts.banner = "Usage: Day21.rb [options]"
  opts.on("-p PART", "--part=PART", Integer, "Part 1 or 2")
  opts.on("-f FILENAME", "--filename=FILENAME", String, "Which filename to use?")
end.parse!(into: options)

ingredient_count = {}
allergen_sets = {}

IO.readlines(options[:filename]).each do |line|
  data = line.match(/\A(?<ingredient_list>.+) \(contains (?<allergen_list>.+)\)\Z/)
  if data
    ingredient_str = data[:ingredient_list]
    allergen_str = data[:allergen_list]

    ingredient_list = []
    ingredient_str.scan(/\w+/).each do |i|
      ingredient_list.push(i)
      if ingredient_count.key?(i)
        ingredient_count[i] += 1
      else
        ingredient_count[i] = 1
      end
    end

    allergen_str.scan(/\w+/).each do |a|
      if allergen_sets.key?(a)
        allergen_sets[a].push(ingredient_list.to_set)
      else
        allergen_sets[a] = [ingredient_list.to_set]
      end
    end
  else
    puts "Bad parse"
  end
end

decoded = decode_allergen_map(allergen_sets)

if options[:part] == 1
  if decoded.nil?
    puts "Figure it out"
  else
    sum = 0
    ingredient_count.keys.each do |ingredient|
      if not decoded.key?(ingredient)
        sum += ingredient_count[ingredient]
      end
    end

    puts sum
  end
elsif options[:part] == 2
  puts decoded.keys.sort{|a, b| decoded[a] <=> decoded[b]}.join(",")
end
