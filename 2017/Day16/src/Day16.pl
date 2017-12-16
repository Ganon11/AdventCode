use strict;
use warnings;
use List::MoreUtils;

sub Spin {
  my $programsRef = shift;
  my $size = shift;

  my @endPrograms = splice(@$programsRef, -1 * $size);
  unshift(@$programsRef, @endPrograms);
}

sub Exchange {
  my $programsRef = shift;
  my $positionA = shift;
  my $positionB = shift;

  my $temp = $$programsRef[$positionA];
  $$programsRef[$positionA] = $$programsRef[$positionB];
  $$programsRef[$positionB] = $temp;
}

sub Partner {
  my $programsRef = shift;
  my $programA = shift;
  my $programB = shift;

  my $positionA = List::MoreUtils::first_index { $_ eq $programA } @$programsRef;
  my $positionB = List::MoreUtils::first_index { $_ eq $programB } @$programsRef;

  Exchange($programsRef, $positionA, $positionB);
}

sub Dance {
  my $programsRef = shift;
  my $directionsRef = shift;

  foreach my $direction (@$directionsRef) {
    if ($direction =~ m/s(\d+)/i) {
      Spin($programsRef, int($1));
    } elsif ($direction =~ m/x(\d+)\/(\d+)/i) {
      Exchange($programsRef, int($1), int($2));
    } elsif ($direction =~ m/p(\w)\/(\w)/i) {
      Partner($programsRef, $1, $2);
    }
  }

  return join('', @$programsRef);
}

my $inputFile;
if (scalar(@ARGV) > 0) {
  $inputFile = shift @ARGV;
} else {
  $inputFile = '../input/Input.txt';
}

my @programs;
if ($inputFile =~ m/Sample/i) {
  @programs = ('a', 'b', 'c', 'd', 'e');
} else {
  @programs = ('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p');
}

open(my $fh, "<", $inputFile) or die "Couldn't open $inputFile: $!";
my $line = <$fh>;
chomp($line);
close($fh);

my %states = ();
$states{ join('', @programs) } = 1;

my $cyclesToPerform;

my @directions = split(/,/, $line);
foreach my $iteration (1..1000000000) {
  my $newState = Dance(\@programs, \@directions);
  if (exists($states{$newState})) {
    print "Cycle after $iteration iterations\n";
    $cyclesToPerform = 1000000000 % $iteration;
    last;
  }

  $states{$newState} = 1;
}

foreach my $iteration2 (1..$cyclesToPerform) {
  Dance(\@programs, \@directions);
}

print join('', @programs), "\n";
