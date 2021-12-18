# Dijkstra (Fast!)

* README:         https://github.com/david-mccullars/dijkstra_fast
* Documentation:  http://www.rubydoc.info/github/david-mccullars/dijkstra_fast
* Bug Reports:    https://github.com/david-mccullars/dijkstra_fast/issues


## Status

[![Gem Version](https://badge.fury.io/rb/dijkstra_fast.svg)](https://badge.fury.io/rb/dijkstra_fast)
[![Travis Build Status](https://travis-ci.org/david-mccullars/dijkstra_fast.svg?branch=master)](https://travis-ci.org/david-mccullars/dijkstra_fast)
[![Code Climate](https://codeclimate.com/github/david-mccullars/dijkstra_fast/badges/gpa.svg)](https://codeclimate.com/github/david-mccullars/dijkstra_fast)
[![Test Coverage](https://codeclimate.com/github/david-mccullars/dijkstra_fast/badges/coverage.svg)](https://codeclimate.com/github/david-mccullars/dijkstra_fast/coverage)


## Description

[Dijkstra](https://en.wikipedia.org/wiki/Dijkstra's_algorithm) is a commonly
used algorithm for finding the shortest path through a graph or network.


## Features

* Native implementation of Dijkstra's algorithm intended for use on large,
sparse graphs for which an array of arrays is inefficient.


## Installation

```
gem install dijkstra_fast
```

## Requirements

* Ruby 3.0 or higher

## Usage

**Dijkstra**

```ruby
require 'dijkstra_fast'

graph = DijkstraFast::Graph.new
graph.add("A", "B", distance: 5)
graph.add("A", "C", distance: 8)
graph.add("B", "C", distance: 2)
best_path = graph.shortest_path("A", "C")
best_path.path

=> ["A", "B", "C"]

best_path.distance

=> 7
```


## License

MIT. See the `LICENSE` file.


## References

> Cormen, Thomas H.; Leiserson, Charles E.; Rivest, Ronald L.; Stein, Clifford (2001). "Section 24.3: Dijkstra's algorithm". Introduction to Algorithms (Second ed.). MIT Press and McGraw–Hill. pp. 595–601. ISBN 0-262-03293-7.

> Knuth, D.E. (1977). "A Generalization of Dijkstra's Algorithm". Information Processing Letters. 6 (1): 1–5. doi:10.1016/0020-0190(77)90002-3.
