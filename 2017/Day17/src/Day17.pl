use strict;
use warnings;

my $inputFile;
if (scalar(@ARGV) > 0) {
  $inputFile = shift @ARGV;
} else {
  $inputFile = '../input/input.txt';
}

my $steps;
open(my $fh, "<", $inputFile) or die "Couldn't open $inputFile: $!";
my $line = <$fh>;
chomp($line);
$steps = int($line);
close($fh);

my @buffer = ( 0 ) x 1;
my $position = 0;

my $afterValue;
foreach my $size (1..50000000) {
  $position = ($position + $steps) % $size;
  if ($position == 0) {
    $afterValue = $size;
  }
  #splice(@buffer, $position + 1, 0, $newValue);
  ++$position;

  #print "@buffer\n";
}

#print "@buffer\n";
#print "buffer[$position + 2] = $buffer[$position+2]\n";
#print "buffer[1] = $buffer[1]\n";

print "$afterValue\n";
