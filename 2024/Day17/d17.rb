require 'optparse'
require 'sorbet-runtime'

# typed: true
extend T::Sig

class ThreeBitComputer
  extend T::Sig

  REGISTER_A_PATTERN = /^Register A: (-?\d+)$/
  REGISTER_B_PATTERN = /^Register B: (-?\d+)$/
  REGISTER_C_PATTERN = /^Register C: (-?\d+)$/
  PROGRAM_PATTERN = /^Program: ([0-9,]+)$/

  sig {returns(Integer)}
  attr_reader :A

  sig {returns(Integer)}
  attr_reader :B

  sig {returns(Integer)}
  attr_reader :C

  sig {returns(Integer)}
  attr_reader :ip

  sig {returns(T::Array[Integer])}
  attr_reader :program

  sig {returns(T::Array[Integer])}
  attr_reader :output

  sig {params(a: Integer, program: T::Array[Integer]).void}
  def initialize(a, program)
    @A = a
    @B = 0
    @C = 0
    @program = program.dup

    @ip = T.let(0, Integer)
    @output = T.let(Array.new, T::Array[Integer])
  end

  sig {void}
  def execute
    #puts to_s
    while @ip < @program.length
      opcode = T.must(@program[@ip])
      operand = T.must(@program[@ip + 1])
      @ip += 2 # possibly overridden by jnz

      case opcode
      when 0 # adv
        numerator = @A
        denominator = 2 ** _combo_operand(operand)
        @A = (numerator / denominator).to_i
      when 1 # bxl
        @B = @B ^ operand
      when 2 # bst
        @B = _combo_operand(operand) % 8
      when 3 # jnz
        @ip = operand if @A != 0
      when 4 # bxc
        @B = @B ^ @C
      when 5 # out
        @output << _combo_operand(operand) % 8
        #puts "  Outputting #{_combo_operand(operand) % 8}"
      when 6 # bdv
        numerator = @A
        denominator = 2 ** _combo_operand(operand)
        @B = (numerator / denominator).to_i
      when 7 # cdv
        numerator = @A
        denominator = 2 ** _combo_operand(operand)
        @C = (numerator / denominator).to_i
      end

      #puts to_s
    end
  end

  sig {params(operand: Integer).returns(Integer)}
  def _combo_operand(operand)
    case operand
    when 0..3
      return operand
    when 4
      return @A
    when 5
      return @B
    when 6
      return @C
    when 7
      raise Exception.new "Reserved combo operand #{operand}"
    else
      raise Exception.new "Invalid combo operand #{operand}"
    end
  end

  sig {returns(String)}
  def to_s
    "A: #{@A}, B: #{@B}, C: #{@C}, IP: #{@ip}"
  end
end

sig {params(program: T::Array[Integer], index: Integer).returns(Integer)}
def calculate_digit(program, index)
  puts "Target digit: #{program[index]}"
  (1..7).each do |a|
    computer = ThreeBitComputer.new(a, program)
    computer.execute
    puts "Output if a = #{a}: #{computer.output}"
    return a if computer.output[0] == program[index]
  end

  return -1
end

def one_iteration(a)
  b = (a % 8) ^ 5
  c = (a / (2**a)).to_i
  d = b ^ c ^ c
  return d % 8
end

def figure_it_out(program, index = program.length - 1, a = 0)
  puts "Index: #{index}, a: #{a}"
  if index < 0
    return a
  end

  next_reg_a_mod_8 = a * 8
  (next_reg_a_mod_8..next_reg_a_mod_8 + 7).each do |next_a|
    return figure_it_out(program, index - 1, next_a) if one_iteration(a) == program[index]
  end
end

options = {
  :filename => 'input.txt',
  :a => nil,
  :program => nil
}

OptionParser.new do |opts|
  opts.banner = 'Usage: d17.rb [options]'
  opts.on('-a A', Integer, 'A register value?')
  opts.on('-p PROGRAM', '--program=PROGRAM', String, 'Program to run?')
end.parse!(into: options)

program = options[:program].split(',').map(&:to_i)
computer = ThreeBitComputer.new(options[:a], program)
computer.execute
puts "First Output: #{computer.output.join(',')}"

puts "Program: #{program}"
puts "Answer: #{figure_it_out(program)}"
# digit = calculate_digit(program, 2)
# puts "First digit: #{digit}"
