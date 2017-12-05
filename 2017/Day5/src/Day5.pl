use strict;
use warnings;

sub CalculateSteps {
  my $instructionsRef = shift;
  my $newOffsetFunc = shift;
  my @instructions = @{$instructionsRef};

  my $instructionPointer = 0;
  my $steps = 0;
  while ($instructionPointer >= 0 and $instructionPointer < scalar(@instructions)) {
    my $oldInstruction = $instructionPointer;
    my $offset = $instructions[$oldInstruction];
    $instructionPointer = $instructionPointer + $instructions[$oldInstruction];
    $instructions[$oldInstruction] = &$newOffsetFunc($offset);
    $steps = $steps + 1;
  }

  return $steps;
}

my $inputFile;
if (scalar(@ARGV) > 0) {
  $inputFile = shift @ARGV;
} else {
  $inputFile = '../input/Input.txt';
}

my @instructions = ();
open(my $fh, "<", $inputFile) or die "Couldn't open $inputFile: $!";
while (<$fh>) {
  my $line = $_;
  chomp($line);
  push(@instructions, int($line));
}
close($fh);

my $part1 = CalculateSteps(\@instructions, sub {
  my $offset = shift;
  return $offset + 1;
});
print "Part 1: Made it out in $part1 steps\n";

my $part2 = CalculateSteps(\@instructions, sub {
  my $offset = shift;
  if ($offset >= 3) {
    return $offset - 1;
  } else {
    return $offset + 1;
  }
});
print "Part 2: Made it out in $part2 steps\n";
