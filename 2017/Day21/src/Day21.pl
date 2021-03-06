use strict;
use warnings;
use Data::Dumper;

sub RotatePattern {
  my $patternRef = shift;
  my $size = scalar(@$patternRef);

  if (2 == $size) {
    # 1 2     3 1
    # 3 4  -> 4 2
    my $temp = $$patternRef[0][0];
    $$patternRef[0][0] = $$patternRef[1][0];
    $$patternRef[1][0] = $$patternRef[1][1];
    $$patternRef[1][1] = $$patternRef[0][1];
    $$patternRef[0][1] = $temp;
  } elsif (3 == $size) {
    # 1 2 3    7 4 1
    # 4 5 6 -> 8 5 2
    # 7 8 9    9 6 3
    my $temp = $$patternRef[0][0];
    $$patternRef[0][0] = $$patternRef[2][0];
    $$patternRef[2][0] = $$patternRef[2][2];
    $$patternRef[2][2] = $$patternRef[0][2];
    $$patternRef[0][2] = $temp;

    $temp = $$patternRef[0][1];
    $$patternRef[0][1] = $$patternRef[1][0];
    $$patternRef[1][0] = $$patternRef[2][1];
    $$patternRef[2][1] = $$patternRef[1][2];
    $$patternRef[1][2] = $temp;
  } else {
    die "Wrong size pattern - can't rotate!";
  }
}

sub RotateString {
  my $string = shift;
  my $pattern = TranslateStringToSquare($string);
  RotatePattern($pattern);
  return TranslateSquareToString($pattern, scalar(@$pattern), 0, 0);
}

sub FlipPattern {
  my $patternRef = shift;
  my $size = scalar(@$patternRef);

  if (2 == $size) {
    # 1 2    2 1
    # 3 4 -> 4 3
    my $temp = $$patternRef[0][0];
    $$patternRef[0][0] = $$patternRef[0][1];
    $$patternRef[0][1] = $temp;

    $temp = $$patternRef[1][0];
    $$patternRef[1][0] = $$patternRef[1][1];
    $$patternRef[1][1] = $temp;
  } elsif (3 == $size) {
    # 1 2 3    3 2 1
    # 4 5 6 -> 6 5 4
    # 7 8 9    9 8 7
    my $temp = $$patternRef[0][0];
    $$patternRef[0][0] = $$patternRef[0][2];
    $$patternRef[0][2] = $temp;

    $temp = $$patternRef[1][0];
    $$patternRef[1][0] = $$patternRef[1][2];
    $$patternRef[1][2] = $temp;

    $temp = $$patternRef[2][0];
    $$patternRef[2][0] = $$patternRef[2][2];
    $$patternRef[2][2] = $temp;
  } else {
    die "Wrong size pattern - can't flip!";
  }
}

sub FlipString {
  my $string = shift;
  my $pattern = TranslateStringToSquare($string);
  FlipPattern($pattern);
  return TranslateSquareToString($pattern, scalar(@$pattern), 0, 0);
}

sub AddMatch {
  my $patterns = shift;
  my $line = shift;

  $line =~ m/^(\S+) => (\S+)$/i;
  my ($match, $expansion) = ($1, $2);
  $patterns->{$match} = $expansion;

  my $flipped = FlipString($match);
  $patterns->{$flipped} = $expansion;

  $match = RotateString($match);
  $patterns->{$match} = $expansion;

  $flipped = FlipString($match);
  $patterns->{$flipped} = $expansion;

  $match = RotateString($match);
  $patterns->{$match} = $expansion;

  $flipped = FlipString($match);
  $patterns->{$flipped} = $expansion;

  $match = RotateString($match);
  $patterns->{$match} = $expansion;

  $flipped = FlipString($match);
  $patterns->{$flipped} = $expansion;
}

sub CountPixels {
  my $patternRef = shift;
  my $size = scalar(@$patternRef);

  my $total = 0;
  foreach my $r (0..($size - 1)) {
    foreach my $c (0..($size - 1)) {
      ++$total if $$patternRef[$r][$c] eq '#';
    }
  }

  return $total;
}

sub TranslateSquareToString {
  my $patternRef = shift;
  my $size = shift;
  my $x = shift;
  my $y = shift;

  my @rowStrings;
  foreach my $r (0..($size - 1)) {
    my $rowString = '';
    foreach my $c (0..($size - 1)) {
      $rowString .= $$patternRef[$r + $x][$c + $y];
    }
    push(@rowStrings, $rowString);
  }

  return join('/', @rowStrings);
}

sub TranslateStringToSquare {
  my $string = shift;

  my @rows = split(/\//, $string);
  my $size = scalar(@rows);
  my $pattern = [];
  foreach my $r (0..($size - 1)) {
    my @characters = split(//, $rows[$r]);
    foreach my $c (0..($size - 1)) {
      $$pattern[$r][$c] = $characters[$c];
    }
  }

  return $pattern;
}

sub ExpandTwoByTwo {
  my $grid = shift;
  my $patterns = shift;

  my $size = scalar(@$grid);
  my $blockCount = $size / 2;
  my @newGrid;
  for (my $i = 0; $i < $blockCount; ++$i) {
    for (my $j = 0; $j < $blockCount; ++$j) {
      # Grab a 2x2 block starting at (2i, 2j) and ending at (2i+1, 2j+1)
      my $inputString = TranslateSquareToString($grid, 2, (2 * $i), (2 * $j));
      my $expandedString = $patterns->{$inputString};
      my @block = @{ TranslateStringToSquare($expandedString) };

      # @block is a 3x3 square that will end up in @newGrid at (3i, 3j) through (3i+2, 3j+2)
      foreach my $blockRow (0..2) {
        foreach my $blockCol (0..2) {
          $newGrid[(3 * $i) + $blockRow][(3 * $j) + $blockCol] = $block[$blockRow][$blockCol];
        }
      }
    }
  }

  return @newGrid;
}

sub ExpandThreeByThree {
  my $grid = shift;
  my $patterns = shift;

  my $size = scalar(@$grid);
  my $blockCount = $size / 3;
  my @newGrid;
  for (my $i = 0; $i < $blockCount; ++$i) {
    for (my $j = 0; $j < $blockCount; ++$j) {
      # Grab a 3x3 block starting at (3i, 3j) and ending at (3i+2, 3j+2)
      my $inputString = TranslateSquareToString($grid, 3, (3 * $i), (3 * $j));
      my $expandedString = $patterns->{$inputString};
      my @block = @{ TranslateStringToSquare($expandedString) };

      # @block is a 4x4 square that will end up in @newGrid at (4i, 4j) through (4i+3, 4j+3)
      foreach my $blockRow (0..3) {
        foreach my $blockCol (0..3) {
          $newGrid[(4 * $i) + $blockRow][(4 * $j) + $blockCol] = $block[$blockRow][$blockCol];
        }
      }
    }
  }

  return @newGrid;
}

################################################################################
#                                MAIN PROG                                     #
################################################################################

my $inputFile;
if (scalar(@ARGV) > 0) {
  $inputFile = shift @ARGV;
} else {
  $inputFile = '../input/input.txt';
}

my %patterns = ();
open(my $fh, "<", $inputFile) or die "Couldn't open $inputFile: $!";
while (<$fh>) {
  my $line = $_;
  chomp($line);
  AddMatch(\%patterns, $line);
}
close($fh);

my @grid = (['.', '#', '.'],
            ['.', '.', '#'],
            ['#', '#', '#']);

my $iterations = 0;
my $TOTAL_ITERATIONS = 18;

do {
  if (0 == (scalar(@grid) % 2)) {
    @grid = ExpandTwoByTwo(\@grid, \%patterns);
  } else {
    @grid = ExpandThreeByThree(\@grid, \%patterns);
  }

  ++$iterations;
  my $pixelCount = CountPixels(\@grid);
  print "After $iterations iterations, there are $pixelCount pixels on.\n";
} while ($iterations < $TOTAL_ITERATIONS);
