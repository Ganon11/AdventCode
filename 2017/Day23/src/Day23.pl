use strict;
use warnings;
use Scalar::Util qw(looks_like_number);
use Switch;
use Math::Prime::Util qw(is_prime);

sub GetRegisterValue {
  my $registerRef = shift;
  my $register = shift;
  if (!exists($$registerRef{$register})) {
    $$registerRef{$register} = 0;
  }

  return $$registerRef{$register};
}

sub Debug {
  my @instructions = @_;
  my $mulCount = 0;
  my %registers = ();
  my $eip = 0;

  while ((0 <= $eip) and ($eip < scalar(@instructions))) {
    my @instruction = @{ $instructions[$eip] };
    my $command = $instruction[0];
    my $nextInstruction = $eip + 1;
    switch ($command) {
      case 'set' {
        my ($x, $y) = ($instruction[1], $instruction[2]);
        GetRegisterValue(\%registers, $x);
        if (looks_like_number($y)) {
          $registers{$x} = int($y);
        } else {
          $registers{$x} = GetRegisterValue(\%registers, $y);
        }
      }
      case 'sub' {
        my ($x, $y) = ($instruction[1], $instruction[2]);
        GetRegisterValue(\%registers, $x);
        if (looks_like_number($y)) {
          $registers{$x} -= int($y);
        } else {
          $registers{$x} -= GetRegisterValue(\%registers, $y);
        }
      }
      case 'mul' {
        ++$mulCount;

        my ($x, $y) = ($instruction[1], $instruction[2]);
        GetRegisterValue(\%registers, $x);
        if (looks_like_number($y)) {
          $registers{$x} *= int($y);
        } else {
          $registers{$x} *= GetRegisterValue(\%registers, $y);
        }
      }
      case 'jnz' {
        my ($x, $y) = ($instruction[1], $instruction[2]);

        my $checkValue;
        if (looks_like_number($x)) {
          $checkValue = int($x);
        } else {
          $checkValue = GetRegisterValue(\%registers, $x);
        }

        if (0 != $checkValue) {
          if (looks_like_number($y)) {
            $nextInstruction = $eip + int($y);
          } else {
            $nextInstruction = $eip + GetRegisterValue(\%registers, $y);
          }
        }
      }
    }

    $eip = $nextInstruction;
  }

  return $mulCount;
}

my $inputFile;
if (scalar(@ARGV) > 0) {
  $inputFile = shift @ARGV;
} else {
  $inputFile = '../input/Input.txt';
}

my @instructions;
open(my $fh, "<", $inputFile) or die "Couldn't open $inputFile: $!";
while (<$fh>) {
  my $line = $_;
  chomp($line);
  if ($line =~ m/^set (\w) (\S+)$/) {
    push(@instructions, ['set', $1, $2]);
  } elsif ($line =~ m/^sub (\w) (\S+)$/) {
    push(@instructions, ['sub', $1, $2]);
  } elsif ($line =~ m/^mul (\w) (\S+)$/) {
    push(@instructions, ['mul', $1, $2]);
  } elsif ($line =~ m/^jnz (\S+) (\S+)$/) {
    push(@instructions, ['jnz', $1, $2]);
  } else {
    print "Unrecognized command: $line\n";
    next;
  }
}
close($fh);

my $mulCount = Debug(@instructions);
print "Part 1: $mulCount\n";

my $b = 109300;
my $c = 126301;
my $h = 0;
while ($b < $c) {
  ++$h if !is_prime($b);
  $b += 17;
}

print "Part 2: $h\n";

=begin comment

a = 1
b = 93
c = 93
(jump)
(these 5 lines had been jumped before)
b = 9300
b = 109300
c = 109300
c = 126300
f = 1
(this was part of debug)
d = 2
e = 2
g = 2
g = 4
g = -109296
(jump over the set f)
e = 3
g = 3
g = -109296
(jump back)
g = 2
g = 6
g = -109294
(jump over the set f)
e = 4
g = 4
g = -109296
(jump back)
(... eventually...)
g = 2 (e is 54650)
g = 109300
g = 0
(don't jump)
f = 0
e = 54651
g = 54651
(... eventually)
g = 2 (e is 109299)
g = 218598
g = 109302
(jump over the set f)
e = 109300
g = 109300
g = 0
(don't jump)
d = 3
=end comment
=cut