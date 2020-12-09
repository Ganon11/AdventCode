require 'minitest/autorun'
require 'handheld_program'

class InstructionTest < Minitest::Test
  def test_acc_instruction
    i = HandheldProgram::Instruction.new("acc", 8)
    assert_equal("acc", i.instruction)
    assert_equal(8, i.value)
  end

  def test_jmp_instruction
    i = HandheldProgram::Instruction.new("jmp", 4)
    assert_equal("jmp", i.instruction)
    assert_equal(4, i.value)
  end

  def test_jmp_instruction_negative_value
    i = HandheldProgram::Instruction.new("jmp", -4)
    assert_equal("jmp", i.instruction)
    assert_equal(-4, i.value)
  end

  def test_nop_instruction
    i = HandheldProgram::Instruction.new("nop", 0)
    assert_equal("nop", i.instruction)
    assert_equal(0, i.value)
  end

  def test_bad_instruction
    assert_raises ArgumentError do
      HandheldProgram::Instruction.new("not_an_instruction", 0)
    end
  end
end
