lib = File.expand_path('lib', __dir__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'dijkstra_fast/version'

Gem::Specification.new do |spec|
  spec.name          = 'dijkstra_fast'
  spec.version       = DijkstraFast::VERSION
  spec.authors       = ['David McCullars']
  spec.email         = ['david.mccullars@gmail.com']

  spec.summary       = '(Native) implementation of Dijkstra algorithm for large, sparse graphs'
  spec.description   = <<~DESCRIPTION
    Native implementation of Dijkstra algorithm for finding the shortest path
    between two vertices in a large, sparse graphs. Underlying algorithm is
    implemented in C using a priority queue.  Edges are represented using linked
    lists rather than an adjacency matrix to reduce memory footprint when operating
    on very large graphs where the average number of edges between nodes is
    relatively small (e.g. < 1/10 the number of nodes). See
    https://en.wikipedia.org/wiki/Dijkstra's_algorithm for additional information.
  DESCRIPTION
  spec.homepage      = 'https://github.com/david-mccullars/dijkstra_fast'
  spec.license       = 'MIT'

  spec.files         = `git ls-files -z`.split("\x0").reject { |f| f.match(%r{^(test|spec|features)/}) }
  spec.bindir        = 'exe'
  spec.executables   = spec.files.grep(%r{^exe/}) { |f| File.basename(f) }
  spec.extensions    = ['ext/dijkstra_fast/extconf.rb']
  spec.require_paths = ['lib']
  spec.required_ruby_version = '>= 3.0.0'

  spec.add_development_dependency 'bundler'
  spec.add_development_dependency 'github-markup'
  spec.add_development_dependency 'rake'
  spec.add_development_dependency 'rake-compiler'
  spec.add_development_dependency 'redcarpet'
  spec.add_development_dependency 'rspec'
  spec.add_development_dependency 'rubocop'
  spec.add_development_dependency 'rubocop-rake'
  spec.add_development_dependency 'rubocop-rspec'
  spec.add_development_dependency 'simplecov', '~> 0.17.0' # 0.18 not supported by code climate
  spec.add_development_dependency 'yard'
  spec.metadata = {
    'rubygems_mfa_required' => 'true',
  }
end
