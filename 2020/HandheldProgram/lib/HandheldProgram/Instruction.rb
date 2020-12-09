#!/usr/bin/ruby

module HandheldProgram
  class Instruction
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
end
