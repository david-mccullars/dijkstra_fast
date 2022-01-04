module DijkstraFast
  ##
  # Do not use this class. For internal purposes only.
  ##
  class Native

    private

    def initialize(delegate)
      @delegate = delegate
      @ids = {}
      @nodes = {}
    end

    def connections(source_id)
      @delegate.connections(node(source_id)) do |dest, distance = 1|
        yield node_id(dest), distance
      end
    end

    def shortest_path(source, dest, progress: false)
      if !progress
        progress = nil
      elsif !progress.is_a?(Proc)
        progress_bar = create_progress_bar(progress)
        progress = lambda do |completed, total|
          progress_bar.total = total
          progress_bar.progress = completed
        end
      end

      distance, path = _shortest_path(node_id(source), node_id(dest), progress)
      [distance, @ids.invert.values_at(*path)]
    ensure
      progress_bar&.clear
    end

    def node_id(node)
      id = @ids[node]
      if id.nil?
        id = @ids[node] = @ids.size + 1 # Auto-incrementing id (starting at 1 to leave 0 = nil)
        @nodes[id] = node
      end
      id
    end

    def node(id)
      @nodes.fetch(id)
    end

    def create_progress_bar(config)
      require 'progressbar' unless defined?(ProgressBar)
      config = progress_bar_config(config) unless config.is_a?(Hash)
      ProgressBar.create(config)
    rescue LoadError
      raise LoadError, 'The default implementation of progress reporting requires the progressbar gem'
    end

    def progress_bar_config(format)
      format = "%a %b\u{15E7}%i %p%% %t" unless format.is_a?(String)
      {
        format:         format,
        progress_mark:  ' ',
        remainder_mark: "\u{FF65}",
        total:          100,
        autofinish:     false,
      }
    end

  end
end
