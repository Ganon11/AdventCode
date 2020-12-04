#!/usr/bin/ruby

require 'optparse'

def validate_kvp?(key, value)
  case key
  when "byr"
    year = value.to_i
    if year < 1920 or 2002 < year
      return false
    end

    return true

  when "iyr"
    year = value.to_i
    if year < 2010 or 2020 < year
      return false
    end

    return true

  when "eyr"
    year = value.to_i
    if year < 2020 or year > 2030
      return false
    end

    return true

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
      if number < 59 or 76 < number
        return false
      end
    else
      if number < 150 or 193 < number
        return false
      end
    end

    return true

  when "hcl"
    if not value.match(/#[0-9a-f]{6}/)
      return false
    end

    return true

  when "ecl"
    return ["amb", "blu", "brn", "gry", "grn", "hzl", "oth"].include?(value)

  when "pid"
    if value.length != 9 or not value.match(/\d{9}/)
      return false
    end

    return true

  when "cid"
    return true

  end
end

def is_basically_valid?(passport)
  keys = passport.keys
  if keys.length == 8
    return true
  elsif keys.length == 7 and not keys.include?("cid")
    return true
  else
    return false
  end

end

def part1(passports)
  return passports.count do |p|
    is_basically_valid?(p)
  end
end

def part2(passports)
  return passports.count do |p|
    is_basically_valid?(p) and
      p.keys.all? {|k| validate_kvp?(k, p[k])}
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
