# author: EUPHORAY

class Node
  attr_accessor :value, :next

  def initialize(value, next_node = nil)
    @value = value
    @next = next_node
  end
end


class LinkedList

	def initialize
		@head = nil
	end

	# append a node
	def append(value)
		if @head
			find_tail.next = Node.new(value)
		else
			@head = Node.new(value)
		end
	end

	# find tail node
	def find_tail
		node = @head
		return node if node.next.nil?
		return node if node.next.nil? while (node = node.next) # node = node.next means point current node address to the next node address
	end

	# find node which value equals target
	def find(target)
		node = @head
		return node if node.value == target while (node = node.next)
	end

	# find node before target node
	def find_before(target)
		node = @head

		return false if node.next.nil?
		return node if node.next.value == target

		while (node = node.next)
			return node if node.next && node.next.value == target
		end
	end

	# append node after target node
	def append_after(target, value)
		node = find(target)

		old_next = node.next
		node.next = Node.new(value)
		node.next.next = old_next
	end

	# append node before target node
	def append_before(target, value)
		node = find_before(target)

		old_next = node.next
		node.next = Node.new(value)
		node.next.next = old_next
	end

	# delete target node
	def delete(target)
		if @head.value == target
			@head = @head.next
			return
		end

		node = find_before(target)
		node.next = node.next.next
	end

	# print node by order
	def print
		node = @head
		puts node

		puts node while (node = node.next)
	end
end
