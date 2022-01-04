##
# When graph nodes "know" which nodes they are connected to, one of the two
# class methods on this module can be be used directly to calculate the shortest
# distance between nodes.  Node objects must implement a method (by default
# <code>connections</code>) which yields all connected nodes and (optionally) the
# distance to that connected node. If the yield only supplies the connected
# node, a defualt distance of <code>1</code> is used.
#
# It is important to ensure that <code>hash</code> and <code>eql?</code> are properly implemented so
# that two objects that are logically the same are treatest thusly.
#
# @example
#
#   class SomethingWithConnections
#     def find_next
#       yield SomethingWithConnections.new('C'), 2
#       yield SomethingWithConnections.new('D'), 4
#     end
#
#     def hash
#       # implement me
#     end
#
#     def eql?(other)
#       # implement me
#     end
#   end
#
#   a = SomethingWithConnections.new('A')
#   b = SomethingWithConnections.new('B')
#   distance, path = DijkstraFast.shortest_path(a, b, connections: :find_next)
#
# @example
#
#   class SomethingElseWithConnections
#     def connections
#       yield # implement me
#       yield # implement me
#     end
#   end
#
#   a = SomethingElseWithConnections.new
#   b = SomethingElseWithConnections.new
#   distance = DijkstraFast.shortest_distance(a, b)
##
module DijkstraFast

  autoload :Graph,              'dijkstra_fast/graph'
  autoload :Native,             'dijkstra_fast/native'
  autoload :NoPathExistsError,  'dijkstra_fast/no_path_exists_error'
  autoload :PriorityQueue,      'dijkstra_fast/priority_queue'
  autoload :ShortestPath,       'dijkstra_fast/shortest_path'
  autoload :VERSION,            'dijkstra_fast/version'

  # Finds the shortest path between items, returning both the path as well as
  # the total distance travelled.
  # @param source [Object] Any Ruby object that represents the source item
  # @param dest [Object] Any Ruby object that represents the destination item
  # @param progress [Boolean|String|Proc] If falsey (default), no progress will
  #        be displayed.  If a Proc is given, it will be yielded progress
  #        (completed / total) as progress is made.  If otherwise truthy the
  #        ruby-progressbar gem will be required and displayed; if passed a
  #        String it will be used as the progress bar's format.
  # @param connections [Symbol|String] The method to call on each item to
  #        obtain adjacent items in the graph.  Defaults to 'connections'
  # @return [Array<int, Array>]
  def self.shortest_path(source, dest, connections: 'connections', progress: false)
    clazz = Class.new { include DijkstraFast::ShortestPath }
    clazz.define_method(:connections) do |u, &block|
      u.send(connections, &block)
    end
    clazz.new.shortest_path(source, dest, progress: progress)
  end

  # Finds the shortest distance between items
  # @param source [Object] Any Ruby object that represents the source item
  # @param dest [Object] Any Ruby object that represents the destination item
  # @param progress [Boolean|String|Proc] If falsey (default), no progress will
  #        be displayed.  If a Proc is given, it will be yielded progress
  #        (completed / total) as progress is made.  If otherwise truthy the
  #        ruby-progressbar gem will be required and displayed; if passed a
  #        String it will be used as the progress bar's format.
  # @param connections [Symbol|String] The method to call on each item to
  #        obtain adjacent items in the graph.
  # @return [int]
  def self.shortest_distance(source, dest, connections: 'connections', progress: false)
    shortest_path(source, dest, connections: connections, progress: progress).first
  end

end

require 'dijkstra_fast/dijkstra_fast'
