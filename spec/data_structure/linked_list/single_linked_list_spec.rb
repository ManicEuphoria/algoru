# frozen_string_literal: true

require './lib/algoru/data_structure/linked_list/single_linked_list'

RSpec.describe Algoru::Node do
  describe 'class Node.new' do
    it 'has a value and a link to next node' do
      node = Algoru::Node.new(10)

      expect(node.value).to eq(10)
      expect(node.next).to eq(nil)
    end
  end
end
