use strict;
use warnings;

my $inputFile;
if (scalar(@ARGV) > 0) {
  $inputFile = shift @ARGV;
} else {
  $inputFile = '../input/Input.txt';
}

my $char;
my $depth = 0;
my $score = 0;
my $garbageCount = 0;
my $readingGarbage = 0;
open(my $fh, "<", $inputFile) or die "Couldn't open $inputFile: $!";
while ($char = getc($fh)) {
  if ($char eq '{') {
    if ($readingGarbage) {
      $garbageCount = $garbageCount + 1;
    } else {
      # New group is beginning.
      $depth = $depth + 1;
    }
  } elsif ($char eq '}') {
    if ($readingGarbage) {
      $garbageCount = $garbageCount + 1;
    } else {
      # Group has ended - add score and reduce depth.
      $score = $score + $depth;
      $depth = $depth - 1;
    }
  } elsif ($char eq '<') {
    if ($readingGarbage) {
      $garbageCount = $garbageCount + 1;
    }
    # Beginning of garbage.
    $readingGarbage = 1;
  } elsif ($char eq '>') {
    # End of garbage.
    $readingGarbage = 0;
  } elsif ($char eq '!') {
    # Ignore the next character.
    getc($fh);
  } else {
    # Character does nothing - ignore.
    if ($readingGarbage) {
      $garbageCount = $garbageCount + 1;
    }
  }
}
close($fh);

print "Total score: $score\n";
print "Garbage Count: $garbageCount\n";
