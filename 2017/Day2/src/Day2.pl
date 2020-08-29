use strict;
use warnings;

sub DetermineDifference {
  my @row = @_;
  my $min = $row[0];
  my $max = $row[0];
  my $length = scalar(@row);

  for (my $i = 1; $i < $length; ++$i) {
    $min = $row[$i] if ($row[$i] < $min);
    $max = $row[$i] if ($row[$i] > $max);
  }

  return $max - $min;
}

sub DetermineEvenlyDivisbleResult {
  my @row = @_;
  my $length = scalar(@row);

  for (my $i = 0; $i < $length; ++$i) {
    for (my $j = $i + 1; $j < $length; ++$j) {
      my ($num1, $num2) = ($row[$i], $row[$j]);
      return $num1 / $num2 if ($num1 % $num2 == 0);
      return $num2 / $num1 if ($num2 % $num1 == 0);
    }
  }
}

my $inputFile;
if (scalar(@ARGV) > 0) {
  $inputFile = $ARGV[0];
} else {
  $inputFile = 'sample.txt';
}

open(my $fh, "<", $inputFile) or die "Couldn't open $inputFile: $!";
my $checksum = 0;
my $checksum2 = 0;
while (<$fh>) {
  my $line = $_;
  my @row = map { int($_) } split(/\s/, $line);
  my $diff = DetermineDifference(@row);
  $checksum = $checksum + $diff;
  my $divisionResult = DetermineEvenlyDivisbleResult(@row);
  $checksum2 = $checksum2 + $divisionResult;
}

print "Checksum: $checksum\n";
print "Checksum 2: $checksum2\n";
