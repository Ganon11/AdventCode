use strict;
use warnings;

sub Trim {
  my $s = shift;
  $s =~ s/^\s+|\s+$//g;
  return $s;
}

sub ParseLengths {
  my $line = Trim(shift);
  return split(/\s+/, $line);
}

sub IsGoodTriangle {
  my @sides = @_;
  my $totalLength = eval join('+', @sides);

  foreach my $side (@sides) {
    my $partialLength = $totalLength - $side;
    if ($partialLength <= $side) {
      return 0;
    }
  }

  return 1;
}

sub DoLine {
  my $line = shift;
  my @sides = ParseLengths($line);
  return IsGoodTriangle(@sides);
}

my $inputfile = shift;

if (!defined($inputfile)) {
  $inputfile = 'Sample.txt';
}

open(my $fh, '<', $inputfile) or die "Couldn't open $inputfile: $!";
chomp(my @lines = <$fh>);
close($fh);

my @results = map { DoLine($_) } @lines;
my $sum = eval join('+', @results);
print "Part 1: $sum good triangles\n";

my @verticalTriangles = ();

foreach my $line (@lines) {
  my @sides = ParseLengths($line);
  push (@verticalTriangles, \@sides);
}

my $goodTriangles = 0;
my $len = @verticalTriangles / 3;

for (my $i = 0; $i < $len; ++$i) {
  for (my $col = 0; $col < 3; ++$col) {
    my $row = ($i * 3);
    my @sides = ($verticalTriangles[$row][$col], $verticalTriangles[$row + 1][$col], $verticalTriangles[$row + 2][$col]);

    if (1 == IsGoodTriangle(@sides)) {
      $goodTriangles = $goodTriangles + 1;
    }
  }
}

print "Part 2: $goodTriangles good triangles.\n";
