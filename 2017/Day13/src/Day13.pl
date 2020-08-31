use strict;
use warnings;
use Storable qw(dclone);

sub WillBeCaught {
  my $delay = shift;
  my $depth = shift;
  my $range = shift;

  if (($delay + $depth) % ((2 * $range) - 2) == 0) {
    return 1;
  }

  return 0;
}

my $inputFile;
if (scalar(@ARGV) > 0) {
  $inputFile = shift @ARGV;
} else {
  $inputFile = '../input/input.txt';
}

my %layers = ();
open(my $fh, "<", $inputFile) or die "Couldn't open $inputFile: $!";
while (<$fh>) {
  my $line = $_;
  chomp($line);
  $line =~ m/^(\d+): (\d+)$/i;
  my ($depth, $range) = (int($1), int($2));
  $layers{$depth} = $range;
}
close($fh);

my $delay = 0;
while (1) {
  my $severity = 0;
  my $caught = 0;
  foreach my $depth (keys %layers) {
    if (1 == WillBeCaught($delay, $depth, $layers{$depth})) {
      $severity = $severity + ($depth * $layers{$depth});
      $caught = 1;
    }
  }

  if ($delay == 0) {
    print "Part 1 Severity: $severity\n";
  }

  if ($caught == 0) {
    print "Didn't get caught with delay $delay\n";
    last;
  }

  ++$delay;
}
