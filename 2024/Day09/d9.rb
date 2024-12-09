require 'optparse'
require 'sorbet-runtime'

# typed: true
extend T::Sig

sig {params(memory: T::Array[Integer]).returns(T::Array[Integer])}
def defrag(memory)
  defragged = memory.dup
  first = T.must(defragged.index(-1))
  last = memory.length - 1
  while memory[last] == -1 && last > 0
    last -= 1
  end
  while first < last
    tmp = defragged[first]
    defragged[first] = T.must(defragged[last])
    defragged[last] = T.must(tmp)

    first = T.must(defragged.index(-1))
    while defragged[last] == -1 && last > 0
      last -= 1
    end
  end

  return defragged
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

memory = Array.new([])
disk_map.chars.each_with_index do |item, index|
  (1..item.to_i).each do |iteration|
    if index % 2 == 0 # File
      memory << index / 2
    else # Free space
      memory << -1
    end
  end
end

if options[:part] == 1
  defragged = defrag(memory)
else
  defragged = Array.new([])
end

puts "checksum : #{checksum(defragged)}"
