use strict;
use warnings;
#use integer;

my $GEN_A_FACTOR = 16807;
my $GEN_B_FACTOR = 48271;
my $DIVISOR = 2147483647;

sub NextValue {
  my ($previousValue, $factor) = @_;
  return (($previousValue * $factor) % $DIVISOR);
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

my $ITERATIONS = 40000000;

my $judge = 0;
foreach my $i (1..$ITERATIONS) {
  $genAValue = NextValue($genAValue, $GEN_A_FACTOR);
  $genBValue = NextValue($genBValue, $GEN_B_FACTOR);
  my $genAString = substr(sprintf("%032b", $genAValue), -16);
  my $genBString = substr(sprintf("%032b", $genBValue), -16);
  if ($genAString eq $genBString) {
    ++$judge;
  }
  # print "$genAString\n";
  # print "$genBString\n";
  # print "\n";
  #print "A: $genAValue, B: $genBValue\n";
}

print "Judge says $judge\n";
