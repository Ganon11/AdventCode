use strict;
use warnings;
use List::MoreUtils qw(uniq);

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
my $smallestAcceleration;
my @guesses;
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

  my $acceleration = Compute3DManhattanDistanceFromValues($accelX, $accelY, $accelZ);
  if (!defined($smallestAcceleration) or $acceleration < $smallestAcceleration) {
    # New smallest acceleration - reset guesses.
    $smallestAcceleration = $acceleration;
    @guesses = ( scalar(@particles) );
  } elsif ($acceleration == $smallestAcceleration) {
    # Same acceleration as smallest - add a guess.
    push(@guesses, scalar(@particles));
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

my $guessCount = scalar(@guesses);
if ($guessCount == 1) {
  print "I GUESS @guesses\n";
} else {
  print "There are $guessCount particles with the smallest acceleration\n";

  my @newGuesses;
  my $smallestVelocity;
  foreach my $index (@guesses) {
    my $v = Compute3DManhattanDistance($particles[$index]);
    if (!defined($smallestVelocity) or $v < $smallestVelocity) {
      # New smallest velocity
      $smallestVelocity = $v;
      @newGuesses = ( $index );
    } elsif ($v == $smallestVelocity) {
      # Same velocity - add a guess
      push(@newGuesses, $index);
    }
  }

  $guessCount = scalar(@newGuesses);

  if ($guessCount == 1) {
    print "I GUESS @newGuesses\n";
  } else {
    print "There are $guessCount particles with the smallest velocity\n";
  }
}

my $timestamp = 0;
my $particleCount;

do {
  my %collisions;
  $particleCount = scalar(@particles);
  my @indicesToRemove = ();
  for (my $index = 0; $index < $particleCount; ++$index) {
    $particles[$index]->{'velocity'}{'X'} += $particles[$index]->{'acceleration'}{'X'};
    $particles[$index]->{'velocity'}{'Y'} += $particles[$index]->{'acceleration'}{'Y'};
    $particles[$index]->{'velocity'}{'Z'} += $particles[$index]->{'acceleration'}{'Z'};

    $particles[$index]->{'position'}{'X'} += $particles[$index]->{'velocity'}{'X'};
    $particles[$index]->{'position'}{'Y'} += $particles[$index]->{'velocity'}{'Y'};
    $particles[$index]->{'position'}{'Z'} += $particles[$index]->{'velocity'}{'Z'};

    my $positionString = "$particles[$index]->{'position'}{'X'},$particles[$index]->{'position'}{'Y'},$particles[$index]->{'position'}{'Z'}";
    if (!exists($collisions{$positionString})) {
      $collisions{$positionString} = $index;
    } else {
      push(@indicesToRemove, $collisions{$positionString}, $index);
    }
  }

  @indicesToRemove = uniq(sort { $b <=> $a } @indicesToRemove);
  foreach my $i (@indicesToRemove) {
    splice(@particles, $i, 1);
  }

  ++$timestamp;
} while ($timestamp < 1000);

print "After $timestamp steps, $particleCount particles remain\n";
