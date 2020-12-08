#!/usr/bin/ruby

require 'optparse'

class HandheldProgramInstruction
  def initialize(command, number)
    @command = command
    @number = number
  end

  def command
    @command
  end

  def number
    @number
  end
end

class HandheldProgram
  def initialize(lines)
    @commands = []
    lines.each do |line|
      line.strip!
      parts = line.split(' ')
      i = HandheldProgramInstruction.new(parts[0], parts[1].to_i)
      @commands.push(i)
    end
  end

  def ip
    @ip
  end

  def commands
    @commands
  end

  def execute
    ip = 0
    @accumulator = 0
    instructions_executed = []
    while true
      if instructions_executed.include?(ip)
        return false
      elsif ip >= @commands.length
        return true
      end

      instructions_executed.push(ip)

      i = @commands[ip]
      case i.command
      when "nop"
        ip += 1
      when "acc"
        @accumulator += i.number
        ip += 1
      when "jmp"
        ip += i.number
      else
        puts "Unknown command  #{i.command}"
        break
      end
    end
  end

  def accumulator
    @accumulator
  end

  def program_size
    @commands.length
  end

  def command(index)
    if index < 0 or @commands.length <= index
      puts "Index out of bounds"
      return nil
    end

    return @commands[index]
  end

  def set_command(index, instruction)
    if index < 0 or @commands.length <= index
      puts "Index out of bounds"
      return nil
    end

    @commands[index] = instruction
  end
end

options = {
  :part => 1,
  :filename => "../input/sample.txt"
}

OptionParser.new do |opts|
  opts.banner = "Usage: {daystr}.rb [options]"
  opts.on("-p PART", "--part=PART", Integer, "Part 1 or 2")
  opts.on("-f FILENAME", "--filename=FILENAME", String, "Which filename to use?")
end.parse!(into: options)

lines = IO.readlines(options[:filename])

if options[:part] == 1
  program = HandheldProgram.new(lines)
  program.execute
  puts program.accumulator
elsif options[:part] == 2
  lines.each_with_index do |line, index|
    program = HandheldProgram.new(lines)
    instruction = program.command(index)
    new_instruction = nil
    if instruction.command == "acc"
      next
    elsif instruction.command == "jmp"
      new_instruction = HandheldProgramInstruction.new("nop", instruction.number)
    elsif instruction.command == "nop"
      new_instruction = HandheldProgramInstruction.new("jmp", instruction.number)
    end

    program.set_command(index, new_instruction)
    if program.execute
      puts "Fixed program, acc #{program.accumulator}"
      break
    end

    program.set_command(index, instruction)
  end
end
