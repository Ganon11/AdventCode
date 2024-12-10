require 'optparse'
require 'sorbet-runtime'

# typed: true
extend T::Sig

class Equation
  extend T::Sig

  sig {returns(Integer)}
  attr_reader :result

  sig {returns(T::Array[Integer])}
  attr_reader :values

  sig {params(line: String).void}
  def initialize(line)
    tokens = line.split(': ', 2)
    @result = tokens[0].to_i
    @values = T.must(tokens[1]).split(' ').map(&:to_i)
  end

  sig {params(current_value: Integer, index: Integer, use_concat: T::Boolean).returns(T::Boolean)}
  def valid_from_index?(current_value, index, use_concat)
    if index == @values.length
      return current_value == @result
    end

    valid_with_add = valid_from_index?(current_value + T.must(@values[index]), index + 1, use_concat)
    valid_with_multiply = valid_from_index?(current_value * T.must(@values[index]), index + 1, use_concat)
    if !use_concat
      return valid_with_add || valid_with_multiply
    end

    valid_with_concat = valid_from_index?("#{current_value}#{T.must(@values[index])}".to_i, index + 1, true)
    return valid_with_add || valid_with_multiply || valid_with_concat
  end

  sig {params(use_concat: T::Boolean).returns(T::Boolean)}
  def could_be_valid?(use_concat)
    return valid_from_index?(@values[0], 1, use_concat)
  end

  sig {returns(String)}
  def to_s
    "#{@result}: #{@values.join(' ')}"
  end

  private :valid_from_index?
end

options = {
  :part => 1,
  :filename => 'input.txt'
}

OptionParser.new do |opts|
  opts.banner = 'Usage: d7.rb [options]'
  opts.on('-p PART', '--part=PART', Integer, 'Which part?')
  opts.on('-f FILENAME', '--filename=FILENAME', String, 'Which file to use?')
end.parse!(into: options)

equations = IO.readlines(options[:filename]).map{ |line| Equation.new(line.strip) }
possibly_valid = equations.select { |eq| eq.could_be_valid?(options[:part] == 2) }
calibration_result = possibly_valid.map{ |eq| eq.result }.sum
puts "Calibration result: #{calibration_result}"
