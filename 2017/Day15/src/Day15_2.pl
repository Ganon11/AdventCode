use strict;
use warnings;
#use integer;

my $GEN_A_FACTOR = 16807;
my $GEN_B_FACTOR = 48271;
my $GEN_A_MULTIPLE = 4;
my $GEN_B_MULTIPLE = 8;
my $DIVISOR = 2147483647;

sub NextValue {
  my ($previousValue, $factor, $multiple) = @_;
  my $newValue = (($previousValue * $factor) % $DIVISOR);
  while (($newValue % $multiple) != 0) {
    $newValue = (($newValue * $factor) % $DIVISOR);
  }
  return $newValue;
}

my $inputFile;
if (scalar(@ARGV) > 0) {
  $inputFile = shift @ARGV;
} else {
  $inputFile = '../input/Input.txt';
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
  $genAValue = NextValue($genAValue, $GEN_A_FACTOR, $GEN_A_MULTIPLE);
  $genBValue = NextValue($genBValue, $GEN_B_FACTOR, $GEN_B_MULTIPLE);
  my $genAString = substr(sprintf("%032b", $genAValue), -16);
  my $genBString = substr(sprintf("%032b", $genBValue), -16);
  if ($genAString eq $genBString) {
    ++$judge;
  }
}

print "Judge says $judge\n";
