use strict;
use warnings;

use feature "switch";

use Data::Dumper;

sub Combine;

sub Combine {
  my ($list, $n) = @_;
  return () if ($n > @$list);

  return map [$_], @$list if $n <= 1;

  my @comb;

  for (my $i = 0; $i+$n <= @$list; ++$i) {
    my $val  = $list->[$i];
    my @rest = @$list[$i+1..$#$list];
    push @comb, [$val, @$_] for Combine \@rest, $n-1;
  }

  return @comb;
}

sub GetFloorNum {
  my $floor = shift;
  given ($floor) {
    when (/^first$/i) {
      return 0;
    }
    when (/^second$/i) {
      return 1;
    }
    when (/^third$/i) {
      return 2;
    }
    when (/^fourth$/i) {
      return 3;
    }
    default {
      warn "Unknown floor: $floor";
      return -1;
    }
  }
}

sub GetGeneratorsOnFloor {
  my ($floornum, $aref) = @_;

  my @floors = @{$aref};
  my @generators = ();
  foreach my $obj (@{$floors[$floornum]}) {
    if ($obj =~ m/^G_\w+$/) {
      push(@generators, $obj);
    }
  }

  return @generators;
}

sub GetChipsOnFloor {
  my ($floornum, $aref) = @_;

  my @floors = @{$aref};
  my @chips = ();
  foreach my $obj (@{$floors[$floornum]}) {
    if ($obj =~ m/^C_\w+$/) {
      push(@chips, $obj);
    }
  }

  return @chips;
}

sub ChipMatchesGenerator {
  my ($chip, $generator) = @_;
  $chip =~ m/^C_(\w+)$/;
  return 1 if ($generator eq "G_$1");
  return 0;
}

sub OneFloorIsValid {
  my $aref = shift;
  my @generators = grep { $_ =~ m/^G_\w+$/ } @$aref;
  return 1 if (0 == scalar(@generators));

  my @chips = grep { $_ =~ m/^C_\w+$/ } @$aref;
  return 1 if (0 == scalar(@chips));

  foreach my $c (@chips) {
    my $foundG = 0;
    foreach my $g (@generators) {
      if (ChipMatchesGenerator($c, $g)) {
        $foundG = 1;
        last;
      }
    }
    return 0 if (0 == $foundG);
  }

  return 1;
}

sub FloorIsValid {
  my ($floornum, $aref) = @_;
  my @generators = GetGeneratorsOnFloor($floornum, $aref);
  return 1 if (0 == scalar(@generators));

  my @chips = GetChipsOnFloor($floornum, $aref);
  return 1 if (0 == scalar(@chips));

  foreach my $c (@chips) {
    my $foundG = 0;
    foreach my $g (@generators) {
      if (ChipMatchesGenerator($c, $g)) {
        $foundG = 1;
        last;
      }
    }
    return 0 if (0 == $foundG);
  }

  return 1;
}

sub StateIsValid {
  my $aref = shift;
  foreach my $f (0..3) {
    return 0 if (0 == FloorIsValid($f, $aref));
  }
  return 1;
}

sub GenerateMoves {
  my ($floornum, $aref, $moves) = @_;

  my $newMoves = $moves + 1;

  my @newFloorNums;
  if (0 == $floornum) {
    @newFloorNums = (1);
  } elsif (3 == $floornum) {
    @newFloorNums = (2);
  } else {
    @newFloorNums = ($floornum - 1, $floornum + 1);
  }

  #print "New Floor Nums: ", join(', ', @newFloorNums), "\n";

  my @moves;

  my @floor = @{@{$aref}[$floornum]};
  #print "Floor $floornum: ", join(', ', @floor), "\n";

  my @objectsToMove = Combine(\@floor, 1);
  push(@objectsToMove, Combine(\@floor, 2));

  foreach my $objectTuple (@objectsToMove) {
    #print Dumper $objectTuple;
    my %objecthash = map { $_ => 1 } @$objectTuple;
    my @floorMinusObjects = grep { !exists($objecthash{$_}) } @floor;
    #print "Floor $floornum minus objectTuple: ", join(', ', @floorMinusObjects), "\n";
    if (0 == OneFloorIsValid(\@floorMinusObjects)) {
      #print "Floor is now invalid, skipping.\n";
      next;
    }

    foreach my $newFloorNum (@newFloorNums) {
      # print "Moving to $newFloorNum\n";
      my @newFloor = @{@{$aref}[$newFloorNum]};
      push(@newFloor, @$objectTuple);
      # print "New floor plus objects: ", join(', ', @newFloor), "\n";
      if (0 == OneFloorIsValid(\@newFloor)) {
        # print "New floor would be invalid, skipping.\n";
        next;
      }

      my @newFloorState = @$aref;
      #print "Old state: \n";
      #print Dumper \@newFloorState;
      $newFloorState[$floornum] = \@floorMinusObjects;
      $newFloorState[$newFloorNum] = \@newFloor;
      #print "New state: \n";
      #print Dumper \@newFloorState;
      my @newMove = ($newFloorNum, \@newFloorState, $newMoves);
      #print "Adding move: \n";
      #print Dumper \@newMove;
      push(@moves, \@newMove);
    }
  }


  #foreach my $m (@moves) {
  #  print "Returning move: \n";
  #  print Dumper $m;
  #}

  return @moves;
}

sub FloorStateIsFinal {
  my $aref = shift;
  my ($floor1, $floor2, $floor3) = (scalar(@{$aref}[0]), scalar(@{$aref}[1]), scalar(@{$aref}[2]));
  return 1 if (0 == $floor1 && 0 == $floor2 && 0 == $floor3);
  return 0;
}

my @floors = ([], [], [], []);

my $inputfile = shift;
if (!defined($inputfile)) {
  $inputfile = 'Sample.txt';
}

open(my $fh, '<', $inputfile) or die "Couldn't open $inputfile: $!";
chomp(my @lines = <$fh>);
close($fh);

foreach my $line (@lines) {
  $line =~ s/\R//g; # Trim whitespace
  if ($line =~ m/^The (\w+) floor contains nothing relevant.$/) {
    my $floornum = GetFloorNum($1);
    # print "Floor $floornum is initially empty.\n";
    next;
  }

  if ($line !~ m/^The (\w+) floor contains/) {
    # print "Unrecognized input line: '$line'\n";
    next;
  }

  my $floornum = GetFloorNum($1);

  my @generators = $line =~ m/a (\w+) generator/g;
  foreach my $gen (@generators) {
    push($floors[$floornum], "G_$gen");
  }

  my @chips = $line =~ m/a (\w+)-compatible microchip/g;
  foreach my $chip (@chips) {
    push($floors[$floornum], "C_$chip");
  }
}

# State is current floor number, current floor state, and number of moves
my $currentFloor = 0;
my $numMoves = 0;

my @moves = GenerateMoves($currentFloor, \@floors, $numMoves);

my %moveNumsProcessed = (0 => 1);
my @floorStatesSeen = (\@floors);

sub SeenFloorState {
  my $aref = shift;
  foreach my $oldState (@floorStatesSeen) {
    foreach my $f (0..3) {
      foreach my $oldObj (@{@{$oldState}[$f]}) {
        my $found = 0;
        foreach my $newObj (@{@{$aref}[$f]}) {
          if ($oldObj eq $newObj) {
            $found = 1;
            last;
          }
        }
        return 0 if (0 == $found);
      }
    }
  }
  return 1;
}

while (0 < scalar(@moves)) {
  my $moveTuple = shift(@moves);
  my ($currentFloor, $currentFloorState, $moveNum) = @$moveTuple;

  if (!exists($moveNumsProcessed{$moveNum})) {
    print "Now processing move: $moveNum\n";
    $moveNumsProcessed{$moveNum} = 1;
  }

  next if (1 == SeenFloorState($currentFloorState));

  if (1 == FloorStateIsFinal($currentFloorState)) {
    print "Found a final state that took $moveNum moves\n";
    last;
  }

  push(@moves, GenerateMoves($currentFloor, $currentFloorState, $moveNum));
}

print "Done.\n";
