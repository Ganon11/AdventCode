use strict;
use warnings;

sub ComponentFits {
  my $component = shift;
  my $port = shift;

  return 0 if $$component[2]; # Component is already used
  return 1 if $$component[0] == $port; # Matches first port
  return 1 if $$component[1] == $port; # Matches second port
  return 0; # Does not fit
}

sub MaxWeight {
  my $components = shift;
  my $port = shift; # Port we're looking for

  my $componentCount = scalar(@$components);
  my $maxWeight = 0;
  my @indices = grep { ComponentFits(@$components[$_], $port) } 0..($componentCount - 1);
  foreach my $i (@indices) {
    my $newPort = $$components[$i][0];
    $newPort = $$components[$i][1] if $port == $newPort;
    $$components[$i][2] = 1;
    my $weight = $$components[$i][0] + $$components[$i][1] + MaxWeight($components, $newPort);
    $$components[$i][2] = 0;

    $maxWeight = $weight if ($weight > $maxWeight);
  }

  return $maxWeight;
}

sub MaxWeightLength {
  my $components = shift;
  my $port = shift; # Port we're looking for

  my $componentCount = scalar(@$components);
  my $maxWeight = 0;
  my $maxLength = 1;
  my @indices = grep { ComponentFits(@$components[$_], $port) } 0..($componentCount - 1);
  foreach my $i (@indices) {
    my $newPort = $$components[$i][0];
    $newPort = $$components[$i][1] if $port == $newPort;
    $$components[$i][2] = 1;
    my ($tempWeight, $tempLength) = MaxWeightLength($components, $newPort);
    $tempWeight += $$components[$i][0] + $$components[$i][1];
    ++$tempLength;
    $$components[$i][2] = 0;

    if ($tempLength > $maxLength) {
      $maxWeight = $tempWeight;
      $maxLength = $tempLength;
    } elsif ($tempLength == $maxLength && $tempWeight > $maxWeight) {
      $maxWeight = $tempWeight;
    }
  }

  return ($maxWeight, $maxLength);
}

my $inputFile;
if (scalar(@ARGV) > 0) {
  $inputFile = shift @ARGV;
} else {
  $inputFile = '../input/Input.txt';
}

my @components;
open(my $fh, "<", $inputFile) or die "Couldn't open $inputFile: $!";
while (<$fh>) {
  my $line = $_;
  chomp($line);

  $line =~ m/(\d+)\/(\d+)/i;
  push(@components, [int($1), int($2), 0]);
}
close($fh);

my $maxWeight = MaxWeight(\@components, 0);
print "Part 1: $maxWeight\n";
my ($longWeight, $maxLength) = MaxWeightLength(\@components, 0);
print "Part 2: ($longWeight, $maxLength)\n";
