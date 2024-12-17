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

  sig {params(args: String).void}
  def initialize(*args)
    puts args.length
    if args.length == 2
      @A = T.let(args[0].to_i, Integer)
      @B = 0
      @C = 0
      @program = T.let(T.must(args[1]).split(',').map(&:to_i), T::Array[Integer])
    elsif args.length == 1
      if m = REGISTER_A_PATTERN.match(args[0])
        @A = T.let(m[1].to_i, Integer)
      end
      if m = REGISTER_B_PATTERN.match(args[1])
        @B = T.let(m[1].to_i, Integer)
      end
      if m = REGISTER_C_PATTERN.match(args[2])
        @C = T.let(m[1].to_i, Integer)
      end
      if m = PROGRAM_PATTERN.match(args[4])
        @program = T.let(T.must(m[1]).split(',').map(&:to_i), T::Array[Integer])
      end
    else
      raise Exception.new "Invalid input #{args}"
    end

    @ip = T.let(0, Integer)
    @output = T.let(Array.new, T::Array[Integer])
    @orig_a = @A
    @orig_b = @B
    @orig_c = @C
  end

  sig {void}
  def execute
    puts to_s
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
        puts "  Outputting #{_combo_operand(operand) % 8}"
      when 6 # bdv
        numerator = @A
        denominator = 2 ** _combo_operand(operand)
        @B = (numerator / denominator).to_i
      when 7 # cdv
        numerator = @A
        denominator = 2 ** _combo_operand(operand)
        @C = (numerator / denominator).to_i
      end

      puts to_s
    end
  end

  sig {params(new_a: Integer).void}
  def reset(new_a)
    @ip = 0
    @A = new_a
    @B = @orig_b
    @C = @orig_c
    @output.clear
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

options = {
  :filename => 'input.txt',
  :a => nil,
  :program => nil
}

OptionParser.new do |opts|
  opts.banner = 'Usage: d17.rb [options]'
  opts.on('-f FILENAME', '--filename=FILENAME', String, 'Which file to use?')
  opts.on('-a A', String, 'A register value?')
  opts.on('-p PROGRAM', '--program=PROGRAM', String, 'Program to run?')
end.parse!(into: options)

if !options[:a].nil? && !options[:program].nil?
  computer = ThreeBitComputer.new(options[:a], options[:program])
else
  computer = ThreeBitComputer.new(IO.readlines(options[:filename]).map(&:strip))
end

a = computer.A
# puts "Computer initialized:"
# puts "  A: #{computer.A}"
# puts "  B: #{computer.B}"
# puts "  C: #{computer.C}"
# puts "  Program: #{computer.program}"

computer.execute

puts "First Output: #{computer.output.join(',')}"

# while computer.output != computer.program
#   a += 1
#   computer.reset(a)
#   computer.execute
# end

# puts "Computer outputs itself at A = #{a}"
