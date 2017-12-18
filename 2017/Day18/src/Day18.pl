use strict;
use warnings;
use Scalar::Util qw(looks_like_number);
use Switch;

sub GetRegisterValue {
  my $registerRef = shift;
  my $register = shift;
  if (!exists($$registerRef{$register})) {
    $$registerRef{$register} = 0;
  }

  return $$registerRef{$register};
}

sub Duet {
  my @instructions = @_;
  my $lastSound;
  my %registers = ();
  my $eip = 0;

  while ((0 <= $eip) and ($eip < scalar(@instructions))) {
    my @instruction = @{ $instructions[$eip] };
    my $command = $instruction[0];
    my $nextInstruction = $eip + 1;
    switch ($command) {
      case 'snd' {
        my $x = $instruction[1];
        if (looks_like_number($x)) {
          $lastSound = int($x);
        } else {
          $lastSound = GetRegisterValue(\%registers, $x);
        }
      }
      case 'set' {
        my ($x, $y) = ($instruction[1], $instruction[2]);
        GetRegisterValue(\%registers, $x);
        if (looks_like_number($y)) {
          $registers{$x} = int($y);
        } else {
          $registers{$x} = GetRegisterValue(\%registers, $y);
        }
      }
      case 'add' {
        my ($x, $y) = ($instruction[1], $instruction[2]);
        GetRegisterValue(\%registers, $x);
        if (looks_like_number($y)) {
          $registers{$x} += int($y);
        } else {
          $registers{$x} += GetRegisterValue(\%registers, $y);
        }
      }
      case 'mul' {
        my ($x, $y) = ($instruction[1], $instruction[2]);
        GetRegisterValue(\%registers, $x);
        if (looks_like_number($y)) {
          $registers{$x} *= int($y);
        } else {
          $registers{$x} *= GetRegisterValue(\%registers, $y);
        }
      }
      case 'mod' {
        my ($x, $y) = ($instruction[1], $instruction[2]);
        GetRegisterValue(\%registers, $x);
        if (looks_like_number($y)) {
          $registers{$x} %= int($y);
        } else {
          $registers{$x} %= GetRegisterValue(\%registers, $y);
        }
      }
      case 'rcv' {
        my $x = $instruction[1];
        if (0 != GetRegisterValue(\%registers, $x)) {
          return $lastSound;
        }
      }
      case 'jgz' {
        my ($x, $y) = ($instruction[1], $instruction[2]);

        my $checkValue;
        if (looks_like_number($x)) {
          $checkValue = int($x);
        } else {
          $checkValue = GetRegisterValue(\%registers, $x);
        }

        if (0 < $checkValue) {
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
}

sub OtherProgram {
  my $p = shift;
  switch ($p) {
    case 0 { return 1; }
    case 1 { return 0; }
  }
}

sub Duet2 {
  my @instructions = @_;
  my $program = 0;
  my @registers = (
    {
      'p' => 0
    },
    {
      'p' => 1
    }
  );
  my @eips = (0, 0);
  my @queues = ([], []);
  my $justSwapped = 0;
  my $sentFromOne = 0;

  while ((0 <= $eips[$program]) and ($eips[$program] < scalar(@instructions))) {
    my @instruction = @{ $instructions[$eips[$program]] };
    my $command = $instruction[0];
    my $nextInstruction = $eips[$program] + 1;

    switch ($command) {
      case 'snd' {
        $justSwapped = 0;

        my $x = $instruction[1];

        if (looks_like_number($x)) {
          push(@{ $queues[OtherProgram($program)] }, int($x));
        } else {
          push(@{ $queues[OtherProgram($program)] }, GetRegisterValue($registers[$program], $x));
        }

        if (1 == $program) {
          ++$sentFromOne;
        }
      }
      case 'set' {
        $justSwapped = 0;

        my ($x, $y) = ($instruction[1], $instruction[2]);
        GetRegisterValue($registers[$program], $x);
        if (looks_like_number($y)) {
          $registers[$program]->{$x} = int($y);
        } else {
          $registers[$program]->{$x} = GetRegisterValue($registers[$program], $y);
        }
      }
      case 'add' {
        $justSwapped = 0;

        my ($x, $y) = ($instruction[1], $instruction[2]);
        GetRegisterValue($registers[$program], $x);
        if (looks_like_number($y)) {
          $registers[$program]->{$x} += int($y);
        } else {
          $registers[$program]->{$x} += GetRegisterValue($registers[$program], $y);
        }
      }
      case 'mul' {
        $justSwapped = 0;

        my ($x, $y) = ($instruction[1], $instruction[2]);
        GetRegisterValue($registers[$program], $x);
        if (looks_like_number($y)) {
          $registers[$program]->{$x} *= int($y);
        } else {
          $registers[$program]->{$x} *= GetRegisterValue($registers[$program], $y);
        }
      }
      case 'mod' {
        $justSwapped = 0;

        my ($x, $y) = ($instruction[1], $instruction[2]);
        GetRegisterValue($registers[$program], $x);
        if (looks_like_number($y)) {
          $registers[$program]->{$x} %= int($y);
        } else {
          $registers[$program]->{$x} %= GetRegisterValue($registers[$program], $y);
        }
      }
      case 'rcv' {
        my $x = $instruction[1];
        if (0 == scalar(@{ $queues[$program] })) {
          if (1 == $justSwapped) {
            # Deadlock!
            return $sentFromOne;
          }

          # Switch contexts
          $program = OtherProgram($program);
          $nextInstruction = $eips[$program];
          $justSwapped = 1;
        } else {
          $justSwapped = 0;

          $registers[$program]->{$x} = shift(@{ $queues[$program] });
        }
      }
      case 'jgz' {
        $justSwapped = 0;

        my ($x, $y) = ($instruction[1], $instruction[2]);
        my $checkValue;
        if (looks_like_number($x)) {
          $checkValue = int($x);
        } else {
          $checkValue = GetRegisterValue($registers[$program], $x);
        }
        if (0 < $checkValue) {
          if (looks_like_number($y)) {
            $nextInstruction = $eips[$program] + int($y);
          } else {
            $nextInstruction = $eips[$program] + GetRegisterValue($registers[$program], $y);
          }
        }
      }
    }

    $eips[$program] = $nextInstruction;
  }

  return $sentFromOne;
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
  if ($line =~ m/^snd (\S+)$/) {
    push(@instructions, ['snd', $1]);
  } elsif ($line =~ m/^set (\w) (\S+)$/) {
    push(@instructions, ['set', $1, $2]);
  } elsif ($line =~ m/^add (\w) (\S+)$/) {
    push(@instructions, ['add', $1, $2]);
  } elsif ($line =~ m/^mul (\w) (\S+)$/) {
    push(@instructions, ['mul', $1, $2]);
  } elsif ($line =~ m/^mod (\w) (\S+)$/) {
    push(@instructions, ['mod', $1, $2]);
  } elsif ($line =~ m/^rcv (\w)$/) {
    push(@instructions, ['rcv', $1]);
  } elsif ($line =~ m/^jgz (\S+) (\S+)$/) {
    push(@instructions, ['jgz', $1, $2]);
  } else {
    print "Unrecognized command: $line\n";
    next;
  }
}
close($fh);

my $lastSound = Duet(@instructions);
print "Last sound: $lastSound\n";

my $sent = Duet2(@instructions);
print "Sent from program 1: $sent\n";
