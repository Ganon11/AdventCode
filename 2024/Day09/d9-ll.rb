require 'linked_list'
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

  sig {returns(LinkedList::LinkedList[Block])}
  attr_reader :memory

  sig {params(disk_map: String).void}
  def initialize(disk_map)
    @memory = T.let(LinkedList::LinkedList.new, LinkedList::LinkedList[Block])
    disk_map.chars.each_with_index do |item, index|
      if index % 2 == 0 # File
        @memory.append(Block.new(Block::Type::File, item.to_i, index / 2))
      else # Free space
        @memory.append(Block.new(Block::Type::Empty, item.to_i))
      end
    end
  end

  sig {void}
  def collapse_free
    current = @memory.head
    while current
      #puts "Looking at #{current}"
      if current.value.type == Block::Type::Empty && !current.next.nil? && T.must(current.next).value.type == Block::Type::Empty
        neighbor = T.must(current.next)
        current.value.size += neighbor.value.size
        current.next = neighbor.next
        if neighbor.next
          neighbor.next.prev = current
        end
      else
        current = current.next
      end
    end
  end

  sig {returns(T::Array[Integer])}
  def flatten
    return Array.new if @memory.length == 0
    node = T.let(T.must(@memory.head), T.nilable(LinkedList::Node[Memory::Block]))
    flattened = T.let(Array.new, T::Array[Integer])
    while node
      (1..node.value.size).each do |_|
        flattened << node.value.file_id
      end
      node = node.next
    end

    return flattened
  end

  sig {returns(String)}
  def to_s
    return "" if @memory.length == 0
    node = T.let(T.must(@memory.head), T.nilable(LinkedList::Node[Block]))
    strings = [T.must(node).value.to_s]
    while (node = T.must(node).next)
      strings << node.value.to_s
    end
    strings.join(', ')
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
  max_file_id = memory.flatten.max
  range = (max_file_id..0)
  (range.first).downto(range.last).each do |file_id|
    file_block_node = memory.memory.find_by_from_tail { |node| node.value.file_id == file_id }
    next if file_block_node.nil?
    file_block = file_block_node.value
    free_block_node = memory.memory.find_by do |node|
      T.must(node.value).type == Memory::Block::Type::Empty && T.must(node.value).size >= file_block.size
    end
    next if free_block_node.nil?
    free_block = free_block_node.value

    next if !memory.memory.after(free_block_node, file_block_node)

    if file_block.size == free_block.size
      # File block is replaced by free_block
      file_block_node.value = Memory::Block.new(Memory::Block::Type::Empty, free_block.size)

      # Free block is replaced by file_block
      free_block_node.value = Memory::Block.new(Memory::Block::Type::File, file_block.size, file_block.file_id)
    else
      # File block is replaced by free_block of file_block.size
      tmp = file_block_node.value
      file_block_node.value = Memory::Block.new(Memory::Block::Type::Empty, file_block.size)

      # Free block is replaced by file_block and free block of size (previous - file.size)
      free_block_node.value = tmp
      new_free_block = Memory::Block.new(Memory::Block::Type::Empty, free_block.size - file_block.size)
      memory.memory.append_after(free_block_node, new_free_block)
    end

    #memory.collapse_free
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
  opts.banner = 'Usage: d9-ll.rb [options]'
  opts.on('-p PART', '--part=PART', Integer, 'Which part?')
  opts.on('-f FILENAME', '--filename=FILENAME', String, 'Which file to use?')
end.parse!(into: options)

disk_map = File.new(options[:filename]).readline.strip
#puts disk_map
memory = Memory.new(disk_map)

if options[:part] == 1
  defragged = defrag(memory.flatten)
else
  defragged = defrag_2(memory)
end

puts "checksum: #{checksum(defragged)}"
