#!/usr/bin/ruby

require 'optparse'

class Field
  def initialize(line)
    kvp = line.split(':')
    @name = kvp[0]
    @valid = []
    kvp[1].split(' or ').each do |range|
      s, e = range.split('-')
      @valid += (s.to_i..e.to_i).to_a
    end
  end

  def valid?(value)
    #puts "Checking #{value} against "
    return @valid.include?(value)
  end

  def name
    @name
  end

  def valid
    @valid
  end
end

class Ticket
  def initialize(line, fields)
    @values = line.split(',').map {|v| v.to_i}
    @invalid = nil
    @values.each do |value|
      if not fields.any? {|f| f.valid?(value)}
        @invalid = value
        break
      end
    end
  end

  def valid?
    return @invalid.nil?
  end

  def invalid_value
    return @invalid
  end

  def value_valid?(index, field)
    return field.valid?(@values[index])
  end

  def value(index)
    return @values[index]
  end
end

def part2(my_ticket, tickets, fields)
  # Find indices where fields are valid
  field_valid_indices = {}
  fields.each do |f|
    field_valid_indices[f.name] = []
    fields.each_index do |i|
      if tickets.all? {|t| t.value_valid?(i, f)}
        field_valid_indices[f.name].push(i)
      end
    end
  end

  # Fortunately, there's one field that only fits in one index. Find it, assign it, and remove that
  # index from other fields. Again, fortunately this process repeats until all fields have been
  # assigned.
  field_indices = {}
  while field_valid_indices.any? {|key, value| value.count > 1}
    k = field_valid_indices.keys.find {|key| field_valid_indices[key].count == 1}
    field_indices[k] = field_valid_indices[k][0]
    field_valid_indices.delete(k)
    field_valid_indices.each do |key, value|
      value.delete(field_indices[k])
      field_valid_indices[key] = value
    end
  end

  return field_indices.keys
    .select{|k| k.start_with?('departure')}
    .map{|k| my_ticket.value(field_indices[k])}
    .inject(&:*)
end

options = {
  :part => 1,
  :filename => "../input/sample.txt"
}

OptionParser.new do |opts|
  opts.banner = "Usage: Day16.rb [options]"
  opts.on("-p PART", "--part=PART", Integer, "Part 1 or 2")
  opts.on("-f FILENAME", "--filename=FILENAME", String, "Which filename to use?")
end.parse!(into: options)

fields = []
my_ticket = nil
nearby_tickets = []
File.open(options[:filename], 'r') do |fh|
  line = fh.readline
  while line != ''
    fields.push(Field::new(line))
    line = fh.readline.chomp
  end

  line = fh.readline.chomp
  line = fh.readline.chomp
  my_ticket = Ticket::new(line, fields)

  line = fh.readline.chomp
  line = fh.readline.chomp
  line = fh.readline.chomp
  while line != ''
    nearby_tickets.push(Ticket::new(line, fields))
    if fh.eof?
      break
    end
    line = fh.readline.chomp
  end
end

valid, invalid = nearby_tickets.partition {|t| t.valid?}

if options[:part] == 1
  puts invalid.map{|t| t.invalid_value}.sum
elsif options[:part] == 2
  puts part2(my_ticket, valid, fields)
end
