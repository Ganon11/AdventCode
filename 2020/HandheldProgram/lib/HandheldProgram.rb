#!/usr/bin/ruby

require 'HandheldProgram/Instruction'

module HandheldProgram
  class Program
    def initialize(lines)
      @commands = []
      lines.each do |line|
        line.strip!
        parts = line.split(' ')
        i = HandheldProgram::Instruction.new(parts[0], parts[1].to_i)
        @commands.push(i)
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
  end
end
