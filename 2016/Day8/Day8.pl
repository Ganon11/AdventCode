use strict;
use warnings;

use Data::Dumper;

my $ROWS = 6;
my $COLUMNS = 50;

sub Rectangle {
  my ($screenref, $a, $b) = @_;

  for (my $x = 0; $x < $a; ++$x) {
    for (my $y = 0; $y < $b; ++$y) {
      @{@$screenref[$x]}[$y] = 1;
    }
  }
}

sub RotateRow {
  my ($screenref, $y, $a) = @_;

  my @values = ();
  for (my $x = 0; $x < $COLUMNS; ++$x) {
    push(@values, @{@$screenref[$x]}[$y]);
  }

  for (my $i = 0; $i < $a; ++$i) {
    unshift(@values, pop(@values));
  }

  for (my $x = 0; $x < $COLUMNS; ++$x) {
    @{@$screenref[$x]}[$y] = $values[$x];
  }
}

sub RotateColumn {
  my ($screenref, $x, $a) = @_;

  my @values = ();
  for (my $y = 0; $y < $ROWS; ++$y) {
    push(@values, @{@$screenref[$x]}[$y]);
  }

  for (my $i = 0; $i < $a; ++$i) {
    unshift(@values, pop(@values));
  }

  for (my $y = 0; $y < $ROWS; ++$y) {
    @{@$screenref[$x]}[$y] = $values[$y];
  }
}

sub PrintScreen {
  my $screenref = shift;
  for (my $y = 0; $y < $ROWS; ++$y) {
    for (my $x = 0; $x < $COLUMNS; ++$x) {
      my $val = @{@$screenref[$x]}[$y];
      if ($val == 1) {
        print '#';
      } else {
        print '.';
      }
    }
    print "\n";
  }
}

sub PixelCount {
  my $screenref = shift;
  my $total = 0;

  for (my $y = 0; $y < $ROWS; ++$y) {
    for (my $x = 0; $x < $COLUMNS; ++$x) {
      my $val = @{@$screenref[$x]}[$y];
      if ($val == 1) {
        $total = $total + 1;
      }
    }
  }

  return $total;
}

my $inputfile = shift;
if (!defined($inputfile)) {
  $inputfile = 'Sample.txt';
}

my @screen;
for (my $x = 0; $x < $COLUMNS; ++$x) {
  for (my $y = 0; $y < $ROWS; ++$y) {
    $screen[$x][$y] = 0;
  }
}

open(my $fh, '<', $inputfile) or die "Couldn't open $inputfile: $!";
chomp(my @lines = <$fh>);
close($fh);

foreach my $command (@lines) {
  $command =~ s/\R//g; # Trim newlines
  if ($command =~ m/^rect (\d+)x(\d+)$/) {
    Rectangle(\@screen, $1, $2);
  }
  elsif ($command =~ m/^rotate row y=(\d+) by (\d+)$/) {
    RotateRow(\@screen, $1, $2);
  }
  elsif ($command =~ m/^rotate column x=(\d+) by (\d+)$/) {
    RotateColumn(\@screen, $1, $2);
  }
  else {
    print "Unrecognized command: $command\n";
  }
}

print "Final screen reads as follows:\n";
PrintScreen(\@screen);
my $total = PixelCount(\@screen);
print "Total lit pixels: $total\n";
