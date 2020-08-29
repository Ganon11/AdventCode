use strict;
use warnings;
use Switch;

sub CubeDistance {
  my ($x, $y, $z) = @_;
  # return (abs(a.x - b.x) + abs(a.y - b.y) + abs(a.z - b.z)) / 2
  # All B coordinates will be 0, which reduces this to:
  return (abs($x) + abs($y) + abs($z)) / 2;
}

my $inputFile;
if (scalar(@ARGV) > 0) {
  $inputFile = shift @ARGV;
} else {
  $inputFile = '../input/input.txt';
}

open(my $fh, "<", $inputFile) or die "Couldn't open $inputFile: $!";
my $line = <$fh>;
chomp($line);
close($fh);

my @directions = split(/,/, $line);

my ($x, $y, $z) = (0, 0, 0);
my $maxDistance = 0;
foreach my $direction (@directions) {
  switch ($direction) {
    case 'n' {
      ++$y;
      --$z;
    }
    case 'ne' {
      ++$x;
      --$z;
    }
    case 'se' {
      --$y;
      ++$x;
    }
    case 's' {
      --$y;
      ++$z;
    }
    case 'sw' {
      --$x;
      ++$z;
    }
    case 'nw' {
      ++$y;
      --$x;
    }
    else {
      die "Unknown direction $direction";
    }
  }

  my $tempDistance = CubeDistance($x, $y, $z);
  $maxDistance = $tempDistance if ($tempDistance > $maxDistance);
}

my $distance = CubeDistance($x, $y, $z);
print "Location: ($x, $y, $z), distance $distance, max distance $maxDistance\n";
