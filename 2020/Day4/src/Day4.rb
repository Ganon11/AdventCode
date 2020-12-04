#!/usr/bin/ruby

require 'optparse'

REQUIRED_FIELDS = ["byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"]

def is_basically_valid?(passport)
  return REQUIRED_FIELDS.all? do |f|
    passport.key?(f)
  end
end

def part1(passports)
  return passports.count do |p|
    is_basically_valid?(p)
  end
end

VALID_EYE_COLORS = ["amb", "blu", "brn", "gry", "grn", "hzl", "oth"]

def validate_kvp?(key, value)
  case key
  when "byr"
    return value.to_i.between?(1920, 2002)
  when "iyr"
    return value.to_i.between?(2010, 2020)
  when "eyr"
    return value.to_i.between?(2020, 2030)
  when "hgt"
    data = value.match(/(?<number>\d+)(?<unit>in|cm)/)
    if not data
      return false
    end

    number = data[:number].to_i
    unit = data[:unit]
    if unit != 'in' and unit != 'cm'
      return false
    end

    if unit == 'in'
      return number.between?(59, 76)
    else
      return number.between?(150, 193)
    end

  when "hcl"
    return value.match(/#[0-9a-f]{6}/)

  when "ecl"
    return VALID_EYE_COLORS.include?(value)

  when "pid"
    return value.match(/\A\d{9}\z/)

  when "cid"
    return true

  end
end

def part2(passports)
  return passports.count do |p|
    is_basically_valid?(p) and
      p.all? {|k, v| validate_kvp?(k, v)}
  end
end

options = {
  :part => 1,
  :filename => "../input/sample.txt"
}

OptionParser.new do |opts|
  opts.banner = "Usage: Day4.rb [options]"
  opts.on("-p PART", "--part=PART", Integer, "Part 1 or 2")
  opts.on("-f FILENAME", "--filename=FILENAME", String, "Which filename to use?")
end.parse!(into: options)

passports = []
current_passport = {}
IO.readlines(options[:filename]).each do |line|
  line = line.strip
  if line == ''
    passports.push(current_passport)
    current_passport = {}
  else
    parts = line.split(' ')
    parts.each do |part|
      pair = part.split(':')
      current_passport[pair[0]] = pair[1]
    end
  end
end

if not current_passport.empty?
  passports.push(current_passport)
  current_passport = {}
end

if options[:part] == 1
  # do part 1
  puts part1(passports)
elsif options[:part] == 2
  # do part 2
  puts part2(passports)
end
