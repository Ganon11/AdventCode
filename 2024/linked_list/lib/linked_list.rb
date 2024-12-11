# frozen_string_literal: true
# typed: true

require_relative "linked_list/version"
require 'sorbet-runtime'

module LinkedList
  class Node
    extend T::Sig

    sig {returns(T.nilable(Node))}
    attr_accessor :next

    sig {returns(T.nilable(Node))}
    attr_accessor :prev

    sig {returns(T.untyped)}
    attr_accessor :value

    sig {params(value: T.untyped).void}
    def initialize(value)
      @value = value
      @next = nil
      @prev = nil
    end

    sig {returns(String)}
    def to_s
      "Node{#{@value}}"
    end
  end

  class LinkedList
    extend T::Sig

    sig {returns(T.nilable(Node))}
    attr_accessor :head

    sig {void}
    def initialize
      @head = T.let(nil, T.nilable(Node))
      @tail = nil
    end

    sig {params(value: T.untyped).void}
    def append(value)
      new_node = Node.new(value)
      if @head
        T.must(@tail).next = new_node
        new_node.prev = @tail
        @tail = new_node
      else
        @head = new_node
        @tail = @head
      end
    end

    sig {params(target: Node, value: T.untyped).void}
    def append_after(target, value)
      target_was_tail = (target == @tail)
      new_node = Node.new(value)
      new_node.next = target.next
      new_node.prev = target
      T.must(target.next).prev = new_node if target.next
      target.next = new_node
      @tail = new_node if target_was_tail
    end

    # sig {returns(T.nilable(Node))}
    # def find_tail
    #   # return nil if !@head
    #   # node = @head
    #   # return node if !node.next
    #   # return node if !node.next while (node = node.next)
    # end

    sig {params(value: T.untyped).returns(T.nilable(Node))}
    def find(value)
      return nil if !@head
      node = @head
      return node if node.value == value
      while (node = node.next)
        return node if node.value == value
      end

      return nil
    end

    sig {returns(Integer)}
    def length
      return 0 if !@head
      length = 1
      node = @head
      while (node = node.next)
        length += 1
      end
      return length
    end

    sig {returns(String)}
    def to_s
      return "" if !@head
      node = @head
      description = ""
      while (node)
        description += node.to_s
        node = node.next
      end
      return description
    end
  end
end

# l = LinkedList::LinkedList.new
# l.append(1)
# l.append(2)
# l.append(3)

# n = l.find(2)

# puts "#{n}, next #{T.must(n).next}, prev #{T.must(n).prev}"

# l.append_after(T.must(n), 7)

# puts "#{n}, next #{T.must(n).next}, prev #{T.must(n).prev}"

# n = l.find_tail

# puts "#{n}, next #{T.must(n).next}, prev #{T.must(n).prev}"
