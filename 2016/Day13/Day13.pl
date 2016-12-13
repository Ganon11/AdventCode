use strict;
use warnings;

use Term::Screen::Uni;
use Time::HiRes qw(usleep);

# Lovingly stolen from http://docstore.mik.ua/orelly/perl/cookbook/ch02_05.htm
sub dec2bin {
    my $str = unpack("B32", pack("N", shift));
    $str =~ s/^0+(?=\d)//; # otherwise you'll get leading zeros
    return $str;
}

sub CoordinateIsOpen {
  my ($x, $y, $favorite) = @_;
  my $num = ($x * $x) + (3 * $x) + (2 * $x * $y) + $y + ($y * $y) + $favorite;
  my $binstr = dec2bin($num);
  my $total = 0;
  foreach my $char (split(//, $binstr)) {
    if ($char eq '1') {
      ++$total;
    }
  }

  return 1 if ($total % 2 == 0); # Not a wall
  return 0; # It's a wall!
}

sub SeenPosition {
  my ($x, $y, $aref) = @_;

  foreach my $pos (@$aref) {
    return 1 if (@$pos[0] == $x && @$pos[1] == $y);
  }

  return 0;
}

my $favoriteNumber = 1362; # Puzzle Input
my ($destX, $destY) = (31, 39); # Puzzle
my ($startingX, $startingY) = (1, 1);

my $part = shift;
if (!defined($part)) {
  $part = '1';
}

my @queue = ([$startingX, $startingY, 0]); # State is (x, y) and steps taken
my @seenPositions;

my $scr = Term::Screen::Uni->new();
die "Something is wrong :(" unless ($scr);
$scr->clrscr();
$scr->at($destY, $destX);
$scr->puts('X'); # X marks the spot!

my $maxY = $destY;

while (0 < scalar(@queue)) {
  my $state = shift(@queue);
  my ($x, $y, $steps) = @$state;

  usleep(10000);

  if ($y > $maxY) {
    $maxY = $y;
  }

  # Don't enter the void
  #next if ($x < 0 || $y < 0);

  # If we're at the destination, we're done!
  if ($part eq '1' && $x == $destX && $y == $destY) {
    $scr->at($maxY + 2, 0);
    print "Found it in $steps steps\n";
    last;
  }

  # More than 50 steps away
  if ($part eq '2' && $steps > 50) {
    $scr->at($maxY + 2, 0);
    my $unique = scalar(@seenPositions);
    print "Visited $unique locations\n";
    last;
  }

  # Don't revisit places
  next if (1 == SeenPosition($x, $y, \@seenPositions));

  # Add current position to the cache
  push(@seenPositions, [$x, $y]);

  $scr->at($y, $x);
  $scr->puts('.');

  # Move down
  if (CoordinateIsOpen($x, $y + 1, $favoriteNumber)) {
    push(@queue, [$x, $y + 1, $steps + 1]);
  } else {
    $scr->at($y + 1, $x);
    $scr->puts('#');
  }

  # Move right
  if (CoordinateIsOpen($x + 1, $y, $favoriteNumber)) {
    push(@queue, [$x + 1, $y, $steps + 1]);
  } else {
    $scr->at($y, $x + 1);
    $scr->puts('#');
  }

  # Move up
  if ($y > 0 && CoordinateIsOpen($x, $y - 1, $favoriteNumber)) {
    push(@queue, [$x, $y - 1, $steps + 1]);
  } else {
    $scr->at($y - 1, $x);
    $scr->puts('#');
  }

  # Move left
  if ($x > 0 && CoordinateIsOpen($x - 1, $y, $favoriteNumber)) {
    push(@queue, [$x - 1, $y, $steps + 1]);
  } else {
    $scr->at($y, $x - 1);
    $scr->puts('#');
  }
}
