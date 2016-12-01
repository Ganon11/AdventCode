use strict;
use warnings;

use feature "switch";

sub rotate {
  my ($orientation, $rotation) = @_;
  given ($orientation) {
    when (/^N$/) {
      if ($rotation eq 'R') {
        return 'E';
      } else {
        return 'W';
      }
    }
    when (/^E$/) {
      if ($rotation eq 'R') {
        return 'S';
      } else {
        return 'N';
      }
    }
    when (/^W$/) {
      if ($rotation eq 'R') {
        return 'N';
      } else {
        return 'S';
      }
    }
    when (/^S$/) {
      if ($rotation eq 'R') {
        return 'W';
      } else {
        return 'E';
      }
    }
    default {
      die 'Incorrect orientation provided';
    }
  }
}

sub walk {
  my ($orientation, $x, $y, $blocks) = @_;

  given ($orientation) {
    when (/^N$/) {
      return ($x, $y + $blocks);
    }
    when (/^E$/) {
      return ($x + $blocks, $y);
    }
    when (/^W$/) {
      return ($x - $blocks, $y);
    }
    when (/^S$/) {
      return ($x, $y - $blocks);
    }
    default {
      die 'Incorrect orientation provided';
    }
  }
}

sub walk_one {
  my ($orientation, $x, $y) = @_;

  given ($orientation) {
    when (/^N$/) {
      return ($x, $y + 1);
    }
    when (/^E$/) {
      return ($x + 1, $y);
    }
    when (/^W$/) {
      return ($x - 1, $y);
    }
    when (/^S$/) {
      return ($x, $y - 1);
    }
    default {
      die 'Incorrect orientation provided';
    }
  }
}

# Inserts a location into the hash
# Returns 1 if the location was already visited.
sub insert_location {
  my ($hashref, $x, $y) = @_;

  if (exists($hashref->{$x}) and exists($hashref->{$x}{$y})) {
    return 1;
  }

  $hashref->{$x}{$y} = 1;
  return 0;
}

my $inputfile = shift;

open(my $fh, '<', $inputfile) or die "Can't open < $inputfile: $!";
my $input = <$fh>;
chomp($input);

my @directions = split(/, /, $input);

my $orientation = 'N';
my $x = 0;
my $y = 0;
my %hash = ();
insert_location(\%hash, $x, $y);
my ($repeatX, $repeatY) = undef;

foreach my $dir (@directions) {
  my $rotation = substr($dir, 0, 1);
  my $blocks = int(substr($dir, 1));

  $orientation = rotate($orientation, $rotation);
  for (my $i = 0; $i < $blocks; ++$i) {
    ($x, $y) = walk_one($orientation, $x, $y);
    if (!defined($repeatX)) {
      if (1 == insert_location(\%hash, $x, $y)) {
        $repeatX = $x;
        $repeatY = $y;
      }
    }
  }
}

print "Final location: ($x, $y)\n";
my $distance = abs($x) + abs($y);
print "Number of blocks away: $distance\n";

if (defined($repeatX)) {
  print "First repeat location: ($repeatX, $repeatY)\n";
  $distance = abs($repeatX) + abs($repeatY);
  print "Number of blocks away: $distance\n";
} else {
  print "Couldn't find repeat location\n";
}
