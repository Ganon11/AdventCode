require 'minitest/autorun'
require 'handheld_program'

class ProgramTest < Minitest::Test
  def test_empty_program
    p = HandheldProgram::Program.new([])
    assert_equal(0, p.accumulator)
    assert_equal(0, p.program_size)

    assert_nil(p.instruction(0))
    assert_nil(p.instruction(1))
    assert_nil(p.instruction(2))

    i = HandheldProgram::Instruction.new("jmp", 0)
    assert_nil(p.set_instruction(0, i))
    assert_nil(p.set_instruction(1, i))
    assert_nil(p.set_instruction(2, i))

    assert(p.execute)
  end

  def test_simple_program
    p = HandheldProgram::Program.new(["jmp +1"])
    assert_equal(0, p.accumulator)
    assert_equal(1, p.program_size)

    old_i = p.instruction(0)
    assert_equal("jmp", old_i.instruction)
    assert_equal(1, old_i.value)

    assert_nil(p.instruction(1))
    assert_nil(p.instruction(2))

    new_i = HandheldProgram::Instruction.new("nop", 0)
    p.set_instruction(0, new_i)
    test_i = p.instruction(0)
    assert_equal("nop", test_i.instruction)
    assert_equal(0, test_i.value)

    assert_nil(p.set_instruction(1, new_i))
    assert_nil(p.set_instruction(2, new_i))

    p.set_instruction(0, old_i)
    assert(p.execute)
  end

  def test_acc
    p = HandheldProgram::Program.new(["acc +1", "jmp +1"])
    assert_equal(0, p.accumulator)
    assert_equal(2, p.program_size)

    first = p.instruction(0)
    assert_equal("acc", first.instruction)
    assert_equal(1, first.value)

    second = p.instruction(1)
    assert_equal("jmp", second.instruction)
    assert_equal(1, second.value)

    assert(p.execute)
    assert_equal(1, p.accumulator)
  end

  def test_nop
    p = HandheldProgram::Program.new(["nop +1", "jmp +1"])
    assert_equal(0, p.accumulator)
    assert_equal(2, p.program_size)

    first = p.instruction(0)
    assert_equal("nop", first.instruction)
    assert_equal(1, first.value)

    second = p.instruction(1)
    assert_equal("jmp", second.instruction)
    assert_equal(1, second.value)

    assert(p.execute)
    assert_equal(0, p.accumulator)
  end

  def test_jmp
    p = HandheldProgram::Program.new(["jmp +2", "acc +10", "acc +1", "jmp +1"])
    assert_equal(0, p.accumulator)
    assert_equal(4, p.program_size)

    first = p.instruction(0)
    assert_equal("jmp", first.instruction)
    assert_equal(2, first.value)

    second = p.instruction(1)
    assert_equal("acc", second.instruction)
    assert_equal(10, second.value)

    third = p.instruction(2)
    assert_equal("acc", third.instruction)
    assert_equal(1, third.value)

    fourth = p.instruction(3)
    assert_equal("jmp", fourth.instruction)
    assert_equal(1, fourth.value)

    assert(p.execute)
    assert_equal(1, p.accumulator)
  end

  def test_negative_jmp
    p = HandheldProgram::Program.new(["nop +1", "jmp -1"])
    assert_equal(0, p.accumulator)
    assert_equal(2, p.program_size)

    first = p.instruction(0)
    assert_equal("nop", first.instruction)
    assert_equal(1, first.value)

    second = p.instruction(1)
    assert_equal("jmp", second.instruction)
    assert_equal(-1, second.value)

    refute(p.execute)
    assert_equal(0, p.accumulator)
  end
end
