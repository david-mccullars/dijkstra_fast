# Dijkstra (Fast!)

* README:         https://github.com/david-mccullars/dijkstra_fast
* Documentation:  http://www.rubydoc.info/github/david-mccullars/dijkstra_fast
* Bug Reports:    https://github.com/david-mccullars/dijkstra_fast/issues


## Status

[![Gem Version](https://badge.fury.io/rb/dijkstra_fast.svg)](https://badge.fury.io/rb/dijkstra_fast)
[![Travis Build Status](https://travis-ci.org/david-mccullars/dijkstra_fast.svg?branch=master)](https://travis-ci.org/david-mccullars/dijkstra_fast)
[![Code Climate](https://codeclimate.com/github/david-mccullars/dijkstra_fast/badges/gpa.svg)](https://codeclimate.com/github/david-mccullars/dijkstra_fast)
[![Test Coverage](https://codeclimate.com/github/david-mccullars/dijkstra_fast/badges/coverage.svg)](https://codeclimate.com/github/david-mccullars/dijkstra_fast/coverage)
[![MIT License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)


## Description

[Dijkstra](https://en.wikipedia.org/wiki/Dijkstra's_algorithm) is a commonly
used algorithm for finding the shortest path/distance between two items in an
interconnected collection of items (such as a graph or network).


## Features

* Native implementation of Dijkstra's algorithm intended for use on large,
typically sparse graphs.
* Allows for calculating shortest distance without requiring all nodes/edges to
be produced. (In many applications doing this can be more expensive than
Dijkstra's algorithm itself - or even impractical.)
* Can be used in one of three ways:
  * __Method 1__: Use an instance of `DijkstraFast::Graph` by adding edges to
  it.
  * __Method 2__: Add `shortest_path` and `shortest_distance` methods to an
  existing class by including the `DijkstraFast::ShortestPath` module.
  * __Method 3__: Call `DijkstraFast::ShortestPath.shortest_path` or
  `DijkstraFast::ShortestPath.shortest_distance` directly when the `source` and
  `dest` objects know how to find "connected" items via some method.


## Installation

```
gem install dijkstra_fast
```

## Requirements

* Ruby 3.0 or higher


## Usage

### Method 1: Use `DijkstraFast::Graph`

```ruby
graph = DijkstraFast::Graph.new
graph.add("A", "B", distance: 5)
graph.add("A", "C", distance: 8)
graph.add("B", "C", distance: 2)
distance, path = graph.shortest_path("A", "C")
path

=> ["A", "B", "C"]

distance

=> 7
```

### Method 2: Include `DijkstraFast::ShortestPath`

```ruby
class MyNetwork
  include DijkstraFast::ShortestPath

  def connections(source)
    case source
    when "A"
      yield "B", 3
      yield "C", 8
    when "B"
      yield "C" # Default distance 1
    end
  end
end

distance, path = MyNetwork.new.shortest_path("A", "C")
path

=> ["A", "B", "C"]

distance

=> 4
```

### Method 3: Call `DijkstraFast.shortest_path` or `DijkstraFast.shortest_distance`

```ruby
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
path.map(&:label)

=> ["A", "B", "C"]

distance

=> 7
```


## Additional Notes

* When using arbitrary Ruby objects as graph nodes, it is important to ensure
that [Object#hash](https://ruby-doc.org/core-3.1.0/Object.html#method-i-hash)
and [Object#eql?](https://ruby-doc.org/core-3.1.0/Object.html#method-i-eql-3F)
are properly implemented so that two instances which are logically the same
will be considered as the same node.  Under the hood, this gem creates a
bi-directional mapping between nodes and an auto-incrementing integer id so
that Ruby objects do not have to be passed into the internals of the native
implementation; doing so would risk problems with garbage collection and is
otherwise frowned upon when implementing native extensions.
* All `shortest_path` and `shortest_distance` methods provide an optional
`progress` named argument which (when set to anything truthy) will provide
progress as the algorithm proceeds.  The default implementation uses the
[progressbar](https://github.com/jfelchner/ruby-progressbar/wiki) gem, but this
is not required.
* The maximum number of items that can be handled by this implementation is
determined by the size of `unsigned long` which is compiler dependent.  On many
machines this can be 18,446,744,073,709,551,615 (2^64 – 1). A
[RuntimeError](https://ruby-doc.org/core-3.1.0/RuntimeError.html) will be
thrown if this is surpassed; if so, congratulations on being bad ass!
* A priority queue is used within the native Dijkstra implemenation to maintain
the next shortest path to consider within the algorithm's main loop. It is
possible that switching to a [Fibonacci
heap](https://en.wikipedia.org/wiki/Fibonacci_heap) might improve performance.


## License

MIT. See the `LICENSE` file.


## References

> Cormen, Thomas H.; Leiserson, Charles E.; Rivest, Ronald L.; Stein, Clifford (2001). "Section 24.3: Dijkstra's algorithm". Introduction to Algorithms (Second ed.). MIT Press and McGraw–Hill. pp. 595–601. ISBN 0-262-03293-7.

> Knuth, D.E. (1977). "A Generalization of Dijkstra's Algorithm". Information Processing Letters. 6 (1): 1–5. doi:10.1016/0020-0190(77)90002-3.
