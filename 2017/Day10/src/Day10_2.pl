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
  $inputFile = '../input/Input.txt';
}

open(my $fh, "<", $inputFile) or die "Couldn't open $inputFile: $!";
my $line = <$fh>;
chomp($line);
close($fh);


my @lengths = map { ord($_) } split(//, $line);
push(@lengths, 17, 31, 73, 47, 23);
print "ASCII codes: @lengths\n";

my @list;
if ($inputFile =~ m/Sample/i) {
  @list = (0..4);
} else {
  @list = (0..255);
}
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

print "List so far: @list\n";

# Assemble dense hash
my @denseHash;
my $denseHashString = '';
foreach my $digit (0..15) {
  my $start = $digit * 16;
  my $result = $list[$start];
  foreach my $offset (1..15) {
    $result = $result ^ $list[$start + $offset];
  }
  push(@denseHash, $result);
  $denseHashString = $denseHashString . sprintf("%02X", $result);
}

print "Dense hash (decimal): @denseHash\n";
print "Dense hash: $denseHashString\n";
