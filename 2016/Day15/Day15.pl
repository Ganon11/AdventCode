use strict;
use warnings;

use Data::Dumper;

sub CapsuleWillFall {
  my ($t, $aref) = @_;

  my @discs = @{$aref};
  my $numDiscs = scalar(@discs);
  for (my $i = 0; $i < $numDiscs; ++$i) {
    return 0 if (($discs[$i][1] + $t + $i + 1) % $discs[$i][0]) != 0;
  }

  return 1;
}

my $inputfile = shift;
if (!defined($inputfile)) {
  $inputfile = 'Sample.txt';
}

my @discs;
open(my $fh, '<', $inputfile) or die "Couldn't open $inputfile: $!";
foreach my $line (<$fh>) {
  $line =~ s/\R//g; # Trim newlines.
  if ($line =~ m/^Disc #(\d+) has (\d+) positions; at time=0, it is at position (\d+)\.$/) {
    my ($positionTotal, $initialPosition) = (int($2), int($3));
    push(@discs, [$positionTotal, $initialPosition]);
  }
  else {
    die "Malformed input: '$line'";
  }
}
close($fh);

my $t = 0;
while (1) {
  last if (1 == CapsuleWillFall($t, \@discs));
  ++$t;
}

print "First capsule falls with t = $t\n";

push(@discs, [11, 0]);

$t = 0;
while (1) {
  last if (1 == CapsuleWillFall($t, \@discs));
  ++$t;
}

print "Second capsule falls with t = $t\n";
