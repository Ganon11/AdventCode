use strict;
use warnings;
use Switch;
use Data::Dumper;

sub GetTapeValue {
  my $tape = shift;
  my $cursor = shift;
  if (!exists($tape->{$cursor})) {
    $tape->{$cursor} = 0;
  }

  return $tape->{$cursor};
}

sub Move {
  my $cursor = shift;
  my $direction = shift;

  switch ($direction) {
    case 'right' { return $cursor + 1; }
    case 'left' { return $cursor - 1; }
    else { die "Unknown move direction $direction" }
  }
}

my $inputFile;
if (scalar(@ARGV) > 0) {
  $inputFile = shift @ARGV;
} else {
  $inputFile = '../input/input.txt';
}

open(my $fh, "<", $inputFile) or die "Couldn't open $inputFile: $!";

my $line = <$fh>;
$line =~ m/Begin in state (\w+)./i;
my $currentState = $1;

$line = <$fh>;
$line =~ m/Perform a diagnostic checksum after (\d+) steps./i;
my $TOTAL_STEPS = int($1);

my %states = ();

while (<$fh>) {
  # First line is blank.
  $line = <$fh>;
  $line =~ m/In state (\w+):/i;
  my $state = $1;

  # Read one transition rule
  $line = <$fh>;
  $line =~ m/If the current value is ([01]):/i;
  my $currValue = int($1);

  $line = <$fh>;
  $line =~ m/Write the value ([01])./i;
  my $writeValue = int($1);

  $line = <$fh>;
  $line =~ m/Move one slot to the (left|right)./i;
  my $moveDirection = $1;

  $line = <$fh>;
  $line =~ m/Continue with state (\w+)./i;
  my $nextState = $1;

  $states{$state}{$currValue}{'write'} = $writeValue;
  $states{$state}{$currValue}{'move'} = $moveDirection;
  $states{$state}{$currValue}{'next'} = $nextState;

  # Read another transition rule
  $line = <$fh>;
  $line =~ m/If the current value is ([01]):/i;
  $currValue = int($1);

  $line = <$fh>;
  $line =~ m/Write the value ([01])./i;
  $writeValue = int($1);

  $line = <$fh>;
  $line =~ m/Move one slot to the (left|right)./i;
  $moveDirection = $1;

  $line = <$fh>;
  $line =~ m/Continue with state (\w+)./i;
  $nextState = $1;

  $states{$state}{$currValue}{'write'} = $writeValue;
  $states{$state}{$currValue}{'move'} = $moveDirection;
  $states{$state}{$currValue}{'next'} = $nextState;
}
close($fh);

my %tape = ();
my $cursor = 0;
# print Dumper(\%states);

for (my $step = 0; $step < $TOTAL_STEPS; ++$step) {
  my $currentTapeValue = GetTapeValue(\%tape, $cursor);
  my $write = $states{$currentState}{$currentTapeValue}{'write'};
  my $move = $states{$currentState}{$currentTapeValue}{'move'};
  my $next = $states{$currentState}{$currentTapeValue}{'next'};

  $tape{$cursor} = $write;
  $cursor = Move($cursor, $move);
  $currentState = $next;
}

my $diagnosticChecksum = grep { $_ == 1 } values(%tape);
print "After $TOTAL_STEPS steps, diagnostic checksum is $diagnosticChecksum\n";