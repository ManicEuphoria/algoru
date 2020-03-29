# frozen_string_literal: true

require './lib/linked_list/data_structure/single_linked_list'

RSpec.describe Node do
  describe 'class Node.new' do
    it 'has a value and a link to next node' do
      node = Node.new(10)

      expect(node.value).to eq(10)
      expect(node.next).to eq(nil)
    end
  end
end
