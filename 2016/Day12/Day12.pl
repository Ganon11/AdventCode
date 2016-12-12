use strict;
use warnings;

use Data::Dumper;

my $inputfile = shift;
if (!defined($inputfile)) {
  $inputfile = 'Sample.txt';
}

open(my $fh, '<', $inputfile) or die "Couldn't open $inputfile: $!";
my @commands = ();
foreach my $line (<$fh>) {
  $line =~ s/\R//g; # Trim newlines
  if ($line =~ m/^cpy (\d+|a|b|c|d) (a|b|c|d)$/) {
    push(@commands, ['cpy', $1, $2]);
  }
  elsif ($line =~ m/^jnz (\d+|a|b|c|d) (-?)(\d+)$/) {
    my ($reg, $neg, $offset) = ($1, $2, int($3));
    if (defined($neg) && $neg eq '-') {
      $offset = -1 * $offset;
    }
    push(@commands, ['jnz', $reg, $offset]);
  }
  elsif ($line =~ m/^inc (a|b|c|d)$/) {
    push(@commands, ['inc', $1]);
  }
  elsif ($line =~ m/^dec (a|b|c|d)$/) {
    push(@commands, ['dec', $1]);
  }
  else {
    die "Unrecognized input sequence: '$line'";
  }
}
close($fh);

#print "Commands:\n";
#print Dumper \@commands;

my $numcommands = scalar(@commands);
my ($a, $b, $c, $d) = (0, 0, 1, 0); # Registers
my $i = 0; # Instruction pointer

while (0 <= $i && $i < $numcommands) {
  my $cmd = $commands[$i][0];

  if ($cmd eq 'cpy') {
    my $val = $commands[$i][1];
    if ($val eq 'a') {
      $val = $a;
    } elsif ($val eq 'b') {
      $val = $b;
    } elsif ($val eq 'c') {
      $val = $c;
    } elsif ($val eq 'd') {
      $val = $d;
    } else {
      $val = int($val);
    }

    my $dest = $commands[$i][2];
    if ($dest eq 'a') {
      $a = $val;
    } elsif ($dest eq 'b') {
      $b = $val;
    } elsif ($dest eq 'c') {
      $c = $val;
    } else {
      $d = $val;
    }

    $i = $i + 1;
  }
  elsif ($cmd eq 'jnz') {
    my ($reg, $offset) = ($commands[$i][1], $commands[$i][2]);

    if (($reg eq 'a' && $a != 0)
      || ($reg eq 'b' && $b != 0)
      || ($reg eq 'c' && $c != 0)
      || ($reg eq 'd' && $d != 0)) {

      $i = $i + $offset;
    }
    elsif (int($reg) != 0) {
      $i = $i + $offset;
    }
    else {
      $i = $i + 1;
    }
  }
  elsif ($cmd eq 'inc') {
    my $reg = $commands[$i][1];
    if ($reg eq 'a') {
      $a = $a + 1;
    }
    elsif ($reg eq 'b') {
      $b = $b + 1;
    }
    elsif ($reg eq 'c') {
      $c = $c + 1;
    }
    else {
      $d = $d + 1;
    }

    $i = $i + 1;
  }
  else {
    # cmd is dec
    my $reg = $commands[$i][1];
    if ($reg eq 'a') {
      $a = $a - 1;
    }
    elsif ($reg eq 'b') {
      $b = $b - 1;
    }
    elsif ($reg eq 'c') {
      $c = $c - 1;
    }
    else {
      $d = $d - 1;
    }

    $i = $i + 1;
  }
}

print "Finished execution: state is:\n";
print "\ta: $a\n";
print "\tb: $b\n";
print "\tc: $c\n";
print "\td: $d\n";
