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
  $inputfile = 'sample.txt';
}

print "Original: $password\n";

my @instructions = ();
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
    $password = SwapPosition($password, $instructionRef->[1], $instructionRef->[2]);
  }
  elsif ($instruction eq 'swapl') {
    $password = SwapLetter($password, $instructionRef->[1], $instructionRef->[2]);
  }
  elsif ($instruction eq 'rotate') {
    $password = Rotate($password, $instructionRef->[1], $instructionRef->[2]);
  }
  elsif ($instruction eq 'rotatep') {
    $password = RotateOnLetter($password, $instructionRef->[1]);
  }
  elsif ($instruction eq 'reverse') {
    $password = ReverseSection($password, $instructionRef->[1], $instructionRef->[2]);
  }
  elsif ($instruction eq 'move') {
    $password = Move($password, $instructionRef->[1], $instructionRef->[2]);
  }
}

print "Scrambled: $password\n";

$password = 'fbgdceah';
print "New scrambled: $password\n";

foreach my $instructionRef (reverse(@instructions)) {
  my $instruction = $instructionRef->[0];
  if ($instruction eq 'swapp') {
    # equivalent
    $password = SwapPosition($password, $instructionRef->[1], $instructionRef->[2]);
  }
  elsif ($instruction eq 'swapl') {
    # equivalent
    $password = SwapLetter($password, $instructionRef->[1], $instructionRef->[2]);
  }
  elsif ($instruction eq 'rotate') {
    if ($instructionRef->[1] eq 'left') {
      $password = Rotate($password, 'right', $instructionRef->[2]);
    }
    else {
      $password = Rotate($password, 'left', $instructionRef->[2]);
    }
  }
  elsif ($instruction eq 'rotatep') {
    my $letter = $instructionRef->[1];
    my $oldStr = $password;
    # Rotate left until a rotatep would result in this string
    for (my $i = 0; $i < length($password); ++$i) {
      $oldStr = substr($oldStr, 1) . substr($oldStr, 0, 1);
      if ($password eq RotateOnLetter($oldStr, $letter)) {
        $password = $oldStr;
        last;
      }
    }
  }
  elsif ($instruction eq 'reverse') {
    # equivalent
    $password = ReverseSection($password, $instructionRef->[1], $instructionRef->[2]);
  }
  elsif ($instruction eq 'move') {
    # reversing to and from should be sufficient
    $password = Move($password, $instructionRef->[2], $instructionRef->[1]);
  }
}

print "Original: $password\n";
