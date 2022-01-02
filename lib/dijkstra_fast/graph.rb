module DijkstraFast
  ##
  # Dijkstra's algorithm finds the shortest "distance" between two items within a
  # collection.  For any two items within that collection that are "connected"
  # there should be an associated "distance" between them.  We can represent this
  # collection of items as nodes in a directed graph, and we can represent the
  # associated connections as weighted edges.
  #
  # = Example
  #
  #  graph = DijkstraFast::Graph.new
  #  graph.add("A", "B", distance: 5)
  #  graph.add("A", "C", distance: 8)
  #  graph.add("B", "C", distance: 2)
  #  best_path = graph.shortest_path("A", "C")
  #  best_path.path
  #
  #  => ["A", "B", "C"]
  #
  #  best_path.distance
  #
  #  => 7
  #
  ##
  class Graph

    include DijkstraFast::ShortestPath

    def initialize
      @edges = {}
    end

    # Adds a weighted edge to the graph. This represents a possible path from the
    # source item to the destination item with corresponding "distance."
    # @param source [Object] Any Ruby object that represents the source item
    # @param dest [Object] Any Ruby object that represents the destination item
    # @param distance [Integer] Optional distance between source and destination.
    #                           If not provided, a default distance of `1` is used.
    # @return [nil]
    def add(source, dest, distance: 1)
      return if source == dest

      @edges[source] ||= []
      @edges[source] << [dest, distance]
    end

    def connections(source, &block)
      @edges[source]&.each(&block)
      nil
    end

  end
end
