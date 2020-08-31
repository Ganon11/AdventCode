use strict;
use warnings;

use Term::Screen::Uni;
use Time::HiRes qw(usleep);

my $ROWS = 6;
my $COLUMNS = 50;
my $SLEEP = 1000;

sub Rectangle {
  my ($screenref, $a, $b) = @_;

  for (my $x = 0; $x < $a; ++$x) {
    for (my $y = 0; $y < $b; ++$y) {
      @{@$screenref[$x]}[$y] = 1;
    }
  }

  PrintScreen($screenref);
}

sub RotateRow {
  my ($screenref, $y, $a) = @_;

  for (my $i = 0; $i < $a; ++$i) {
    my $tmp = @{@$screenref[-1]}[$y];
    for (my $x = $COLUMNS - 1; $x > 0; --$x) {
      @{@$screenref[$x]}[$y] = @{@$screenref[$x - 1]}[$y];
    }
    @{@$screenref[0]}[$y] = $tmp;
  }

  PrintScreen($screenref);
}

sub RotateColumn {
  my ($screenref, $x, $a) = @_;

  for (my $i = 0; $i < $a; ++$i) {
    my $tmp = @{@$screenref[$x]}[-1];
    for (my $y = $ROWS - 1; $y > 0; --$y) {
      @{@$screenref[$x]}[$y] = @{@$screenref[$x]}[$y - 1];
    }
    @{@$screenref[$x]}[0] = $tmp;
  }

  PrintScreen($screenref);
}

sub PrintScreen {
  my $screenref = shift;
  my $total = 0;

  my $scr = Term::Screen::Uni->new();
  die "Something is wrong :(\n" unless ($scr);

  $scr->clrscr();
  $scr->rows($ROWS);
  $scr->cols($COLUMNS);
  for (my $y = 0; $y < $ROWS; ++$y) {
    for (my $x = 0; $x < $COLUMNS; ++$x) {
      my $val = @{@$screenref[$x]}[$y];
      if ($val == 1) {
        $scr->at($y, $x);
        $scr->puts('#');
        $total = $total + 1;
      }
    }
  }
  $scr->at($ROWS, 0);
  usleep($SLEEP);

  return $total;
}

my $inputfile = shift;
if (!defined($inputfile)) {
  $inputfile = 'sample.txt';
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

my $total = PrintScreen(\@screen);
print "Total lit pixels: $total\n";
