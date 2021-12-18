module DijkstraFast
  ##
  # The "best path" from applying Dijkstra's algorithm contains both the ordered
  # list of nodes travelled as well as the total distance travelled.
  ##
  class BestPath

    attr_reader :path
    attr_accessor :distance

    def initialize
      @path = []
      @distance = 0
    end

  end
end
