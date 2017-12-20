use strict;
use warnings;

use Data::Dumper;

sub Compute3DManhattanDistanceFromValues {
  my ($x, $y, $z) = @_;
  return abs($x) + abs($y) + abs($z);
}

sub Compute3DManhattanDistance {
  my $particle = shift;
  return Compute3DManhattanDistanceFromValues($particle->{'position'}{'X'}, $particle->{'position'}{'Y'}, $particle->{'position'}{'Z'});
}

my $inputFile;
if (scalar(@ARGV) > 0) {
  $inputFile = shift @ARGV;
} else {
  $inputFile = '../input/Input.txt';
}

my @particles;
my $closestParticle;
my $initialMinDistance;
open(my $fh, "<", $inputFile) or die "Couldn't open $inputFile: $!";
while (<$fh>) {
  my $line = $_;
  chomp($line);
  $line =~ m/^p=<([^,]+),([^,]+),([^,]+)>, v=<([^,]+),([^,]+),([^,]+)>, a=<([^,]+),([^,]+),([^,]+)>$/i;
  my (
    $positionX, $positionY, $positionZ,
    $velocityX, $velocityY, $velocityZ,
    $accelX, $accelY, $accelZ
  ) = (
    int($1), int($2), int($3),
    int($4), int($5), int($6),
    int($7), int($8), int($9)
  );

  my $distance = Compute3DManhattanDistanceFromValues($positionX, $positionY, $positionZ);
  if (!defined($initialMinDistance) or $distance < $initialMinDistance) {
    $initialMinDistance = $distance;
    $closestParticle = scalar(@particles); # Will equal the position of this particle.
  }

  push(@particles, {
    'position' => {
      'X' => $positionX,
      'Y' => $positionY,
      'Z' => $positionZ
    },
    'velocity' => {
      'X' => $velocityX,
      'Y' => $velocityY,
      'Z' => $velocityZ
    },
    'acceleration' => {
      'X' => $accelX,
      'Y' => $accelY,
      'Z' => $accelZ
    }
  });
}
close($fh);

my $timestamp = 0;
my $matchingCount = 0;

do {
  my $minDistance;
  my $currentClosestParticle;

  foreach my $index (0..scalar(@particles)-1) {
    $particles[$index]->{'velocity'}{'X'} += $particles[$index]->{'acceleration'}{'X'};
    $particles[$index]->{'velocity'}{'Y'} += $particles[$index]->{'acceleration'}{'Y'};
    $particles[$index]->{'velocity'}{'Z'} += $particles[$index]->{'acceleration'}{'Z'};

    $particles[$index]->{'position'}{'X'} += $particles[$index]->{'velocity'}{'X'};
    $particles[$index]->{'position'}{'Y'} += $particles[$index]->{'velocity'}{'Y'};
    $particles[$index]->{'position'}{'Z'} += $particles[$index]->{'velocity'}{'Z'};

    my $particleDistance = Compute3DManhattanDistance($particles[$index]);
    if (!defined($minDistance) or $particleDistance < $minDistance) {
      $minDistance = $particleDistance;
      $currentClosestParticle = $index;
    }
  }

  if ($closestParticle == $currentClosestParticle) {
    ++$matchingCount;
  } else {
    $closestParticle = $currentClosestParticle;
    $matchingCount = 0;
  }
  ++$timestamp;
} while ($matchingCount < 10000);

print "After $timestamp steps, particle $closestParticle is closest\n";
