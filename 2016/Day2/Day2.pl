use strict;
use warnings;

use feature "switch";

sub move_one {
  my ($num, $direction) = @_;

  given ($num) {
    when (1) {
      given ($direction) {
        when ('U') {
          return 1;
        }
        when ('R') {
          return 2;
        }
        when ('L') {
          return 1;
        }
        when ('D') {
          return 4;
        }
      }
    }
    when (2) {
      given ($direction) {
        when ('U') {
          return 2;
        }
        when ('R') {
          return 3;
        }
        when ('L') {
          return 1;
        }
        when ('D') {
          return 5;
        }
      }
    }
    when (3) {
      given ($direction) {
        when ('U') {
          return 3;
        }
        when ('R') {
          return 3;
        }
        when ('L') {
          return 2;
        }
        when ('D') {
          return 6;
        }
      }
    }
    when (4) {
      given ($direction) {
        when ('U') {
          return 1;
        }
        when ('R') {
          return 5;
        }
        when ('L') {
          return 4;
        }
        when ('D') {
          return 7;
        }
      }
    }
    when (5) {
      given ($direction) {
        when ('U') {
          return 2;
        }
        when ('R') {
          return 6;
        }
        when ('L') {
          return 4;
        }
        when ('D') {
          return 8;
        }
      }
    }
    when (6) {
      given ($direction) {
        when ('U') {
          return 3;
        }
        when ('R') {
          return 6;
        }
        when ('L') {
          return 5;
        }
        when ('D') {
          return 9;
        }
      }
    }
    when (7) {
      given ($direction) {
        when ('U') {
          return 4;
        }
        when ('R') {
          return 8;
        }
        when ('L') {
          return 7;
        }
        when ('D') {
          return 7;
        }
      }
    }
    when (8) {
      given ($direction) {
        when ('U') {
          return 5;
        }
        when ('R') {
          return 9;
        }
        when ('L') {
          return 7;
        }
        when ('D') {
          return 8;
        }
      }
    }
    when (9) {
      given ($direction) {
        when ('U') {
          return 6;
        }
        when ('R') {
          return 9;
        }
        when ('L') {
          return 8;
        }
        when ('D') {
          return 9;
        }
      }
    }
  }
  return $num;
}

sub move {
  my ($num, $directions) = @_;

  my @chars = split(//, $directions);
  foreach my $direction (@chars) {
    $num = move_one($num, $direction);
  }
  return $num;
}

sub move_one_letter {
  my ($num, $direction) = @_;

  given ($num) {
    when ('1') {
      given ($direction) {
        when ('U') {
          return '1';
        }
        when ('R') {
          return '1';
        }
        when ('L') {
          return '1';
        }
        when ('D') {
          return '3';
        }
      }
    }
    when ('2') {
      given ($direction) {
        when ('U') {
          return '2';
        }
        when ('R') {
          return '3';
        }
        when ('L') {
          return '2';
        }
        when ('D') {
          return '6';
        }
      }
    }
    when ('3') {
      given ($direction) {
        when ('U') {
          return '1';
        }
        when ('R') {
          return '4';
        }
        when ('L') {
          return '2';
        }
        when ('D') {
          return '7';
        }
      }
    }
    when ('4') {
      given ($direction) {
        when ('U') {
          return '4';
        }
        when ('R') {
          return '4';
        }
        when ('L') {
          return '3';
        }
        when ('D') {
          return '8';
        }
      }
    }
    when ('5') {
      given ($direction) {
        when ('U') {
          return '5';
        }
        when ('R') {
          return '6';
        }
        when ('L') {
          return '5';
        }
        when ('D') {
          return '5';
        }
      }
    }
    when ('6') {
      given ($direction) {
        when ('U') {
          return '2';
        }
        when ('R') {
          return '7';
        }
        when ('L') {
          return '5';
        }
        when ('D') {
          return 'A';
        }
      }
    }
    when ('7') {
      given ($direction) {
        when ('U') {
          return '3';
        }
        when ('R') {
          return '8';
        }
        when ('L') {
          return '6';
        }
        when ('D') {
          return 'B';
        }
      }
    }
    when ('8') {
      given ($direction) {
        when ('U') {
          return '4';
        }
        when ('R') {
          return '9';
        }
        when ('L') {
          return '7';
        }
        when ('D') {
          return 'C';
        }
      }
    }
    when ('9') {
      given ($direction) {
        when ('U') {
          return '9';
        }
        when ('R') {
          return '9';
        }
        when ('L') {
          return '8';
        }
        when ('D') {
          return '9';
        }
      }
    }
    when ('A') {
      given ($direction) {
        when ('U') {
          return '6';
        }
        when ('R') {
          return 'B';
        }
        when ('L') {
          return 'A';
        }
        when ('D') {
          return 'A';
        }
      }
    }
    when ('B') {
      given ($direction) {
        when ('U') {
          return '7';
        }
        when ('R') {
          return 'C';
        }
        when ('L') {
          return 'A';
        }
        when ('D') {
          return 'D';
        }
      }
    }
    when ('C') {
      given ($direction) {
        when ('U') {
          return '8';
        }
        when ('R') {
          return 'C';
        }
        when ('L') {
          return 'B';
        }
        when ('D') {
          return 'C';
        }
      }
    }
    when ('D') {
      given ($direction) {
        when ('U') {
          return 'B';
        }
        when ('R') {
          return 'D';
        }
        when ('L') {
          return 'D';
        }
        when ('D') {
          return 'D';
        }
      }
    }
  }
  return $num;
}

sub move_letter {
  my ($num, $directions) = @_;

  my @chars = split(//, $directions);
  foreach my $direction (@chars) {
    $num = move_one_letter($num, $direction);
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

foreach my $line (@lines) {
  $num = move($num, $line);
  print "$num";
}

print "\nNew code: ";

foreach my $line (@lines) {
  $num = move_letter($num, $line);
  print "$num";
}
print "\n";