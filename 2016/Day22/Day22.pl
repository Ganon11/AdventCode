use strict;
use warnings;

my @nodes;
open(my $fh, '<', 'input.txt');
foreach my $line (<$fh>) {
  $line =~ s/\R//g;
  if ($line =~ m/^\/dev\/grid\/node-x(\d+)-y(\d+)\s+(\d+)T\s+(\d+)T\s+(\d+)T\s+(\d+)%$/) {
    my ($x, $y, $size, $used, $avail, $usePercent) = (int($1), int($2), int($3), int($4), int($5), int($6));
    $nodes[$x][$y] = {
      'size' => $size,
      'used' => $used,
      'avail' => $avail,
      'use%' => $usePercent
    };
  }
}
close($fh);

my @viable;
my $rowCount = scalar(@nodes);
my $columnCount = scalar(@{$nodes[0]});
for (my $x1 = 0; $x1 < $rowCount; ++$x1) {
  for (my $y1 = 0; $y1 < $columnCount; ++$y1) {
    my $A = $nodes[$x1][$y1];
    for (my $x2 = 0; $x2 < $rowCount; ++$x2) {
      for (my $y2 = 0; $y2 < $columnCount; ++$y2) {
        next if $x1 == $x2 && $y1 == $y2;

        my $B = $nodes[$x2][$y2];
        if ($A->{'used'} > 0 && $A->{'used'} < $B->{'avail'}) {
          push(@viable, "($x1, $y1) -> ($x2, $y2)");
        }
      }
    }
  }
}

my $numViable = scalar(@viable);
print "Viable nodes: $numViable\n";

# First line - 10s place
print '   ';
for (my $c = 0; $c < $rowCount; ++$c) {
  my $legend = int($c / 10);
  print "$legend ";
}
print "\n";

# Second line - 1s place
print '   ';
for (my $c = 0; $c < $rowCount; ++$c) {
  my $legend = ($c % 10);
  print "$legend ";
}
print "\n";

for (my $y = 0; $y < $columnCount; ++$y) {
  print ' ' if $y < 10;
  print "$y ";
  for (my $x = 0; $x < $rowCount; ++$x) {
    if ($x == $rowCount - 1 && $y == 0) {
      print 'G ';
    }
    elsif ($nodes[$x][$y]->{'use%'} > 90) {
      print '# ';
    }
    elsif ($nodes[$x][$y]->{'use%'} < 10) {
      print '_ ';
    }
    else {
      print '. ';
    }
  }

  print "\n";
}