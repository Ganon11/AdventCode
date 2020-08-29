use strict;
use warnings;

use Data::Dumper;

sub PrintRoom {
  my $aref = shift;
  foreach my $row (@$aref) {
    print join('', @$row), "\n";
  }
}

sub GetTileType {
  my ($left, $center, $right) = @_;
  return '^' if ($left eq '^' and $center eq '^' and $right eq '.');
  return '^' if ($left eq '.' and $center eq '^' and $right eq '^');
  return '^' if ($left eq '^' and $center eq '.' and $right eq '.');
  return '^' if ($left eq '.' and $center eq '.' and $right eq '^');
  return '.';
}

my $inputfile = shift;
if (!defined($inputfile)) {
  $inputfile = 'sample1.txt';
}

open(my $fh, '<', $inputfile) or die "Couldn't open $inputfile: $!";
my $line = <$fh>;
close($fh);
$line =~ s/\R//g;

my @room = ([]);
my $rowsize = length($line);
my $colSize = 400000;
my $safeTotal = 0;
foreach my $newTile (split(//, $line)) {
  push($room[0], $newTile);
  $safeTotal = $safeTotal + 1 if ($newTile eq '.');
}

for (my $i = 1; $i < $colSize; ++$i) {
  for (my $j = 0; $j < $rowsize; ++$j) {
    my $newTile;
    if ($j == 0) {
      $newTile = GetTileType('.', $room[$i - 1][$j], $room[$i - 1][$j + 1]);
    }
    elsif ($j == $rowsize - 1) {
      $newTile = GetTileType($room[$i - 1][$j - 1], $room[$i - 1][$j], '.');
    }
    else {
      $newTile = GetTileType($room[$i - 1][$j - 1], $room[$i - 1][$j], $room[$i - 1][$j + 1]);
    }

    $safeTotal = $safeTotal + 1 if ($newTile eq '.');
    $room[$i][$j] = $newTile;
  }
}

# PrintRoom(\@room);
print "Total Safe Tiles: $safeTotal\n";
