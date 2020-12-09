#!/usr/bin/ruby

module HandheldProgram
  class Instruction
    def initialize(instruction, value)
      case instruction
      when "acc", "jmp", "nop"
        @instruction = instruction
      else
        raise ArgumentError.new("Unrecognized instruction")
      end

      @value = value
    end

    def instruction
      @instruction
    end

    def value
      @value
    end
  end
end
