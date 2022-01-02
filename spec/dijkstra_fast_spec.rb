require 'spec_helper'

describe DijkstraFast do
  specify 'shortest_path' do
    Node = Struct.new(:label) do
      def connections
        case label
        when "A"
          yield Node.new("B"), 5
          yield Node.new("C"), 8
        when "B"
          yield Node.new("C"), 2
        end
      end
    end

    a = Node.new("A")
    b = Node.new("B")
    c = Node.new("C")

    distance, path = DijkstraFast.shortest_path(a, c)
    expect(path).to eq([a, b, c])
    expect(distance).to eq(7)
  end
end
