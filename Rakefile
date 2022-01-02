require 'bundler/gem_tasks'
require 'rake/extensiontask'
require 'rspec/core/rake_task'
require 'yard'

RSpec::Core::RakeTask.new(:spec)

task default: :spec

YARD::Rake::YardocTask.new

Rake::ExtensionTask.new('dijkstra_fast') do |ext|
  ext.lib_dir = 'lib/dijkstra_fast'
end
