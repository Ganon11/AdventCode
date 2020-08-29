use strict;
use warnings;

my $GEN_A_FACTOR = 16807;
my $GEN_B_FACTOR = 48271;
my $GEN_A_MULTIPLE = 4;
my $GEN_B_MULTIPLE = 8;
my $DIVISOR = 2147483647;

my $inputFile;
if (scalar(@ARGV) > 0) {
  $inputFile = shift @ARGV;
} else {
  $inputFile = '../input/input.txt';
}

my $genAValue;
my $genBValue;

open(my $fh, "<", $inputFile) or die "Couldn't open $inputFile: $!";
my $line = <$fh>;
chomp($line);
if ($line =~ m/^Generator A starts with (\d+)$/) {
  $genAValue = int($1);
}
$line = <$fh>;
chomp($line);
if ($line =~ m/^Generator B starts with (\d+)$/) {
  $genBValue = int($1);
}
close($fh);

my $ITERATIONS = 5000000;

my $judge = 0;
foreach my $i (1..$ITERATIONS) {
  do {
    $genAValue = (($genAValue * $GEN_A_FACTOR) % $DIVISOR);
  } while (($genAValue % $GEN_A_MULTIPLE) != 0);

  do {
    $genBValue = (($genBValue * $GEN_B_FACTOR) % $DIVISOR);
  } while (($genBValue % $GEN_B_MULTIPLE) != 0);

  if (($genAValue & 0xFFFF) == ($genBValue & 0xFFFF)) {
    ++$judge;
  }
}

print "Judge says $judge\n";
