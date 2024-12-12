# frozen_string_literal: true
# typed: true

require_relative "linked_list/version"
require 'sorbet-runtime'

module LinkedList
  class Node
    extend T::Sig
    extend T::Generic

    Elem = type_member

    sig {returns(T.nilable(Node[Elem]))}
    attr_accessor :next

    sig {returns(T.nilable(Node[Elem]))}
    attr_accessor :prev

    sig {returns(Elem)}
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
    extend T::Generic

    Elem = type_member

    sig {returns(T.nilable(Node[Elem]))}
    attr_accessor :head

    sig {void}
    def initialize
      @head = T.let(nil, T.nilable(Node[Elem]))
      @tail = T.let(nil, T.nilable(Node[Elem]))
    end

    sig {params(value: Elem).void}
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

    sig {params(target: Node[Elem], value: Elem).void}
    def append_after(target, value)
      if target == @tail
        append(value)
      else
        new_node = Node.new(value)
        new_node.prev = target
        new_node.next = target.next
        T.must(new_node.next).prev = new_node
        target.next = new_node
      end
    end

    sig {params(first: Node[Elem], second: Node[Elem]).returns(T::Boolean)}
    def after(first, second)
      return false if !@head
      return false if first == second
      seen_first = T.let(false, T::Boolean)
      seen_second = T.let(false, T::Boolean)
      node = @head
      while node
        if node == first
          seen_first = T.let(true, T::Boolean)
          return false if seen_second
        elsif node == second
          seen_second = T.let(true, T::Boolean)
          return true if seen_first
        end

        node = node.next
      end

      return false
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

    sig {params(condition: T.proc.params(node: Node[Elem]).returns(T::Boolean)).returns(T.nilable(Node[Elem]))}
    def find_by(&condition)
      return nil if !@head
      node = @head
      while node
        if yield(node)
          return node
        end

        node = node.next
      end

      nil
    end

    sig {params(condition: T.proc.params(node: Node[Elem]).returns(T::Boolean)).returns(T.nilable(Node[Elem]))}
    def find_by_from_tail(&condition)
      return nil if !@tail
      node = @tail
      while node
        if yield(node)
          return node
        end

        node = node.prev
      end

      nil
    end

    sig {returns(String)}
    def to_s
      return "Empty" if !@head
      strings = T.let([], T::Array[String])
      node = @head
      while node
        strings << node.to_s
        node = node.next
      end

      strings.join(', ')
    end
  end
end
