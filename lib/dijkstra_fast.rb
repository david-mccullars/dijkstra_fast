##
# Provides native implementation of Dijkstra's algorithm for finding the shortest
# path between two vertices in a large, sparse graph.
#
# Underlying algorithm is implemented in C using a priority queue.  Edges are
# represented using linked lists rather than an adjacency matrix to reduce memory
# footprint when operating on very large graphs where the average number of edges
# between nodes is relatively small (e.g. < 1/10 the number of nodes). See
#
# @see README
# @see https://en.wikipedia.org/wiki/Dijkstra's_algorithm
##
module DijkstraFast

  autoload :BestPath,           'dijkstra_fast/best_path'
  autoload :Graph,              'dijkstra_fast/graph'
  autoload :NoPathExistsError,  'dijkstra_fast/no_path_exists_error'
  autoload :VERSION,            'dijkstra_fast/version'

end

require 'dijkstra_fast/dijkstra_fast'
