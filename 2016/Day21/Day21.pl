use strict;
use warnings;

sub SwapPosition {
  my ($str, $from, $to) = @_;

  my $tmp = substr($str, $to, 1);
  substr($str, $to, 1) = substr($str, $from, 1);
  substr($str, $from, 1) = $tmp;

  return $str;
}

sub SwapLetter {
  my ($str, $char1, $char2) = @_;

  $str =~ s/$char1/./g;
  $str =~ s/$char2/$char1/g;
  $str =~ s/\./$char2/g;

  return $str;
}

sub Rotate {
  my ($str, $direction, $steps) = @_;

  my $len = length($str);
  for (my $i = 0; $i < $steps; ++$i) {
    if ($direction eq 'left') {
      $str = substr($str, 1) . substr($str, 0, 1);
    }
    else {
      $str = substr($str, $len - 1, 1) . substr($str, 0, $len - 1);
    }
  }

  return $str;
}

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

sub ReverseSection {
  my ($str, $from, $to) = @_;

  my $sublen = ($to - $from) + 1;
  substr($str, $from, $sublen) = reverse(substr($str, $from, $sublen));

  return $str;
}

sub Move {
  my ($str, $from, $to) = @_;

  my $fromChar = substr($str, $from, 1);
  substr($str, $from, 1) = '';
  substr($str, $to, 0) = $fromChar;

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
    $scrambled = SwapPosition($scrambled, $instructionRef->[1], $instructionRef->[2]);
  }
  elsif ($instruction eq 'swapl') {
    $scrambled = SwapLetter($scrambled, $instructionRef->[1], $instructionRef->[2]);
  }
  elsif ($instruction eq 'rotate') {
    $scrambled = Rotate($scrambled, $instructionRef->[1], $instructionRef->[2]);
  }
  elsif ($instruction eq 'rotatep') {
    $scrambled = RotateOnLetter($scrambled, $instructionRef->[1]);
  }
  elsif ($instruction eq 'reverse') {
    $scrambled = ReverseSection($scrambled, $instructionRef->[1], $instructionRef->[2]);
  }
  elsif ($instruction eq 'move') {
    $scrambled = Move($scrambled, $instructionRef->[1], $instructionRef->[2]);
  }
}

print "Scrambled: $scrambled\n";

$scrambled = 'fbgdceah';
print "New scrambled: $scrambled\n";

foreach my $instructionRef (reverse(@instructions)) {
  my $instruction = $instructionRef->[0];
  if ($instruction eq 'swapp') {
    # equivalent
    $scrambled = SwapPosition($scrambled, $instructionRef->[1], $instructionRef->[2]);
  }
  elsif ($instruction eq 'swapl') {
    # equivalent
    $scrambled = SwapLetter($scrambled, $instructionRef->[1], $instructionRef->[2]);
  }
  elsif ($instruction eq 'rotate') {
    if ($instructionRef->[1] eq 'left') {
      $scrambled = Rotate($scrambled, 'right', $instructionRef->[2]);
    }
    else {
      $scrambled = Rotate($scrambled, 'left', $instructionRef->[2]);
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
    $scrambled = ReverseSection($scrambled, $instructionRef->[1], $instructionRef->[2]);
  }
  elsif ($instruction eq 'move') {
    # reversing to and from should be sufficient
    $scrambled = Move($scrambled, $instructionRef->[2], $instructionRef->[1]);
  }
}

print "Original: $scrambled\n";
