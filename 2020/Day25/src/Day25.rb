#!/usr/bin/ruby

require 'optparse'

$SUBJECT_NUMBER = 7
$MODULO_VALUE = 20201227

def transform_value(value, subject_number = $SUBJECT_NUMBER)
  return (value * subject_number) % $MODULO_VALUE
end

def find_loop_size(target, subject_number = $SUBJECT_NUMBER)
  count = 0
  value = 1
  while value != target
    count += 1
    value = transform_value(value, subject_number)
  end

  return count
end

def do_transform(loop_size, subject_number = $SUBJECT_NUMBER)
  value = 1
  loop_size.times do
    value = transform_value(value, subject_number)
  end

  return value
end

def get_encryption_key(card, door)
  door_loop_size = find_loop_size(door)
  answer = do_transform(door_loop_size, card)
  return answer
end

options = {
  :filename => "../input/sample.txt"
}

OptionParser.new do |opts|
  opts.banner = "Usage: Day25.rb [options]"
  opts.on("-f FILENAME", "--filename=FILENAME", String, "Which filename to use?")
end.parse!(into: options)

card, door = IO.readlines(options[:filename]).map{|l| l.strip.to_i}

puts get_encryption_key(card, door)
