#!/usr/bin/ruby

require 'optparse'

require 'handheld_program'

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
  program = HandheldProgram::Program.new(lines)
  program.execute
  puts program.accumulator
elsif options[:part] == 2
  lines.each_with_index do |line, index|
    program = HandheldProgram::Program.new(lines)
    i = program.instruction(index)
    new_instruction = nil
    if i.instruction == "acc"
      next
    elsif i.instruction == "jmp"
      new_instruction = HandheldProgram::Instruction.new("nop", i.value)
    elsif i.instruction == "nop"
      new_instruction = HandheldProgram::Instruction.new("jmp", i.value)
    end

    program.set_instruction(index, new_instruction)
    if program.execute
      puts "Fixed program, acc #{program.accumulator}"
      break
    end

    program.set_instruction(index, i)
  end
end
