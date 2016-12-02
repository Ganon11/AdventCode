use strict;
use warnings;

use feature "switch";

sub MoveOne {
  my ($num, $direction) = @_;

  given ($num) {
    when (1) {
      given ($direction) {
        return 1 when ['U', 'L'];
        return 2 when ['R'];
        return 4 when ['D'];
      }
    }
    when (2) {
      given ($direction) {
        return 2 when ['U'];
        return 3 when ['R'];
        return 1 when ['L'];
        return 5 when ['D'];
      }
    }
    when (3) {
      given ($direction) {
        return 3 when ['U', 'R'];
        return 2 when ['L'];
        return 6 when ['D'];
      }
    }
    when (4) {
      given ($direction) {
        return 1 when ['U'];
        return 5 when ['R'];
        return 4 when ['L'];
        return 7 when ['D'];
      }
    }
    when (5) {
      given ($direction) {
        return 2 when ['U'];
        return 6 when ['R'];
        return 4 when ['L'];
        return 8 when ['D'];
      }
    }
    when (6) {
      given ($direction) {
        return 3 when ['U'];
        return 6 when ['R'];
        return 5 when ['L'];
        return 9 when ['D'];
      }
    }
    when (7) {
      given ($direction) {
        return 7 when ['L', 'D'];
        return 4 when ['U'];
        return 8 when ['R'];
      }
    }
    when (8) {
      given ($direction) {
        return 5 when ['U'];
        return 9 when ['R'];
        return 7 when ['L'];
        return 8 when ['D'];
      }
    }
    when (9) {
      given ($direction) {
        return 9 when ['R', 'D'];
        return 6 when ['U'];
        return 8 when ['L'];
      }
    }
  }
  return $num;
}

sub MoveOneLetter {
  my ($num, $direction) = @_;

  given ($num) {
    when ('1') {
      given ($direction) {
        return '1' when ['U', 'R', 'L'];
        return '3' when ['D'];
      }
    }
    when ('2') {
      given ($direction) {
        return '2' when ['U', 'L'];
        return '3' when ['R'];
        return '6' when ['D'];
      }
    }
    when ('3') {
      given ($direction) {
        return '1' when ['U'];
        return '4' when ['R'];
        return '2' when ['L'];
        return '7' when ['D'];
      }
    }
    when ('4') {
      given ($direction) {
        return '4' when ['U', 'R'];
        return '3' when ['L'];
        return '8' when ['D'];
      }
    }
    when ('5') {
      given ($direction) {
        return '5' when ['U', 'L', 'D'];
        return '6' when ['R'];
      }
    }
    when ('6') {
      given ($direction) {
        return '2' when ['U'];
        return '7' when ['R'];
        return '5' when ['L'];
        return 'A' when ['D'];
      }
    }
    when ('7') {
      given ($direction) {
        return '3' when ['U'];
        return '8' when ['R'];
        return '6' when ['L'];
        return 'B' when ['D'];
      }
    }
    when ('8') {
      given ($direction) {
        return '4' when ['U'];
        return '9' when ['R'];
        return '7' when ['L'];
        return 'C' when ['D'];
      }
    }
    when ('9') {
      given ($direction) {
        return '9' when ['U', 'R', 'D'];
        return '8' when ['L'];
      }
    }
    when ('A') {
      given ($direction) {
        return 'A' when ['L', 'D'];
        return '6' when ['U'];
        return 'B' when ['R'];
      }
    }
    when ('B') {
      given ($direction) {
        return '7' when ['U'];
        return 'C' when ['R'];
        return 'A' when ['L'];
        return 'D' when ['D'];
      }
    }
    when ('C') {
      given ($direction) {
        return 'C' when ['R', 'D'];
        return '8' when ['U'];
        return 'B' when ['L'];
      }
    }
    when ('D') {
      given ($direction) {
        return 'D' when ['R', 'L', 'D'];
        return 'B' when ['U'];
      }
    }
  }
  return $num;
}

sub Move {
  my ($num, $directions, $subref) = @_;

  my @chars = split(//, $directions);
  foreach my $direction (@chars) {
    $num = &$subref($num, $direction);
  }

  return $num;
}

my $inputfile = shift;
if (!defined($inputfile)) {
  $inputfile = 'Input.txt';
}

open(my $fh, '<', $inputfile) or die "Couldn't open input file $inputfile: $!";
chomp(my @lines = <$fh>);
close($fh);

my $num = 5;

print "\nFirst code: ";

foreach my $line (@lines) {
  my $ref = \&MoveOne;
  $num = Move($num, $line, $ref);
  print "$num";
}

print "\nNew code: ";

foreach my $line (@lines) {
  my $ref = \&MoveOneLetter;
  $num = Move($num, $line, $ref);
  print "$num";
}

print "\n";