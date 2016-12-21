use strict;
use warnings;

sub RotateOnLetter {
  my ($str, $letter) = @_;

  my $len = length($str);
  my $index = index($str, $letter);
  my $steps = (1 + $index);
  if ($index > 3) {
    ++$steps;
  }

  for (my $i = 0; $i < $steps; ++$i) {
    $str = substr($str, $len - 1, 1) . substr($str, 0, $len - 1);
  }

  return $str;
}

my $password = shift;
if (!defined($password)) {
  $password = 'abcde';
}

my $inputfile = shift;
if (!defined($inputfile)) {
  $inputfile = 'Sample.txt';
}

print "Original: $password\n";

my @instructions = ();
my $scrambled = $password;
my $len = length($scrambled);
open(my $fh, '<', $inputfile);
foreach (<$fh>) {
  $_ =~ s/\R//g;
  if ($_ =~ m/^swap position (\d+) with position (\d+)$/) {
    my ($from, $to) = ($1, $2);
    push(@instructions, ['swapp', $from, $to]);
  }
  elsif ($_ =~ m/^swap letter (\w) with letter (\w)$/) {
    my ($letter1, $letter2) = ($1, $2);
    push(@instructions, ['swapl', $letter1, $letter2]);
  }
  elsif ($_ =~ m/^rotate (left|right) (\d+) step(?:s)?$/) {
    my ($direction, $steps) = ($1, $2);
    push(@instructions, ['rotate', $direction, $steps]);
  }
  elsif ($_ =~ m/^rotate based on position of letter (\w)$/) {
    my $letter = $1;
    push(@instructions, ['rotatep', $letter]);
  }
  elsif ($_ =~ m/^reverse positions (\d+) through (\d+)$/) {
    my ($from, $to) = ($1, $2);
    push(@instructions, ['reverse', $from, $to]);
  }
  elsif ($_ =~ m/^move position (\d+) to position (\d+)$/) {
    my ($from, $to) = ($1, $2);
    push(@instructions, ['move', $from, $to]);
  }
}
close($fh);

foreach my $instructionRef (@instructions) {
  my $instruction = $instructionRef->[0];
  if ($instruction eq 'swapp') {
    my ($from, $to) = ($instructionRef->[1], $instructionRef->[2]);
    my $tmp = substr($scrambled, $to, 1);
    substr($scrambled, $to, 1) = substr($scrambled, $from, 1);
    substr($scrambled, $from, 1) = $tmp;
  }
  elsif ($instruction eq 'swapl') {
    my ($letter1, $letter2) = ($instructionRef->[1], $instructionRef->[2]);
    $scrambled =~ s/$letter1/./g;
    $scrambled =~ s/$letter2/$letter1/g;
    $scrambled =~ s/\./$letter2/g;
  }
  elsif ($instruction eq 'rotate') {
    my ($direction, $steps) = ($instructionRef->[1], $instructionRef->[2]);

    for (my $i = 0; $i < $steps; ++$i) {
      if ($direction eq 'left') {
        $scrambled = substr($scrambled, 1) . substr($scrambled, 0, 1);
      }
      else {
        $scrambled = substr($scrambled, $len - 1, 1) . substr($scrambled, 0, $len - 1);
      }
    }
  }
  elsif ($instruction eq 'rotatep') {
    $scrambled = RotateOnLetter($scrambled, $instructionRef->[1]);
  }
  elsif ($instruction eq 'reverse') {
    my ($from, $to) = ($instructionRef->[1], $instructionRef->[2]);
    my $sublen = ($to - $from) + 1;
    substr($scrambled, $from, $sublen) = reverse(substr($scrambled, $from, $sublen));    
  }
  elsif ($instruction eq 'move') {
    my ($from, $to) = ($instructionRef->[1], $instructionRef->[2]);
    my $fromChar = substr($scrambled, $from, 1);
    substr($scrambled, $from, 1) = '';
    substr($scrambled, $to, 0) = $fromChar;
  }
}

print "Scrambled: $scrambled\n";

$scrambled = 'fbgdceah';
print "New scrambled: $scrambled\n";

foreach my $instructionRef (reverse(@instructions)) {
  my $instruction = $instructionRef->[0];
  if ($instruction eq 'swapp') {
    # equivalent
    my ($from, $to) = ($instructionRef->[1], $instructionRef->[2]);
    my $tmp = substr($scrambled, $to, 1);
    substr($scrambled, $to, 1) = substr($scrambled, $from, 1);
    substr($scrambled, $from, 1) = $tmp;
  }
  elsif ($instruction eq 'swapl') {
    # equivalent
    my ($letter1, $letter2) = ($instructionRef->[1], $instructionRef->[2]);
    $scrambled =~ s/$letter1/./g;
    $scrambled =~ s/$letter2/$letter1/g;
    $scrambled =~ s/\./$letter2/g;
  }
  elsif ($instruction eq 'rotate') {
    my ($direction, $steps) = ($instructionRef->[1], $instructionRef->[2]);

    for (my $i = 0; $i < $steps; ++$i) {
      if ($direction eq 'left') {
        # actually rotate right
        $scrambled = substr($scrambled, $len - 1, 1) . substr($scrambled, 0, $len - 1);
      }
      else {
        # actually rotate left
        $scrambled = substr($scrambled, 1) . substr($scrambled, 0, 1);
      }
    }
  }
  elsif ($instruction eq 'rotatep') {
    my $letter = $instructionRef->[1];
    my $oldStr = $scrambled;
    # Rotate left until a rotatep would result in this string
    for (my $i = 0; $i < $len; ++$i) {
      $oldStr = substr($oldStr, 1) . substr($oldStr, 0, 1);
      if ($scrambled eq RotateOnLetter($oldStr, $letter)) {
        $scrambled = $oldStr;
        last;
      }
    }
  }
  elsif ($instruction eq 'reverse') {
    # equivalent
    my ($from, $to) = ($instructionRef->[1], $instructionRef->[2]);
    my $sublen = ($to - $from) + 1;
    substr($scrambled, $from, $sublen) = reverse(substr($scrambled, $from, $sublen));    
  }
  elsif ($instruction eq 'move') {
    # reversing to and from should be sufficient
    my ($to, $from) = ($instructionRef->[1], $instructionRef->[2]);
    my $fromChar = substr($scrambled, $from, 1);
    substr($scrambled, $from, 1) = '';
    substr($scrambled, $to, 0) = $fromChar;
  }
}

print "Original: $scrambled\n";
