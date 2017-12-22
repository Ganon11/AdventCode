use strict;
use warnings;
#use Data::Dumper;
use Clone qw(clone);
use Switch;

sub Turn {
  my $direction = shift;
  my $rotation = shift;

  switch ($direction) {
    case 'up' {
      return 'left' if $rotation eq 'left';
      return 'right';
    }
    case 'right' {
      return 'up' if $rotation eq 'left';
      return 'down';
    }
    case 'down' {
      return 'right' if $rotation eq 'left';
      return 'left';
    }
    case 'left' {
      return 'down' if $rotation eq 'left';
      return 'up';
    }
    else {
      die "Unknown direction $direction";
    }
  }
}

sub Burst {
  my $map = shift;
  my $x = shift;
  my $y = shift;
  my $direction = shift;

  # Rotate and Toggle
  my $rotation;
  my $causedInfection = 0;
  if (exists($map->{"$x,$y"}) and $map->{"$x,$y"} eq '#') {
    $rotation = 'right';
    $map->{"$x,$y"} = '.';
  } else {
    $rotation = 'left';
    $map->{"$x,$y"} = '#';
    $causedInfection = 1;
  }
  $direction = Turn($direction, $rotation);

  # Move
  switch ($direction) {
    case 'up' {
      --$x;
    }
    case 'right' {
      ++$y;
    }
    case 'down' {
      ++$x;
    }
    case 'left' {
      --$y;
    }
    else {
      die "Unknown direction $direction";
    }
  }

  return ($x, $y, $direction, $causedInfection);
}

sub Burst2 {
  my $map = shift;
  my $x = shift;
  my $y = shift;
  my $direction = shift;

  # Rotate and Toggle
  my $nodeStatus;
  if (!exists($map->{"$x,$y"})) {
    $nodeStatus = '.';
  } else {
    $nodeStatus = $map->{"$x,$y"};
  }

  my $causedInfection = 0;
  my $newStatus;
  switch ($nodeStatus) {
    case '.' {
      $direction = Turn($direction, 'left');
      $newStatus = 'W';
    }
    case 'W' {
      $newStatus = '#';
      $causedInfection = 1;
    }
    case '#' {
      $direction = Turn($direction, 'right');
      $newStatus = 'F';
    }
    case 'F' {
      $direction = Turn(Turn($direction, 'left'), 'left');
      $newStatus = '.';
    }
    else {
      die "Unknown node status $nodeStatus";
    }
  }
  $map->{"$x,$y"} = $newStatus;

  # Move
  switch ($direction) {
    case 'up' {
      --$x;
    }
    case 'right' {
      ++$y;
    }
    case 'down' {
      ++$x;
    }
    case 'left' {
      --$y;
    }
    else {
      die "Unknown direction $direction";
    }
  }

  return ($x, $y, $direction, $causedInfection);
}

my $inputFile;
if (scalar(@ARGV) > 0) {
  $inputFile = shift @ARGV;
} else {
  $inputFile = '../input/Input.txt';
}

my @rows;
open(my $fh, "<", $inputFile) or die "Couldn't open $inputFile: $!";
while (<$fh>) {
  my $line = $_;
  chomp($line);
  push(@rows, $line);
}
close($fh);

my %map;
my $mapSize = scalar(@rows);
# Map is N by N, where N is an odd integer (so there can be a middle)
# This means that X and Y will range from -M to +M, where M is (N-1/2)
foreach my $x ((-1 * (($mapSize - 1) / 2))..(($mapSize - 1) / 2)) {
  my @rowChars = split(//, shift(@rows));
  foreach my $y ((-1 * (($mapSize - 1) / 2))..(($mapSize - 1) / 2)) {
    my $newChar = shift(@rowChars);
    $map{"$x,$y"} = $newChar;
  }
}

my %map2 = %{ clone(\%map) };
my ($x, $y, $direction) = (0, 0, 'up');
my $totalInfectionBursts = 0;
my $TOTAL_BURSTS = 10000;
my $bursts = 0;

do {
  my $causedInfection;
  ($x, $y, $direction, $causedInfection) = Burst(\%map, $x, $y, $direction);
  ++$totalInfectionBursts if $causedInfection;
  ++$bursts;
} while ($bursts < $TOTAL_BURSTS);

print "Part 1: After $bursts bursts, a total of $totalInfectionBursts caused an infection.\n";

($x, $y, $direction) = (0, 0, 'up');
$totalInfectionBursts = 0;
$TOTAL_BURSTS = 10000000;
$bursts = 0;

do {
  my $causedInfection;
  ($x, $y, $direction, $causedInfection) = Burst2(\%map2, $x, $y, $direction);
  ++$totalInfectionBursts if $causedInfection;
  ++$bursts;
} while ($bursts < $TOTAL_BURSTS);

print "Part 2: After $bursts bursts, a total of $totalInfectionBursts caused an infection.\n";
