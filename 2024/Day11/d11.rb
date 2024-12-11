require 'linked_list'
require 'optparse'
require 'sorbet-runtime'

# typed: true
extend T::Sig

sig {params(list: LinkedList::LinkedList).returns(LinkedList::LinkedList)}
def blink(list)
  current = list.head
  new_list = LinkedList::LinkedList.new
  while current
    if current.value == 0
      stone_digits = 1
    else
      stone_digits = Math.log10(current.value).to_i + 1
    end

    if current.value == 0 # Rule 1: If the stone is engraved with 0, change value to 1
      new_list.append(1)
    elsif stone_digits % 2 == 0 # Rule 2: If the stone has an even number of digits, split into 2 stones
      power_of_ten = (10 ** (stone_digits / 2))
      new_list.append(current.value / power_of_ten)
      new_list.append(current.value % power_of_ten)
    else # Rule 3: Multiply the stone's value by 2024
      new_list.append(current.value * 2024)
    end

    current = current.next
  end

  return new_list
end

options = {
  :filename => 'input.txt'
}

OptionParser.new do |opts|
  opts.banner = 'Usage: d11.rb [options]'
  opts.on('-f FILENAME', '--filename=FILENAME', String, 'Which file to use?')
end.parse!(into: options)

stones = File.new(options[:filename]).readline.strip.split(' ').map(&:to_i)
list = LinkedList::LinkedList.new
stones.each do |stone|
  list.append(stone)
end

(1..25).each do |count|
  list = blink(list)
end
puts list.length