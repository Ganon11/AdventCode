use strict;
use warnings;

sub Winner {
  my $n = shift;
  my $w = 1;

  for (my $i = 1; $i < $n; ++$i) {
    $w = ($w % $i) + 1;
    if ($w > ($i + 1) / 2) {
      ++$w;
    }
  }

  return $w;
}

my $numberOfElves = shift;
if (!defined($numberOfElves)) {
  $numberOfElves = 5;
}

print "Winner: " . Winner($numberOfElves) . "\n";

# my @elves;
# for (my $i = 0; $i < $numberOfElves; ++$i) {
#   push(@elves, $i + 1);
# }

# my $i = 0;
# my $len = scalar(@elves);
# while ($len > 1) {
#   #print "i: $i, len: $len\n";
#   my $stealFrom = ($i + ($len / 2)) % $len;
#   #print "Index $i stealing from index $stealFrom\n";
#   #print "Elf $elves[$i] steals from elf $elves[$stealFrom]\n";
#   splice(@elves, $stealFrom, 1);
#   $len = scalar(@elves);
#   $i = ($i + 1) % $len;
#   print "Remaining elves: " . join(', ', @elves) . "\n" if $len < 10;
# }

# print "Elf $elves[0] is the winner!\n";
