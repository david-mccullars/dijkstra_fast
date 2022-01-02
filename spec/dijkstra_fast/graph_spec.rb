require 'spec_helper'

describe DijkstraFast::Graph do
  subject do
    DijkstraFast::Graph.new
  end

  specify 'other' do
    subject.add('A', 'B', distance: 5)
    subject.add('A', 'C', distance: 8)
    subject.add('B', 'C', distance: 2)
    distance, path = subject.shortest_path('A', 'C')
    expect(path).to eq(%w[A B C])
    expect(distance).to eq(7)
  end

  context :integers do
    before do
      [
        [1, 2, 10],
        [2, 4, 15],
        [1, 3, 20],
        [3, 4, 30],
        [2, 5, 50],
        [4, 5, 30],
        [2, 3, 40],
        [4, 6, 25],
        [5, 6, 15],
        [4, 7, 50],
        [3, 8, 70],
        [5, 8, 25],
      ].each do |x, y, w|
        subject.add(x, y, distance: w)
      end
    end

    specify '1 to 4' do
      distance, path = subject.shortest_path(1, 4)
      expect(distance).to eq(25)
      expect(path).to eq([1, 2, 4])
    end

    specify '1 to 5' do
      distance, path = subject.shortest_path(1, 5)
      expect(distance).to eq(55)
      expect(path).to eq([1, 2, 4, 5])
    end

    specify '2 to 5' do
      distance, path = subject.shortest_path(2, 5)
      expect(distance).to eq(45)
      expect(path).to eq([2, 4, 5])
    end

    specify '1 to 8' do
      distance, path = subject.shortest_path(1, 8)
      expect(distance).to eq(80)
      expect(path).to eq([1, 2, 4, 5, 8])
    end

    specify '1 to 1' do
      distance, path = subject.shortest_path(1, 1)
      expect(distance).to eq(0)
      expect(path).to eq([1])
    end
  end

  context :strings do
    before do
      [
        ['A', 'B', 10],
        ['B', 'D', 15],
        ['A', 'C', 20],
        ['C', 'D', 30],
        ['B', 'E', 50],
        ['D', 'E', 30],
        ['B', 'C', 40],
        ['D', 'F', 25],
        ['E', 'F', 15],
        ['D', 'G', 50],
        ['C', 'H', 70],
        ['E', 'H', 25],
      ].each do |x, y, w|
        subject.add(x, y, distance: w)
      end
    end

    specify "'A' to 'D'" do
      distance, path = subject.shortest_path('A', 'D')
      expect(distance).to eq(25)
      expect(path).to eq(%w[A B D])
    end

    specify "'A' to 'E'" do
      distance, path = subject.shortest_path('A', 'E')
      expect(distance).to eq(55)
      expect(path).to eq(%w[A B D E])
    end

    specify "'B' to 'E'" do
      distance, path = subject.shortest_path('B', 'E')
      expect(distance).to eq(45)
      expect(path).to eq(%w[B D E])
    end

    specify "'A' to 'H'" do
      distance, path = subject.shortest_path('A', 'H')
      expect(distance).to eq(80)
      expect(path).to eq(%w[A B D E H])
    end

    specify "'A' to 'A'" do
      distance, path = subject.shortest_path('A', 'A')
      expect(distance).to eq(0)
      expect(path).to eq(%w[A])
    end
  end

  context :errors do
    specify 'no such destination' do
      subject.add('A', 'B', distance: 5)
      subject.add('A', 'C', distance: 8)
      subject.add('B', 'C', distance: 2)
      expect { subject.shortest_path('A', 'Z') }.to raise_error(DijkstraFast::NoPathExistsError)
    end

    specify 'no such source' do
      subject.add('A', 'B', distance: 5)
      subject.add('A', 'C', distance: 8)
      subject.add('B', 'C', distance: 2)
      expect { subject.shortest_path('Z', 'C') }.to raise_error(DijkstraFast::NoPathExistsError)
    end

    specify 'no path between source and destination' do
      subject.add(5, 8, distance: 5)
      subject.add(8, 5, distance: 8)
      subject.add(13, 8, distance: 2)
      expect { subject.shortest_path(5, 13).tap { |x| p x } }.to raise_error(DijkstraFast::NoPathExistsError)
    end
  end
end
