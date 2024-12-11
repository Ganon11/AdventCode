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
  end
end
