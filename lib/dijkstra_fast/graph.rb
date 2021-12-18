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

    def initialize
      @nodes = {}
    end

    # Adds a weighted edge to the graph. This represents a possible path from the
    # source item to the destination item with corresponding "distance."
    # @param source [Object] Any Ruby object that represents the source item
    # @param dest [Object] Any Ruby object that represents the destination item
    # @param distance [Integer] Optional distance between source and destination.
    #                           If not provided, a default distance of `1` is used.
    # @return [nil]
    def add(source, dest, distance: 1)
      _add_edge(node(source), node(dest), distance) unless source == dest
    end

    # Finds the shortest path between items, returning both the path as well as
    # the total distance travelled.
    # @param source [Object] Any Ruby object that represents the source item
    # @param dest [Object] Any Ruby object that represents the destination item
    # @return [BestPath]
    def shortest_path(source, dest)
      best_path = BestPath.new
      best_path.distance = _shortest_path(node(source), node(dest), best_path.path)
      if best_path.path.empty? || best_path.distance.nil? || best_path.distance < 0
        raise NoPathExistsError
      end
      best_path
    end

    private

    def node(obj)
      @nodes[obj] ||= @nodes.size # Auto-increment id
    end

  end
end
