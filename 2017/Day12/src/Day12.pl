use strict;
use warnings;
use Graph;

my $inputFile;
if (scalar(@ARGV) > 0) {
  $inputFile = shift @ARGV;
} else {
  $inputFile = '../input/input.txt';
}

my $graph = Graph::Undirected->new;
open(my $fh, "<", $inputFile) or die "Couldn't open $inputFile: $!";
while (<$fh>) {
  my $line = $_;
  chomp($line);
  if ($line =~ m/^(\d+) <-> (.*)$/i) {
    my ($source, $sink_string) = (int($1), $2);
    my @sinks = map { int($_) } $sink_string =~ m/(\d+)/ig;
    foreach my $sink (@sinks) {
      $graph->add_edge($source, $sink) if not $graph->has_edge($source, $sink);
    }
  }
}
close($fh);

my $componentIndex = $graph->connected_component_by_vertex(0);
my @reachable = $graph->connected_component_by_index($componentIndex);
my $reachableCount = scalar(@reachable);
print "Vertices connected to 0: $reachableCount\n";

my @allGroups = $graph->connected_components();
my $allGroupsCount = scalar(@allGroups);
print "Number of groups: $allGroupsCount\n";
