use strict;
use warnings;
use Data::Dumper;
use List::MoreUtils;
use Switch;

sub PrintBoard {
  my $boardRef = shift;
  my @board = @$boardRef;
  my $rowCount = scalar(@board);
  for (my $r = 0; $r < $rowCount; ++$r) {
    my @row = @{ $board[$r] };
    my $colCount = scalar(@row);
    for (my $c = 0; $c < $colCount; ++$c) {
      my $char = $row[$c];
      print "$char";
    }
    print "\n";
  }
}

my $inputFile;
if (scalar(@ARGV) > 0) {
  $inputFile = shift @ARGV;
} else {
  $inputFile = '../input/input.txt';
}

my @board;
my $r = 0;
my $c = 0;
open(my $fh, "<", $inputFile) or die "Couldn't open $inputFile: $!";
while (<$fh>) {
  my $line = $_;
  chomp($line);
  my @row = split(//, $line);
  my $colCount = scalar(@row);
  for ($c = 0; $c < $colCount; ++$c) {
    $board[$r][$c] = $row[$c];
  }
  ++$r;
}
close($fh);

#PrintBoard(\@board);

# Find starting point:
$r = 0;
$c = List::MoreUtils::first_index { $_ eq '|' } @{$board[$r]};
my $ROW_COUNT = scalar(@board);
my $COL_COUNT = scalar(@{ $board[0] });
my $direction = 'down';
my $visited = '';
my $steps = 0;

while (0 <= $r and $r < $ROW_COUNT and 0 <= $c and $c < $COL_COUNT and $board[$r][$c] ne ' ') {
  my $currentChar = $board[$r][$c];
  if ($currentChar =~ m/([a-z])/i) {
    $visited .= $currentChar;
  } elsif ($currentChar eq '+') {
    # New direction
    if ($direction eq 'down' or $direction eq 'up') {
      # Check left and right
      if ($c > 0 and $board[$r][$c - 1] ne ' ') {
        $direction = 'left';
      } elsif ($c < ($COL_COUNT - 1) and $board[$r][$c + 1] ne ' ') {
        $direction = 'right';
      } else {
        die "I'm lost at ($r, $c) and don't know where to go.";
      }
    } else {
      # Check up and down
      if ($r > 0 and $board[$r - 1][$c] ne ' ') {
        $direction = 'up';
      } elsif ($r < ($ROW_COUNT - 1) and $board[$r + 1][$c] ne ' ') {
        $direction = 'down';
      } else {
        die "I'm lost at ($r, $c) and don't know where to go.";
      }
    }
  }

  # Calculate next position
  switch ($direction) {
    case 'down' {
      ++$r;
    }
    case 'up' {
      --$r;
    }
    case 'left' {
      --$c;
    }
    case 'right' {
      ++$c;
    }
  }

  ++$steps;
}

print "I saw: $visited\n";
print "I walked $steps steps\n";
