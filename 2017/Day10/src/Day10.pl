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

my $inputFile;
if (scalar(@ARGV) > 0) {
  $inputFile = shift @ARGV;
} else {
  $inputFile = '../input/input.txt';
}

open(my $fh, "<", $inputFile) or die "Couldn't open $inputFile: $!";
my $line = <$fh>;
chomp($line);
close($fh);
my @lengths = map { int($_) } split(/,/, $line);

my @list;
if ($inputFile =~ m/Sample/i) {
  @list = (0..4);
} else {
  @list = (0..255);
}
my $listLength = scalar(@list);

my $position = 0;
my $skipSize = 0;

foreach my $length (@lengths) {
#  print "List: @list, position: $position, skip size: $skipSize, length: $length\n";
  ReverseSection(\@list, $position, $length);
  $position = ($position + $length + $skipSize) % $listLength;
  $skipSize = $skipSize + 1;
}

#print "List: @list\n";
my $product = $list[0] * $list[1];
print "Part 1: $product\n";
