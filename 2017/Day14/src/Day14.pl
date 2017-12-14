use strict;
use warnings;
use Switch;
use Data::Dumper;

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

sub UsedSquares {
  my $hexDigit = shift;

  switch ($hexDigit) {
    case '0' { return 0 }
    case ['1', '2', '4', '8'] { return 1 }
    case ['3', '5', '6', '9', 'a', 'A', 'c', 'C'] { return 2 }
    case ['7', 'b', 'B', 'd', 'D', 'e', 'E'] { return 3 }
    case ['f', 'F'] { return 4 }
    else { die "This isn't a hex digit: $hexDigit" }
  }
}

sub Visit {
  my $diskRef = shift;
  my $visitedRef = shift;
  my $row = shift;
  my $col = shift;

  $visitedRef->{"$row,$col"} = 1;

  my @disk = @{ $diskRef };
  my $currentRowRef = $disk[$row];

  if ($col > 0) {
    my $newCol = $col - 1;
    if (!exists($visitedRef->{"$row,$newCol"}) and $$currentRowRef[$newCol] == 1) {
      Visit($diskRef, $visitedRef, $row, $newCol);
    }
  }

  if ($col < 127) {
    my $newCol = $col + 1;
    if (!exists($visitedRef->{"$row,$newCol"}) and $$currentRowRef[$newCol] == 1) {
      Visit($diskRef, $visitedRef, $row, $newCol);
    }
  }

  if ($row > 0) {
    my $newRow = $row - 1;
    my $previousRowRef = $disk[$newRow];
    if (!exists($visitedRef->{"$newRow,$col"}) and $$previousRowRef[$col] == 1) {
      Visit($diskRef, $visitedRef, $newRow, $col);
    }
  }

  if ($row < 127) {
    my $newRow = $row + 1;
    my $nextRowRef = $disk[$newRow];
    if (!exists($visitedRef->{"$newRow,$col"}) and $$nextRowRef[$col] == 1) {
      Visit($diskRef, $visitedRef, $newRow, $col);
    }
  }
}

sub CountRegions {
  my $diskRef = shift;

  my @disk = @{ $diskRef };
  my %visited = ();
  my $regionCount = 0;
  foreach my $r (0..127) {
    my @diskRow = @{ $disk[$r] };
    foreach my $c (0..127) {
      if ($diskRow[$c] == 1 && !exists($visited{"$r,$c"})) {
        Visit($diskRef, \%visited, $r, $c);
        ++$regionCount;
      }
    }
  }

  return $regionCount;
}

sub PrintDisk {
  my $diskRef = shift;

  foreach my $row (0..127) {
    my @diskRow = @{ @$diskRef[$row] };
    foreach my $col (0..127) {
      my $digit = $diskRow[$col];
      if ($digit) {
        print '#';
      }
      else {
        print '.';
      }
    }
    print "\n";
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

my @disk = ();
my $totalUsed = 0;
foreach my $row (0..127) {
  my @diskRow = ();
  my $hashInput = $line . "-$row";
  my $knotHash = CalculateKnotHash($hashInput);
  foreach my $hexDigit (split(//, $knotHash)) {
    $totalUsed = $totalUsed + UsedSquares($hexDigit);
    my $binString = sprintf("%04b", hex($hexDigit));
    #print "Hex: $hexDigit, bin: $binString\n";
    push(@diskRow, map { int($_) } split(//, $binString));
  }
  push(@disk, \@diskRow);
}

print "Total squares used: $totalUsed\n";
#print Dumper(\@disk);
#PrintDisk(\@disk);
my $regionCount = CountRegions(\@disk);
print "Total regions: $regionCount\n";
