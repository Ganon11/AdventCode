require 'optparse'
require 'sorbet-runtime'

# typed: true
extend T::Sig

class Memory
  extend T::Sig

  class Block
    extend T::Sig

    class Type < T::Enum
      enums do
        File = new
        Empty = new
      end
    end

    sig {returns(Type)}
    attr_accessor :type

    sig {returns(Integer)}
    attr_accessor :size

    sig {returns(Integer)}
    attr_accessor :file_id

    sig {params(type: Type, size: Integer, file_id: Integer).void}
    def initialize(type, size, file_id = -1)
      @type = type
      @size = size
      @file_id = file_id
    end

    sig {params(other: Block).void}
    def swap(other)
      tmp = other.type
      other.type = @type
      @type = tmp

      tmp = other.size
      other.size = @size
      @size = tmp

      tmp = other.file_id
      other.file_id = @file_id
      @file_id = tmp
    end

    sig {returns(String)}
    def to_s
      case @type
      when Type::Empty then return "[Free, #{@size} blocks]"
      when Type::File then return "[Id: #{@file_id}, #{@size} blocks]"
      else T.absurd(@type)
      end
    end
  end

  sig {returns(T::Array[Block])}
  attr_reader :memory

  sig {params(disk_map: String).void}
  def initialize(disk_map)
    @memory = T.let([], T::Array[Block])
    disk_map.chars.each_with_index do |item, index|
      if index % 2 == 0 # File
        @memory << Block.new(Block::Type::File, item.to_i, index / 2)
      else # Free space
        @memory << Block.new(Block::Type::Empty, item.to_i)
      end
    end
  end

  sig {params(index: Integer).returns(T.nilable(Block))}
  def [](index)
    @memory[index]
  end

  sig {params(index: Integer, value: Block).void}
  def []=(index, value)
    @memory[index] = value
  end

  sig {returns(T.nilable(Integer))}
  def index_of_contiguous_free_space
    @memory.each_index do |index|
      next if index == @memory.length - 1
      return index if T.must(@memory[index]).type == Block::Type::Empty && T.must(@memory[index + 1]).type == Block::Type::Empty
    end

    nil
  end

  sig {void}
  def collapse_free
    while index = index_of_contiguous_free_space
      T.must(@memory[index]).size += T.must(@memory[index + 1]).size
      @memory.delete_at(index + 1)
    end
  end

  sig {returns(T::Array[Integer])}
  def flatten
    flattened = Array.new([])
    @memory.each do |block|
      (1..block.size).each do |_|
        flattened << block.file_id
      end
    end

    flattened
  end

  sig {returns(String)}
  def to_s
    @memory.map(&:to_s).join(', ')
  end
end

sig {params(memory: T::Array[Integer]).returns(T::Array[Integer])}
def defrag(memory)
  first = T.must(memory.index(-1))
  last = memory.length - 1
  while memory[last] == -1 && last > 0
    last -= 1
  end
  while first < last
    tmp = memory[first]
    memory[first] = T.must(memory[last])
    memory[last] = T.must(tmp)

    first = T.must(memory.index(-1))
    while memory[last] == -1 && last > 0
      last -= 1
    end
  end

  return memory
end

sig {params(memory: Memory).returns(T::Array[Integer])}
def defrag_2(memory)
  max_file_id = memory.memory.map { |block| block.file_id }.max
  range = (max_file_id..0)
  (range.first).downto(range.last).each do |file_id|
    file_block_index = T.let(T.must(memory.memory.index { |block| block.file_id == file_id }), Integer)
    file_block = T.let(T.must(memory[file_block_index]), Memory::Block)
    free_block_index =  T.let(memory.memory.index { |block| block.type == Memory::Block::Type::Empty && block.size >= file_block.size }, T.nilable(Integer))
    next if free_block_index.nil?
    next if free_block_index >= file_block_index
    free_block =  T.let(T.must(memory[free_block_index]), Memory::Block)

    #puts "Swapping file_id #{file_id} #{file_block_index} and #{free_block_index}"
    if file_block.size == free_block.size
      # File block is replaced by free_block
      memory[file_block_index] = Memory::Block.new(Memory::Block::Type::Empty, free_block.size)

      # Free block is replaced by file_block
      memory[free_block_index] = Memory::Block.new(Memory::Block::Type::File, file_block.size, file_block.file_id)
    else
      # File block is replacedby free_block of file_block.size
      memory[file_block_index] = Memory::Block.new(Memory::Block::Type::Empty, file_block.size)

      # Free block is replaced by file_block and free block of size (previous - file.size)
      new_free_block = Memory::Block.new(Memory::Block::Type::Empty, free_block.size - file_block.size)
      memory[free_block_index] = new_free_block
      memory.memory.insert(free_block_index, file_block)
    end

    memory.collapse_free
  end

  memory.flatten
end

sig {params(memory: T::Array[Integer]).returns(Integer)}
def checksum(memory)
  sum = 0
  memory.each_with_index do |item, index|
    next if item == -1
    sum += item * index
  end

  sum
end

options = {
  :part => 1,
  :filename => 'input.txt'
}

OptionParser.new do |opts|
  opts.banner = 'Usage: d9.rb [options]'
  opts.on('-p PART', '--part=PART', Integer, 'Which part?')
  opts.on('-f FILENAME', '--filename=FILENAME', String, 'Which file to use?')
end.parse!(into: options)

disk_map = File.new(options[:filename]).readline.strip
memory = Memory.new(disk_map)

#puts memory

if options[:part] == 1
  defragged = defrag(memory.flatten)
else
  defragged = defrag_2(memory)
end

puts "checksum : #{checksum(defragged)}"
