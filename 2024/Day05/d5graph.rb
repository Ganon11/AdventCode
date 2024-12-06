require 'optparse'
require 'rgl/adjacency'
require 'rgl/dot'

options = {
  :filename => 'input.txt'
}

OptionParser.new do |opts|
  opts.banner = 'Usage: d5graph.rb [options]'
  opts.on('-f FILENAME', '--filename=FILENAME', String, 'Which file to use?')
end.parse!(into: options)

dg = RGL::DirectedAdjacencyGraph[]
IO.readlines(options[:filename]).map(&:strip).each do |line|
  if line.empty?
    break
  end

  first, second = line.split('|').map(&:to_i)
  dg.add_edge first, second
end

puts "Vertices: #{dg.vertices}"
puts "Edges: #{dg.edges.sort.to_s}"
dg.write_to_graphic_file('jpg', File.basename(options[:filename], '.txt'))
