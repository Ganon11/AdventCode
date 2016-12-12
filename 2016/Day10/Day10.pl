use strict;
use warnings;
use List::Util qw(max min);
use Data::Dumper;

my $inputfile = shift;
$inputfile = 'Sample.txt' if not defined($inputfile);
open(my $fh, '<', $inputfile) or die "Couldn't open $inputfile: $!";
my @lines = <$fh>;
close($fh);

my %bots = (); # Hash of bot id => list of values
my %commands = (); # Hash of bot id => tuple of command parameters
my @pendingCommands = (); # List of command parameters than can be executed [bot, lowType, lowDest, highType, highDest]
my %outputs = (); # Hash of output bin number => list of outputted values

foreach my $line (@lines) {
  $line =~ s/\R//g; # Trim newlines
  if ($line =~ m/^value (\d+) goes to bot (\d+)$/) {
    my ($val, $bot) = (int($1), int($2));
    if (!defined($bots{$bot})) {
      $bots{$bot} = [$val];
    } else {
      push($bots{$bot}, $val);
      if (defined($commands{$bot})) {
        my ($lowDestType, $lowDestNum, $highDestType, $highDestNum) = @{$commands{$bot}};
        push(@pendingCommands, [$bot, $lowDestType, $lowDestNum, $highDestType, $highDestNum]);
        delete($commands{$bot});
      }
    }
  } elsif ($line =~ m/^bot (\d+) gives low to (\w+) (\d+) and high to (\w+) (\d+)$/) {
    my ($source, $lowDestType, $lowDestNum, $highDestType, $highDestNum) = (int($1), $2, int($3), $4, int($5));
    if (!defined($commands{$source})) {
      $commands{$source} = [$lowDestType, $lowDestNum, $highDestType, $highDestNum];
    } else {
      push($commands{$source}, [$lowDestType, $lowDestNum, $highDestType, $highDestNum]);
    }
  }
}

my $pendingCommandsLen = scalar(@pendingCommands);
while ($pendingCommandsLen > 0) {
  my ($source, $lowType, $lowDest, $highType, $highDest) = @{shift(@pendingCommands)};
  my @vals = @{$bots{$source}};
  delete($bots{$source});
  my ($low, $high) = (min(@vals), max(@vals));

  if ($low == 17 && $high == 61) {
    print "We found it: bot $source\n";
    #last;
  }

  if ($lowType eq 'bot') {
    if (!defined($bots{$lowDest})) {
      $bots{$lowDest} = [$low];
    } else {
      push($bots{$lowDest}, $low);
      if (defined($commands{$lowDest})) {
        my ($lowDestType, $lowDestNum, $highDestType, $highDestNum) = @{$commands{$lowDest}};
        push(@pendingCommands, [$lowDest, $lowDestType, $lowDestNum, $highDestType, $highDestNum]);
        delete($commands{$lowDest});
      }
    }
  } elsif ($lowType eq 'output') {
    if (!defined($outputs{$lowDest})) {
      $outputs{$lowDest} = ($low);
    } else {
      push($outputs{$lowDest}, $low);
    }
  }

  if ($highType eq 'bot') {
    if (!defined($bots{$highDest})) {
      $bots{$highDest} = [$high];
    } else {
      push($bots{$highDest}, $high);
      if (defined($commands{$highDest})) {
        my ($lowDestType, $lowDestNum, $highDestType, $highDestNum) = @{$commands{$highDest}};
        push(@pendingCommands, [$highDest, $lowDestType, $lowDestNum, $highDestType, $highDestNum]);
        delete($commands{$highDest});
      }
    }
  } elsif ($highType eq 'output') {
    if (!defined($outputs{$highDest})) {
      $outputs{$highDest} = ($high);
    } else {
      push($outputs{$highDest}, $high);
    }
  }

  $pendingCommandsLen = scalar(@pendingCommands);
}

print "Outputs: ";
print Dumper \%outputs;

if (defined($outputs{0}) && defined($outputs{1}) && defined($outputs{2})) {
  my $result = int($outputs{0}) * int($outputs{1}) * int($outputs{2});
  print "Result: $result\n";
}
