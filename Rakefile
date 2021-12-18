require "bundler/gem_tasks"
require "rake/extensiontask"
require "rspec/core/rake_task"

RSpec::Core::RakeTask.new(:spec)

task :default => :spec

require 'rdoc/task'
RDoc::Task.new do |rdoc|
  rdoc.main = "README.md"
  rdoc.rdoc_files.include("README.md", "lib/**/*.rb")
end

Rake::ExtensionTask.new('dijkstra_fast') do |ext|
  ext.lib_dir = 'lib/dijkstra_fast'
end
