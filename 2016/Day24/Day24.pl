use strict;
use warnings;

use Data::Dumper;

sub PrintGrid {
  my $aref = shift;
  foreach my $row (@$aref) {
    foreach my $char (@$row) {
      print "$char";
    }
    print "\n";
  }
}

sub HasSeenState {
  my ($statestring, $aref) = @_;

  foreach my $oldStateString (@$aref) {
    return 1 if $oldStateString eq $statestring;
  }

  return 0;
}

sub IsFinalState {
  my $stateref = shift;

  foreach my $objective (1..9) {
    next if !defined($stateref->{$objective});
    return 0 if $stateref->{$objective} == 0;
  }

  return 1;
}

sub StateToString {
  my $stateref = shift;
  my %state = %{$stateref};
  my ($x, $y, $steps) = ($state{'x'}, $state{'y'}, $state{'steps'});
  #my $output = "($x, $y) in $steps steps, ";
  my $output = "($x, $y): ";

  my @objectives;
  foreach my $key (sort keys %state) {
    if ($key =~ m/^\d$/) {
      push(@objectives, $state{$key});
    }
  }

  $output .= join(', ', @objectives);

  print "$output\n";
  return $output;
}

my $inputfile = shift;
if (!defined($inputfile)) {
  $inputfile = 'sample.txt';
}

open(my $fh, '<', $inputfile) or die "Couldn't open $inputfile: $!";
my @grid;
my @objectives;
my ($startingX, $startingY, $numRows, $numCols) = (undef, undef, 0, undef);
foreach my $line (<$fh>) {
  $line =~ s/\R//g;
  ++$numRows;
  if (!defined($numCols)) {
    $numCols = length($line);
  }

  my $index = index($line, '0');
  if ($index != -1) {
    $startingX = scalar(@grid);
    $startingY = $index;
  }

  foreach my $match ($line =~ m/[1-9]/g) {
    push(@objectives, $match);
  }

  push(@grid, [split(//, $line)]);
}
close($fh);

print "Grid: $numRows x $numCols\n";
PrintGrid(\@grid);
print "Starting at: [$startingX, $startingY]\n";

my @statesSeen;

my %startingState = (
  'x' => $startingX,
  'y' => $startingY,
  'steps' => 0
);

foreach my $objective (@objectives) {
  $startingState{$objective} = 0;
}

my @states = (\%startingState);
while (scalar(@states) > 0) {
  my $currentState = shift(@states);
  my $stateAsString = StateToString($currentState);
  push(@statesSeen, StateToString($currentState));
  #print "$stateAsString\n";

  my $steps = $currentState->{'steps'};
  if (1 == IsFinalState($currentState)) {
    print "Reached all objectives in $steps steps\n";
    last;
  }

  my ($x, $y) = ($currentState->{'x'}, $currentState->{'y'});

  # Move up (y - 1)
  if ($grid[$x][$y - 1] ne '#') {
    my $newState = {
      'x' => $x,
      'y' => $y - 1,
      'steps' => $steps + 1
    };

    foreach my $objective (@objectives) {
      if ($objective eq $grid[$x][$y - 1]) {
        $newState->{$objective} = 1;
      }
      else {
        $newState->{$objective} = $currentState->{$objective};
      }
    }

    if (0 == HasSeenState(StateToString($newState), \@statesSeen)) {
      push(@states, $newState);
    }
  }

  # Move down (y + 1)
  if ($grid[$x][$y + 1] ne '#') {
    my $newState = {
      'x' => $x,
      'y' => $y + 1,
      'steps' => $steps + 1
    };

    foreach my $objective (@objectives) {
      if ($objective eq $grid[$x][$y + 1]) {
        $newState->{$objective} = 1;
      }
      else {
        $newState->{$objective} = $currentState->{$objective};
      }
    }

    if (0 == HasSeenState(StateToString($newState), \@statesSeen)) {
      push(@states, $newState);
    }
  }

  # Move left (x - 1)
  if ($grid[$x - 1][$y] ne '#') {
    my $newState = {
      'x' => $x - 1,
      'y' => $y,
      'steps' => $steps + 1
    };

    foreach my $objective (@objectives) {
      if ($objective eq $grid[$x - 1][$y]) {
        $newState->{$objective} = 1;
      }
      else {
        $newState->{$objective} = $currentState->{$objective};
      }
    }

    if (0 == HasSeenState(StateToString($newState), \@statesSeen)) {
      push(@states, $newState);
    }
  }

  # Move right (x + 1)
  if ($grid[$x + 1][$y] ne '#') {
    my $newState = {
      'x' => $x + 1,
      'y' => $y,
      'steps' => $steps + 1
    };

    foreach my $objective (@objectives) {
      if ($objective eq $grid[$x + 1][$y]) {
        $newState->{$objective} = 1;
      }
      else {
        $newState->{$objective} = $currentState->{$objective};
      }
    }

    if (0 == HasSeenState(StateToString($newState), \@statesSeen)) {
      push(@states, $newState);
    }
  }
}
