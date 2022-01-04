require 'spec_helper'

describe DijkstraFast::ShortestPath do
  subject do
    Class.new do
      include DijkstraFast::ShortestPath

      def connections(source)
        case source
        when 'A'
          yield 'B', 5
          yield 'C', 8
        when 'B'
          yield 'C', 2
        when /[C-Y]/
          yield (source.ord + 1).chr
        end
      end
    end.new
  end

  specify :shortest_path do
    distance, path = subject.shortest_path('A', 'C')
    expect(path).to eq(%w[A B C])
    expect(distance).to eq(7)
  end

  specify :shortest_distance do
    distance = subject.shortest_distance('A', 'C')
    expect(distance).to eq(7)
  end
end
