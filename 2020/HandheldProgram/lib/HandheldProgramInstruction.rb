#!/usr/bin/ruby

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
