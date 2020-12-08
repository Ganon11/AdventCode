#!/usr/bin/ruby

require 'optparse'

require 'HandheldProgram'

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
