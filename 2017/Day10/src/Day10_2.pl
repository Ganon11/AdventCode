use strict;
use warnings;

sub ReverseSection {
  my $listRef = shift;
  my $start = shift;
  my $length = shift;

  my $listLength = scalar(@$listRef);
  my @reversedList = ();
  for (my $i = 0; $i < $length; ++$i) {
    push(@reversedList, $$listRef[($start + $i) % $listLength]);
  }

  @reversedList = reverse(@reversedList);
  for (my $i = 0; $i < $length; ++$i) {
    $$listRef[($start + $i) % $listLength] = $reversedList[$i];
  }
}

sub CalculateKnotHash {
  my $input = shift;

  my @lengths = map { ord($_) } split(//, $input);
  push(@lengths, 17, 31, 73, 47, 23);
  my @list = (0..255);
  my $listLength = scalar(@list);
  my $position = 0;
  my $skipSize = 0;

  foreach my $round (1..64) {
    foreach my $length (@lengths) {
      ReverseSection(\@list, $position, $length);
      $position = ($position + $length + $skipSize) % $listLength;
      $skipSize = $skipSize + 1;
    }
  }

  my $denseHashString = '';
  foreach my $digit (0..15) {
    my $start = $digit * 16;
    my $result = $list[$start];
    foreach my $offset (1..15) {
      $result = $result ^ $list[$start + $offset];
    }
    $denseHashString = $denseHashString . sprintf("%02X", $result);
  }

  return $denseHashString;
}

my $inputFile;
if (scalar(@ARGV) > 0) {
  $inputFile = shift @ARGV;
} else {
  $inputFile = '../input/Input.txt';
}

open(my $fh, "<", $inputFile) or die "Couldn't open $inputFile: $!";
my $line = <$fh>;
chomp($line);
close($fh);

my $knotHash = CalculateKnotHash($line);
print "Knot hash: $knotHash\n";
