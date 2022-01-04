require 'spec_helper'

describe DijkstraFast do
  let(:a) { AbcNode.new('A') }
  let(:b) { AbcNode.new('B') }
  let(:c) { AbcNode.new('C') }

  before do
    AbcNode = Struct.new(:label) do
      def connections
        case label
        when "A"
          yield AbcNode.new("B"), 5
          yield AbcNode.new("C"), 8
        when "B"
          yield AbcNode.new("C"), 2
        end
      end
    end unless defined?(AbcNode)
  end

  specify :shortest_path do
    distance, path = DijkstraFast.shortest_path(a, c)
    expect(path).to eq([a, b, c])
    expect(distance).to eq(7)
  end

  specify :shortest_distance do
    distance = DijkstraFast.shortest_distance(a, c)
    expect(distance).to eq(7)
  end

  context :progress do
    let(:progress_bar) { ProgressBar.new }
    let(:config) { { format: '%a %bᗧ%i %p%% %t', progress_mark: ' ', remainder_mark: '･', total: 100, autofinish: false } }

    before do
      # Dummy version instead of from gem
      class ProgressBar
        def clear; end
        def total=(v); end
        def progress=(v); end
      end
    end

    specify :true do
      expect(ProgressBar).to receive(:create).with(config).and_return(progress_bar)
      [[1, 3], [2, 4]].each do |p, t|
        expect(progress_bar).to receive(:progress=).with(p)
        expect(progress_bar).to receive(:total=).with(t)
      end
      expect(progress_bar).to receive(:clear)
      DijkstraFast.shortest_distance(a, c, progress: true)
    end

    specify :hash do
      alt_config = config.merge(format: 'ABC', progress_mark: 'X!')
      expect(ProgressBar).to receive(:create).with(alt_config).and_return(progress_bar)
      DijkstraFast.shortest_distance(a, c, progress: alt_config)
    end

    specify :string do
      alt_config = config.merge(format: 'ABC-123')
      expect(ProgressBar).to receive(:create).with(alt_config).and_return(progress_bar)
      DijkstraFast.shortest_distance(a, c, progress: 'ABC-123')
    end

    specify :proc do
      yielded_progress = []
      progress_proc = ->(completed, total) { yielded_progress << [completed, total] }
      DijkstraFast.shortest_distance(a, c, progress: progress_proc)
      expect(yielded_progress).to eq([[1, 3], [2, 4]])
    end
  end

  specify :progress_load_error do
    expect { DijkstraFast.shortest_distance(a, c, progress: true) }.to raise_error(LoadError)
  end
end
