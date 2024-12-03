require 'optparse'

def find_distance(list1, list2)
  list1.sort!
  list2.sort!

  sum = 0
  list1.each_with_index do |_, index|
    sum += (list1[index] - list2[index]).abs
  end

  sum
end

def find_similarity(list1, list2)
  similarity = 0
  list1.each do |num1|
    similarity += (num1 * list2.count(num1))
  end

  similarity
end

options = {
  :part => 1,
  :filename => 'input.txt'
}

OptionParser.new do |opts|
  opts.banner = 'Usage: d1.rb [options]'
  opts.on('-p PART', '--part=PART', Integer, 'Part 1 or 2?')
  opts.on('-f FILENAME', '--filename=FILENAME', String, 'Which file to use?')
end.parse!(into: options)

location_ids = IO.readlines(options[:filename])
list1 = []
list2 = []
location_ids.each do |location_pair|
  ids = location_pair.split
  list1 << ids[0].to_i
  list2 << ids[1].to_i
end

if options[:part] == 1
  puts find_distance(list1, list2)
elsif options[:part] == 2
  puts find_similarity(list1, list2)
end
