use Data::Dumper;
use strict;
use warnings;

sub FindMaxBank {
  my $banksRef = shift;
  my @banks = @{$banksRef};
  my $maxBankIndex = 0;
  for (my $i = 1; $i < scalar(@banks); ++$i) {
    $maxBankIndex = $i if ($banks[$maxBankIndex] < $banks[$i]);
  }

  return $maxBankIndex;
}

sub ReallocateBlocks {
  my $index = shift;
  my $banksRef = shift;
  my $bankSize = scalar(@$banksRef);
  my $blockCount = @$banksRef[$index];
  @$banksRef[$index] = 0;
  $index = ($index + 1) % $bankSize;
  while ($blockCount > 0) {
    @$banksRef[$index] = @$banksRef[$index] + 1;
    $blockCount = $blockCount - 1;
    $index = ($index + 1) % $bankSize;
  }
}

my $inputFile;
if (scalar(@ARGV) > 0) {
  $inputFile = shift @ARGV;
} else {
  $inputFile = '../input/Input.txt';
}

my @banks = ();
open(my $fh, "<", $inputFile) or die "Couldn't open $inputFile: $!";
while (<$fh>) {
  my $line = $_;
  chomp($line);
  @banks = map { int($_) } split(/\s+/, $line);
}
close($fh);

my %states = ();
do {
  # Indicate that we've seen a state by setting its hashvalue to the step we
  # first saw it. This helps in calculating part 2.
  $states{join(',', @banks)} = scalar(keys(%states));

  # Reallocate blocks according to the puzzle's rules.
  ReallocateBlocks(FindMaxBank(\@banks), \@banks);
} while (not exists($states{join(',', @banks)}));

my $stateCount = scalar(keys(%states));
print "Part 1: $stateCount\n";

# The loop size is the difference between the second time we saw this state
# (which is right now) and the first time we saw this state (which is recorded
# as the hashvalue for this state)
my $loopSize = $stateCount - $states{join(',', @banks)};
print "Part 2: $loopSize\n";
