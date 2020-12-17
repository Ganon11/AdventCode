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
    return @valid.include?(value)
  end

  def name
    @name
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

  def invalid_value
    return @invalid
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
      if tickets.all? {|t| f.valid?(t.value(i))}
        field_valid_indices[f.name].push(i)
      end
    end
  end

  # Fortunately, there's one field that only fits in one index. Find it, assign it, and remove that
  # index from other fields. Again, fortunately this process repeats until all fields have been
  # assigned.
  field_indices = {}
  k = field_valid_indices.keys.find {|key| field_valid_indices[key].count == 1}
  while not k.nil?
    field_indices[k] = field_valid_indices[k][0]
    field_valid_indices.delete(k)
    field_valid_indices.each do |key, value|
      value.delete(field_indices[k])
    end

    k = field_valid_indices.keys.find {|key| field_valid_indices[key].count == 1}
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

data = IO.read(options[:filename]).split("\n\n")
fields = data[0].split("\n").map{|l| Field::new(l)}
my_ticket = Ticket::new(data[1].split("\n")[1], fields)
nearby_tickets = data[2].split("\n").drop(1).map{|l| Ticket::new(l, fields)}

valid, invalid = nearby_tickets.partition {|t| t.invalid_value.nil?}

if options[:part] == 1
  puts invalid.map{|t| t.invalid_value}.sum
elsif options[:part] == 2
  puts part2(my_ticket, valid, fields)
end
