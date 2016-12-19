use strict;
use warnings;

sub GetElvesInPlay {
  my $aref = shift;
  return grep { $_ > 0 } @$aref;
}

sub DoRoundOfPlay {
  my $aref = shift;
  my $len = scalar(@$aref);
  for (my $i = 0; $i < $len; ++$i) {
    # Skip elves with no presents.
    next if ($aref->[$i] == 0);

    # Find the next elf with presents
    for (my $j = 1; $j < $len; ++$j) {
      my $index = ($i + $j) % $len;

      # Skip elves with no presents
      next if ($aref->[$index] == 0);

      # Take all presents from elf at index
      $aref->[$i] = $aref->[$i] + $aref->[$index];
      $aref->[$index] = 0;
      last;
    }
  }
}

my $numberOfElves = shift;
if (!defined($numberOfElves)) {
  $numberOfElves = 5;
}

my @presents;
for (my $i = 0; $i < $numberOfElves; ++$i) {
  push(@presents, 1);
}

my $count = GetElvesInPlay(\@presents);
while ($count > 1) {
  DoRoundOfPlay(\@presents);
  $count = GetElvesInPlay(\@presents);
}

for (my $i = 0; $i < $numberOfElves; ++$i) {
  next if $presents[$i] == 0;
  print "Elf " . ($i + 1) . " is the winner!\n";
  last;
}
