use strict;
use warnings;

sub genSpiralPart2
{
  my $num = shift;
  my ($x, $y) = (0, 0);
  my $stepCount = 1;
  my $stepCountChange = 1;
  my $direction = 'E';
  my %values = ('0,0' => 1);

  my $limit = 100;
  my $iteration = 0;
  while ($iteration < $limit) {
    for (my $j = 0; $j < $stepCount; ++$j) {
      if ($direction eq 'E') {
        $x = $x + 1;
      } elsif ($direction eq 'N') {
        $y = $y + 1;
      } elsif ($direction eq 'W') {
        $x = $x - 1;
      } elsif ($direction eq 'S') {
        $y = $y - 1;
      } else {
        die "wtf";
      }

      my $sum = 0;
      for (my $a = $x - 1; $a <= $x + 1; ++$a) {
        for (my $b = $y - 1; $b <= $y + 1; ++$b) {
          next if ($a == $x and $b == $y);
          #print "Checking ($a,$b)";
          if (exists($values{"$a,$b"})) {
            $sum = $sum + $values{"$a,$b"};
          }
        }
      }

      return $sum if ($sum > $num);

      $values{"$x,$y"} = $sum;
    }

    if ($direction eq 'E') {
      $direction = 'N';
    } elsif ($direction eq 'N') {
      $direction = 'W';
    } elsif ($direction eq 'W') {
      $direction = 'S';
    } elsif ($direction eq 'S') {
      $direction = 'E';
    } else {
      die "wtf";
    }

    if ($stepCountChange == 1) {
      $stepCountChange = 0;
    } else {
      $stepCountChange = 1;
      $stepCount += 1;
    }

    $iteration = $iteration + 1;
  }
}

my $inputFile;
if (scalar(@ARGV) > 0) {
  $inputFile = shift @ARGV;
} else {
  $inputFile = '../input/input.txt';
}

open(my $fh, "<", $inputFile) or die "Couldn't open $inputFile: $!";
my $input = <$fh>;
close($fh);
chomp($input);
$input = int($input);

if ($input == 1) {
  print "Distance: 0\n";
  exit(0);
} else {
  my @lowerRightCorners = ();
  my $num = 1;
  do {
    push @lowerRightCorners, ($num * $num);
    $num = $num + 2;
  } while (($num * $num) < $input);
  push @lowerRightCorners, ($num * $num);

  my $square = scalar(@lowerRightCorners) - 1;
  my $maxDistance = $square * 2;

  my $sideLength = ($square * 2);
  my $wonkyDifference = $sideLength / 2;
  my $wonkyAddition = -1;
  $num = $num * $num;
  while ($num > $input) {
    $wonkyAddition = $wonkyAddition * -1;
    $num = $num - $wonkyDifference;
  }

  my $actualDistince = $square + ($wonkyAddition * ($input - $num));
  print "Distance: $actualDistince\n";
}

my $answerPart2 = genSpiralPart2(347991);
print "$answerPart2\n";