use strict;
use warnings;
use Digest::MD5 qw(md5_hex);

sub IsOpen {
  my $char = shift;
  return 1 if ($char eq 'b' or $char eq 'c' or $char eq 'd' or $char eq 'e' or $char eq 'f');
  return 0;
}

my $inputfile = shift;
if (!defined($inputfile)) {
  $inputfile = 'sample1.txt';
}

open(my $fh, '<', $inputfile) or die "Couldn't open $inputfile: $!";
my $code = <$fh>;
close($fh);
$code =~ s/\R//g;

my @paths = ();
my @states = ([0, 0, '']);

while (0 < scalar(@states)) {
  my ($x, $y, $pathSoFar) = @{shift(@states)};

  if ($x == 3 and $y == 3) {
    push(@paths, $pathSoFar);
    next;
  }

  my $digest = md5_hex($code . $pathSoFar);
  my @chars = split(//, $digest);

  # Up
  if ($y > 0 && IsOpen($chars[0])) {
    push(@states, [$x, $y - 1, $pathSoFar . 'U']);
  }

  # Down
  if ($y < 3 && IsOpen($chars[1])) {
    push(@states, [$x, $y + 1, $pathSoFar . 'D']);
  }

  # Left
  if ($x > 0 && IsOpen($chars[2])) {
    push(@states, [$x - 1, $y, $pathSoFar . 'L']);
  }

  # Right
  if ($x < 3 && IsOpen($chars[3])) {
    push(@states, [$x + 1, $y, $pathSoFar . 'R']);
  }
}

my @sortedPaths = sort { length($b) <=> length($a) } @paths;
my $shortestLength = length($sortedPaths[-1]);
print "Shortest path length: $shortestLength ($sortedPaths[-1])\n";
my $longestLength = length($sortedPaths[0]);
print "Longest path length: $longestLength ($sortedPaths[0])\n";
