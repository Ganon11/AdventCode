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
    values = line.split(',').map {|v| v.to_i}
    @invalid = nil
    values.each do |value|
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
end

def part1(tickets)
  sum = 0
  tickets.each do |t|
    if not t.valid?
      sum += t.invalid_value
    end
  end

  return sum
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

# fields.each do |f|
#   puts "Field #{f.name}, values #{f.valid}"
# end

if options[:part] == 1
  puts part1(nearby_tickets)
elsif options[:part] == 2
  # do part 2
end
