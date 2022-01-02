module DijkstraFast
  ##
  # Interface to allow arbitrary classes to calculate the shortest path between
  # two "items" using a native implementation of Dijkstra's algorithm.
  #
  # @example
  #
  #   class GraphOne
  #     include DijkstraFast::ShortestPath
  #
  #     def connections(source)
  #       source.find_next do |dest, distance|
  #         yield dest, distance
  #       end
  #     end
  #   end
  #
  #   distance, path = GraphOne.new.shortest_path(source, dest)
  #
  # @example
  #
  #   class GraphTwo
  #     include DijkstraFast::ShortestPath
  #
  #     def initialize
  #       # All edges have distance 1
  #       @edges = { 1 => [2, 3], 2 => [3] }
  #     end
  #
  #     def connections(source, &block)
  #       @edges.fetch(source, []).each(&block)
  #     end
  #   end
  #
  #   distance = GraphTwo.new.shortest_distance(source, dest)
  #
  # @example
  #
  #   class GraphThree
  #     include DijkstraFast::ShortestPath
  #
  #     def connections(source)
  #       case source
  #       when 'A'
  #         yield 'B', 3
  #         yield 'C' # Default distance 1
  #       when 'B'
  #         yield 'C', 10
  #       end
  #     end
  #   end
  #
  #   distance, path = GraphThree.new.shortest_path(source, dest)
  #
  # @see README
  # @see https://en.wikipedia.org/wiki/Dijkstra's_algorithm
  ##
  module ShortestPath

    # Returns the edges originating at source
    # @param source [Object] Any Ruby object that represents the source item
    # @yield [Object, int] A connection to destination object with given distance
    # @return [nil]
    def connections(source)
      # Does nothing by default but should be implemented by class
    end

    # Finds the shortest path between items, returning both the path as well as
    # the total distance travelled.
    # @param source [Object] Any Ruby object that represents the source item
    # @param dest [Object] Any Ruby object that represents the destination item
    # @param progress [Boolean|String|Proc] If falsey (default), no progress will
    #        be displayed.  If a Proc is given, it will be yielded progress
    #        (completed / total) as progress is made.  If otherwise truthy the
    #        ruby-progressbar gem will be required and displayed; if passed a
    #        String it will be used as the progress bar's format.
    # @return [Array<int, Array>]
    def shortest_path(source, dest, progress: false)
      Native.new(self).send(:shortest_path, source, dest, progress: progress)
    end

    # Finds the shortest distance between items
    # @param source [Object] Any Ruby object that represents the source item
    # @param dest [Object] Any Ruby object that represents the destination item
    # @param progress [Boolean|String|Proc] If falsey (default), no progress will
    #        be displayed.  If a Proc is given, it will be yielded progress
    #        (completed / total) as progress is made.  If otherwise truthy the
    #        ruby-progressbar gem will be required and displayed; if passed a
    #        String it will be used as the progress bar's format.
    # @return [int]
    def shortest_distance(source, dest, progress: false)
      shortest_path(source, dest, progress: progress).first
    end

  end
end
