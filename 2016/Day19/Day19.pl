use strict;
use warnings;

sub WinnerPart1 {
  my $n = shift;

  # Find biggest power of 2 that fits into n
  my $t = 1;
  while (2 * $t <= $n) {
    $t *= 2;
  }

  return 1 if $n == $t; # For a power of 2, answer is 1
  return (2 * ($n - $t) + 1);
}

sub WinnerPart2 {
  my $n = shift;

  # Find biggest power of 3 that fits into n
  my $t = 1;
  while (3 * $t <= $n) {
    $t *= 3;
  }

  return $n if $n == $t; # For a power of 3, answer is n

  # Need to compare n to t and 3t
  my $midpoint = ((2 * $t) / 2) + $t;
  return $n - $t if $n < $midpoint;
  return (2 * $n) - (3 * $t);
}

my $numberOfElves = shift;
if (!defined($numberOfElves)) {
  $numberOfElves = 5;
}

print "Part 1: Elf " . WinnerPart1($numberOfElves) . " is the winner!\n";
print "Part 2: Elf " . WinnerPart2($numberOfElves) . " is the winner!\n";
