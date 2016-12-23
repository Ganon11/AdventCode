use strict;
use warnings;
use Scalar::Util qw(looks_like_number);
use Data::Dumper;
use Time::HiRes qw(usleep);

my $inputfile = shift;
if (!defined($inputfile)) {
  $inputfile = 'Sample.txt';
}

open(my $fh, '<', $inputfile) or die "Couldn't open $inputfile: $!";
my @commands = ();
foreach my $line (<$fh>) {
  $line =~ s/\R//g; # Trim newlines
  if ($line =~ m/^cpy (-?\d+|a|b|c|d) (a|b|c|d)$/) {
    push(@commands, ['cpy', $1, $2]);
  }
  elsif ($line =~ m/^jnz (-?\d+|a|b|c|d) (-?\d+|a|b|c|d)$/) {
    my ($reg, $offset) = ($1, $2);
    push(@commands, ['jnz', $reg, $offset]);
  }
  elsif ($line =~ m/^inc (a|b|c|d)$/) {
    push(@commands, ['inc', $1]);
  }
  elsif ($line =~ m/^dec (a|b|c|d)$/) {
    push(@commands, ['dec', $1]);
  }
  elsif ($line =~ m/^tgl (-?\d+|a|b|c|d)$/) {
    my $offset = $1;
    $offset = int($offset) if looks_like_number($offset);
    push(@commands, ['tgl', $offset]);
  }
  elsif ($line =~ m/^mult$/) {
    push(@commands, ['mult']);
  }
  else {
    die "Unrecognized input sequence: '$line'";
  }
}
close($fh);

# print "Commands:\n";
# print Dumper \@commands;

my $numcommands = scalar(@commands);
my ($a, $b, $c, $d) = (12, 0, 0, 0); # Registers
my $i = 0; # Instruction pointer

while (0 <= $i && $i < $numcommands) {
  my $cmd = $commands[$i][0];
  # print "$i: $cmd ";
  # print "$commands[$i][1] " if defined($commands[$i][1]);
  # print "$commands[$i][2] " if defined($commands[$i][2]);

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
    } elsif ($dest eq 'd') {
      $d = $val;
    }

    #print "$i: cpy $val $dest\n";
    $i = $i + 1;
  }
  elsif ($cmd eq 'jnz') {
    my ($reg, $offset) = ($commands[$i][1], $commands[$i][2]);
    #print "$i: jnz $reg $offset\n";
    if ($offset eq 'a') {
      $offset = $a;
    }
    elsif ($offset eq 'b') {
      $offset = $b;
    }
    elsif ($offset eq 'c') {
      $offset = $c;
    }
    elsif ($offset eq 'd') {
      $offset = $d;
    }
    else {
      $offset = int($offset);
    }

    if (($reg eq 'a' && $a != 0)
      || ($reg eq 'b' && $b != 0)
      || ($reg eq 'c' && $c != 0)
      || ($reg eq 'd' && $d != 0)) {

      $i = $i + $offset;
    }
    elsif (looks_like_number($reg) && int($reg) != 0) {
      $i = $i + $offset;
    }
    else {
      $i = $i + 1;
    }
  }
  elsif ($cmd eq 'inc') {
    my $reg = $commands[$i][1];
    #print "$i: inc $reg\n";
    if ($reg eq 'a') {
      $a = $a + 1;
      #print "a: $a\n";
    }
    elsif ($reg eq 'b') {
      $b = $b + 1;
      #print "b: $b\n";
    }
    elsif ($reg eq 'c') {
      $c = $c + 1;
      #print "c: $c\n";
    }
    elsif ($reg eq 'd') {
      $d = $d + 1;
      #print "d: $d\n";
    }

    $i = $i + 1;
  }
  elsif ($cmd eq 'tgl') {
    my $offset = $commands[$i][1];
    #print "$i: tgl $offset\n";
    my $tglIndex = $i;
    $i = $i + 1;
    if ($offset eq 'a') {
      $tglIndex += $a;
    }
    elsif ($offset eq 'b') {
      $tglIndex += $b;
    }
    elsif ($offset eq 'c') {
      $tglIndex += $c;
    }
    elsif ($offset eq 'd') {
      $tglIndex += $d;
    }
    else {
      $tglIndex += $offset;
    }

    next if $tglIndex < 0;
    next if $tglIndex >= $numcommands;

    my $oldCmd = $commands[$tglIndex][0];
    if ($oldCmd eq 'inc') {
      $commands[$tglIndex][0] = 'dec';
    }
    elsif ($oldCmd eq 'tgl') {
      $commands[$tglIndex][0] = 'inc';
    }
    elsif ($oldCmd eq 'dec') {
      $commands[$tglIndex][0] = 'inc';
    }
    elsif ($oldCmd eq 'cpy') {
      $commands[$tglIndex][0] = 'jnz';
    }
    elsif ($oldCmd eq 'jnz') {
      $commands[$tglIndex][0] = 'cpy';
    }

    #print join(' ', @{$commands[$tglIndex]}), "\n";
  }
  elsif ($cmd eq 'dec') {
    # cmd is dec
    my $reg = $commands[$i][1];
    #print "$i: dec $reg\n";
    if ($reg eq 'a') {
      $a = $a - 1;
      #print "a: $a\n";
    }
    elsif ($reg eq 'b') {
      $b = $b - 1;
      #print "b: $b\n";
    }
    elsif ($reg eq 'c') {
      $c = $c - 1;
      #print "c: $c\n";
    }
    else {
      $d = $d - 1;
      #print "d: $d\n";
    }

    $i = $i + 1;
  }
  elsif ($cmd eq 'mult') {
    # Processor multiplication
    $a += ($b * $d);
    $c = 0;
    $d = 0;

    $i = $i + 1;
  }

  #print "[a: $a, b: $b, c: $c, d: $d]\n";
  #usleep(5000);
}

print "Finished execution: state is:\n";
print "\ta: $a\n";
print "\tb: $b\n";
print "\tc: $c\n";
print "\td: $d\n";
